(function (global, factory) {
  typeof exports === 'object' && typeof module !== 'undefined' ? factory(require('agora-rtc-sdk'), require('eventemitter3'), require('formdata-polyfill')) :
  typeof define === 'function' && define.amd ? define(['agora-rtc-sdk', 'eventemitter3', 'formdata-polyfill'], factory) :
  (global = global || self, factory(global.AgoraRTC, global.EventEmitter3, global.formData));
}(this, function (AgoraRTC, EventEmitter3, formData) { 'use strict';

  AgoraRTC = AgoraRTC && AgoraRTC.hasOwnProperty('default') ? AgoraRTC['default'] : AgoraRTC;
  EventEmitter3 = EventEmitter3 && EventEmitter3.hasOwnProperty('default') ? EventEmitter3['default'] : EventEmitter3;

  class RTCClient {
      constructor(options) {
          this._dispatch = new EventEmitter3();
          this._options = options;
          this._client = AgoraRTC.createClient(options);
          this._state = {
              joined: false,
              published: false
          };
          this.subClientEvents();
      }
      static get rtc() {
          return this._rtc;
      }
      static getRTCInfo() {
          if (!this._rtcInfo) {
              this._rtcInfo = document.querySelector("#local_video_info");
          }
          return this._rtcInfo;
      }
      static async getDevices() {
          return new Promise((resolve, reject) => {
              navigator.mediaDevices.getUserMedia({
                  video: true,
                  audio: true
              }).then((stream) => {
                  return AgoraRTC.getDevices((items) => {
                      const _items = items
                          .filter((item) => ['audioinput', 'videoinput'].indexOf(item.kind) !== -1)
                          .map((item) => ({
                          name: item.label,
                          value: item.deviceId,
                          kind: item.kind,
                      }));
                      stream.getTracks().forEach((track) => track.stop());
                      const videos = [];
                      const audios = [];
                      for (let item of _items) {
                          if ('videoinput' == item.kind) {
                              if (!item.name) {
                                  item.name = `camera-${videos.length}`;
                              }
                              videos.push(item);
                          }
                          if ('audioinput' == item.kind) {
                              if (!item.name) {
                                  item.name = `microphone-${audios.length}`;
                              }
                              audios.push(item);
                          }
                      }
                      return resolve({
                          videos,
                          audios
                      });
                  });
              });
          });
      }
      static async create(options) {
          if (!this._rtc) {
              const rtc = new RTCClient(options);
              await rtc.init();
              this._rtc = rtc;
              console.log("[rtc-client create]");
          }
      }
      static async join() {
          if (!this._rtc)
              return;
          if (!this._rtc._state.joined) {
              await this._rtc.join();
              await this._rtc.createStream(this._rtc._options);
              await this._rtc.play("local_stream");
          }
      }
      static async publish() {
          if (!this._rtc)
              return;
          if (this._rtc._state.published === false) {
              await this._rtc.setRole('host');
              return this._rtc.publish();
          }
      }
      static async unpublish() {
          if (!this._rtc)
              return;
          if (this._rtc._state.published === true) {
              return this._rtc.unpublish();
          }
      }
      static async leave() {
          if (!this._rtc)
              return;
          await this.unpublish();
          await this._rtc.stopLocalStream();
          await this._rtc.leave();
          delete this._rtc;
      }
      static async subscribe(stream) {
          if (!this._rtc)
              return;
          return this._rtc.subscribe(stream);
      }
      static addView(id) {
          if (!document.querySelector(`#remote_video_${id}`)) {
              const newVideoView = document.createElement("div");
              newVideoView.setAttribute("id", `remote_video_${id}`);
              newVideoView.setAttribute("class", "video-view");
              const newVideoInfo = document.createElement("div");
              newVideoInfo.setAttribute("class", "video-info");
              newVideoInfo.innerHTML = `<span class="uid">uid: ${id}</span>`;
              newVideoView.appendChild(newVideoInfo);
              document.querySelector("#video").appendChild(newVideoView);
          }
      }
      static removeView(id) {
          if (document.querySelector(`#remote_video_${id}`)) {
              document.querySelector(`#remote_video_${id}`).remove();
          }
      }
      static startLiveStreaming(url, transcodingConfig) {
          if (!this._rtc)
              return;
          const _url = decodeURIComponent(url);
          this._rtc.startLiveStreaming(_url, transcodingConfig);
      }
      static stopLiveStreaming(url) {
          if (!this._rtc)
              return;
          this._rtc.stopLiveStreaming(url);
      }
      init() {
          return new Promise((resolve, reject) => {
              this._client.init(this._options.appID, () => {
                  resolve(true);
              }, (err) => {
                  reject(err);
              });
          });
      }
      join() {
          const { token, channelName } = this._options;
          return new Promise((resolve, reject) => {
              this._client.join(token, channelName, this._options.uid, (uid) => {
                  this._state.joined = true;
                  resolve(uid);
              }, (err) => {
                  reject(err);
              });
          });
      }
      createStream(streamConfig) {
          this._localStream = AgoraRTC.createStream(streamConfig);
          this.subStreamEvents();
          return new Promise((resolve, reject) => {
              this._localStream.setVideoProfile(this._options.resolution);
              this._localStream.init(() => {
                  resolve();
              }, (err) => {
                  reject(err);
              });
          });
      }
      setRole(role) {
          return new Promise((resolve, reject) => {
              this._client.setClientRole(role, resolve, reject);
          });
      }
      async subscribe(stream) {
          return this._client.subscribe(stream, (err) => {
              this._dispatch.emit("subscribeFailed", err);
          });
      }
      async play(id) {
          return this._localStream.play(id, { fit: "cover" });
      }
      async stopLocalStream() {
          this._localStream.close();
          return this._localStream.stop();
      }
      async publish() {
          this._state.published = true;
          return this._client.publish(this._localStream, (err) => {
              this._dispatch.emit("publishFailed", err);
          });
      }
      async unpublish() {
          this._state.published = false;
          return this._client.unpublish(this._localStream, (err) => {
              this._dispatch.emit("unpublishFailed", err);
          });
      }
      startLiveStreaming(url, transcodingConfig) {
          this._client.setLiveTranscoding(transcodingConfig);
          this._client.startLiveStreaming(url, true);
      }
      stopLiveStreaming(url) {
          this._client.stopLiveStreaming(url);
      }
      leave() {
          return new Promise((resolve, reject) => {
              this._client.leave(() => {
                  resolve();
              }, (err) => {
                  reject(err);
              });
          });
      }
      on(eventName, cb) {
          this._dispatch.on(eventName, cb);
      }
      subStreamEvents() {
          const events = [
              "accessAllowed",
              "accessDenied",
              "stopScreenSharing",
              "audioMixingPlayed",
              "audioMixingFinished"
          ];
          for (let event of events) {
              this._localStream.on(event, (args) => {
                  this._dispatch.emit(event, args);
              });
          }
      }
      subClientEvents() {
          const events = [
              "stream-published",
              "stream-added",
              "stream-removed",
              "stream-subscribed",
              "peer-leave",
              "mute-audio",
              "unmute-audio",
              "mute-video",
              "unmute-video",
              "client-banned",
              "active-speaker",
              "volume-indicator",
              "liveStreamingStarted",
              "liveStreamingFailed",
              "liveStreamingStopped",
              "liveTranscodingUpdated",
              "streamInjectedStatus",
              "onTokenPrivilegeWillExpire",
              "onTokenPrivilegeDidExpire",
              "error",
              "network-type-changed",
              "recording-device-changed",
              "playout-device-changed",
              "camera-changed",
              "stream-type-changed",
              "connection-state-change",
              "stream-reconnect-start",
              "stream-reconnect-end",
              "client-role-changed",
              "peer-online",
              "reconnect",
              "connected",
              "network-quality",
              "stream-fallback",
              "stream-updated",
              "exception"
          ];
          for (let event of events) {
              this._client.on(event, (evt) => {
                  this._dispatch.emit(event, evt);
              });
          }
      }
  }

  const FormData = !window.FormData ? formData : window.FormData;

  const i18n = (key, val) => {
    const zh = {
      channelName_required: "房间名不能为空",
      appID_required: "appID不能为空",
      transcodingUrl_required: "CDN推流地址不能为空",
      uid_required: "UID不能为空"
    };
    const match = zh[key];
    if (match) {
      // return match.replace("$name", val);
      return match;
    }
  };

  const Notice = (msg) => {
    document.querySelector(".notice").innerHTML =  
    `<span class="flex notice-alert">${msg}</span>`;
    
    setTimeout(() => {
      document.querySelector(".notice").innerHTML = '';
    }, 1000);
  };

  const ErrorNotice = (msg) => {
    document.querySelector(".notice").innerHTML = 
    `<span class="flex danger-alert">${msg}</span>`;
    setTimeout(() => {
      document.querySelector(".notice").innerHTML = '';
    }, 1000);
  };


  const checkForm = (id, fields) => {
    const _result = serializeFormDOM(id);
    const emptyKeys = Object.keys(_result).filter(key => 
      null === _result[key] || undefined === _result[key]
      || (typeof _result[key] === 'string' && !_result[key]));
    const result = {};
    for (let key of emptyKeys) {
      if (fields.indexOf(key) != -1) {
        result[key] = i18n(`${key}_required`, _result[key]);
      }
    }
    result[Object.keys(result)[0]] && ErrorNotice(result[Object.keys(result)[0]]);
    return _result;

  };

  const serializeFormDOM = (id) => {
    const form = document.querySelector(id);
    const _data = Array.from(
      new FormData(form),
      e => e.map(encodeURIComponent).join("=")
    );
    const result = {};
    for (let key of _data) {
      const vals = key.split("=");
      result[vals[0]] = vals[1];
    }
    return result;
  };

  class Select {
    constructor (opts) {
      this._opts = opts;
      this._dom = document.querySelector(`#${opts.id}`);
      this._direction = 'fa-chevron-down';
      const customSelect = document.createElement("div");
      customSelect.setAttribute("class", `${opts.class}`);
      this._currentSelect = {value: '', name: ''};
      let itemTemplate = '';
      for (let item of opts.items) {
        itemTemplate += 
          `<span class="select-item" value="${item.value}">
          ${item.name}
        </span>`;
      }
      customSelect.innerHTML = `
      <i id="icon-${this._opts.id}" class="fas ${this._direction} select-chevron"></i>
      <div id="current-${this._opts.id}" class="current-select">
        <input type="hidden" name="${this._opts.name}" value="${this._currentSelect.value}"/>
        <span class="item" value="${this._currentSelect.value}">${this._currentSelect.name}</span>
      </div>
      <div id="select-${this._opts.id}" class="select-items hide">${itemTemplate}</div>
    `;
      this._dom.appendChild(customSelect);

      if (opts.items.length) {
        this.items = opts.items;
      }
    }

    onChange (callback) {
      this._dom.addEventListener("click", (evt) => {
        const target = evt.target;
        this._direction = this._direction === 'fa-chevron-down' ? 'fa-chevron-up' : 'fa-chevron-down';
        document.querySelector(`#icon-${this._opts.id}`)
          .setAttribute("class", `fas ${this._direction}  select-chevron`);
        document.querySelector(`#select-${this._opts.id}`)
          .classList
          .toggle('hide');
        this._currentSelect = this.items.find(item => item.value === target.getAttribute("value"));
        if (this._currentSelect) {
          document.querySelector(`#current-${this._opts.id}`).innerHTML = `
          <input type="hidden" name="${this._opts.name}" value="${this._currentSelect.value}"/>
          <span class="item" value="${this._currentSelect.value}">${this._currentSelect.name}</span>
        `;
        }

        if (callback) {
          callback({
            current: this._currentSelect,
            evt
          });
        }
      }, true);
    }

    set items (newItems) {
      this._items = newItems;
      this._currentSelect = this._items[0];
      if (this._currentSelect) {
        document.querySelector(`#current-${this._opts.id}`).innerHTML = `
      <input type="hidden" name="${this._opts.name}" value="${this._currentSelect.value}"/>
        <span class="item" value="${this._currentSelect.value}">${this._currentSelect.name}</span>
      `;
        document.querySelector(`#select-${this._opts.id}`).innerHTML = this._items.map((item) => 
        `<span class="select-item" value="${item.value}">${item.name}</span>`
        ).join("");
      }
    }

    get items() {
      return this._items;
    }

  }

  window.addEventListener('DOMContentLoaded', () => {
    const microphone = new Select({
      id: "microphoneId",
      name: 'microphoneId',
      items: [],
      class: "custom-select",
    });
    const camera = new Select({
      id: "cameraId",
      name: 'cameraId',
      items: [],
      class: "custom-select"
    });
    RTCClient.getDevices().then(({videos, audios}) => {
      microphone.items = audios;
      camera.items = videos;
      microphone.onChange(() => {});
      camera.onChange(() => {});
    });
    document.querySelector("#create").addEventListener("click", (evt) => {
      evt.preventDefault();
      const data = checkForm("#form", ["channelName"]);
      if (!data.channelName) return;
      RTCClient.create({
        appID: data.appID, channelName: data.channelName,
        streamID: data.uid,
        uid: data.uid, resolution: "480p",
        codec: 'h264', mode: 'live',
        microphoneId: data.microphoneId,
        cameraId: data.cameraId,
        video: true,
        audio: true,
      }).then(_ => {
        Notice("创建房间成功");
        console.log("invoke create");
        RTCClient.rtc.on("error", (err) => {
          console.log(err);
        });
        RTCClient.rtc.on("peer-leave", (evt) => {
          const id = evt.uid;
          if (id !== data.uid) {
            RTCClient.removeView(id);
          }
          console.log('peer-leave', id);
        });
        RTCClient.rtc.on("stream-published", (evt) => {
          console.log("stream-published");
        });
        RTCClient.rtc.on("stream-added", (evt) => {
          window.remoteStream = evt.stream;
          const remoteStream = evt.stream;
          const id = remoteStream.getId();
          if (id !== data.uid) {
            RTCClient.subscribe(remoteStream).then(_ => {
              RTCClient.addView(id);
              remoteStream.play(`remote_video_${id}`, {fit: "cover", muted: true});
            });
          }
          console.log('stream-added', id);
        });
        RTCClient.rtc.on("stream-removed", (evt) => {
          const remoteStream = evt.stream;
          const id = remoteStream.getId();
          remoteStream.stop(`remote_video_${id}`);
          RTCClient.removeView(id);
          console.log('stream-removed', id);
        });
      });
    });

    document.querySelector("#join").addEventListener("click", (evt) => {
      evt.preventDefault();
      const data = checkForm("#form", ["channelName"]);
      if (!data.channelName) return;
      RTCClient.join().then(_ => {
        Notice("加入频道成功");
        document.querySelector("#local_video_info").innerHTML = `
        <span class="uid">uid: ${data.uid}</span>
      `;
        console.log("invoke join");
      }).catch(err => console.trace(err));
    });

    document.querySelector("#publish").addEventListener("click", (evt) => {
      const data = checkForm("#form", ["channelName"]);
      if (!data.channelName) return;
      RTCClient.publish().then(_ => {
        Notice("发布成功");
        console.log("invoke publish");
      }).catch(err => console.trace(err));
    });

    document.querySelector("#unpublish").addEventListener("click", (evt) => {
      const data = checkForm("#form", ["channelName"]);
      if (!data.channelName) return;
      RTCClient.unpublish().then(_ => {
        Notice("取消发布成功");
        console.log("invoke unpublish");
      }).catch(err => console.trace(err));
    });

    document.querySelector("#leave").addEventListener("click", (evt) => {
      const data = checkForm("#form", ["channelName"]);
      if (!data.channelName) return;
      RTCClient.leave().then(_ => {
        Notice("离开频道成功");
        document.querySelector("#local_video_info").innerHTML = ``;
        console.log("invoke leave");
      }).catch(err => console.trace(err));
    });
  });

}));
