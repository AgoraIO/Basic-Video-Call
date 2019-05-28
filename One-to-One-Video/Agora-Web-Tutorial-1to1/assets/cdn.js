(function (global, factory) {
  typeof exports === 'object' && typeof module !== 'undefined' ? factory(require('agora-rtc-sdk'), require('eventemitter3'), require('formdata-polyfill')) :
  typeof define === 'function' && define.amd ? define(['agora-rtc-sdk', 'eventemitter3', 'formdata-polyfill'], factory) :
  (global = global || self, factory(global.AgoraRTC, global.EventEmitter3, global.formData));
}(this, function (AgoraRTC, EventEmitter3, formData) { 'use strict';

  AgoraRTC = AgoraRTC && AgoraRTC.hasOwnProperty('default') ? AgoraRTC['default'] : AgoraRTC;
  EventEmitter3 = EventEmitter3 && EventEmitter3.hasOwnProperty('default') ? EventEmitter3['default'] : EventEmitter3;

  /*! *****************************************************************************
  Copyright (c) Microsoft Corporation. All rights reserved.
  Licensed under the Apache License, Version 2.0 (the "License"); you may not use
  this file except in compliance with the License. You may obtain a copy of the
  License at http://www.apache.org/licenses/LICENSE-2.0

  THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
  WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
  MERCHANTABLITY OR NON-INFRINGEMENT.

  See the Apache Version 2.0 License for specific language governing permissions
  and limitations under the License.
  ***************************************************************************** */

  function __awaiter(thisArg, _arguments, P, generator) {
      return new (P || (P = Promise))(function (resolve, reject) {
          function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
          function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
          function step(result) { result.done ? resolve(result.value) : new P(function (resolve) { resolve(result.value); }).then(fulfilled, rejected); }
          step((generator = generator.apply(thisArg, _arguments || [])).next());
      });
  }

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
      static getDevices() {
          return __awaiter(this, void 0, void 0, function* () {
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
          });
      }
      static create(options) {
          return __awaiter(this, void 0, void 0, function* () {
              if (!this._rtc) {
                  const rtc = new RTCClient(options);
                  yield rtc.init();
                  this._rtc = rtc;
                  console.log("[rtc-client create]");
              }
          });
      }
      static join() {
          return __awaiter(this, void 0, void 0, function* () {
              if (!this._rtc)
                  return;
              if (!this._rtc._state.joined) {
                  yield this._rtc.join();
                  yield this._rtc.createStream(this._rtc._options);
                  yield this._rtc.play("local_stream");
              }
          });
      }
      static publish() {
          return __awaiter(this, void 0, void 0, function* () {
              if (!this._rtc)
                  return;
              if (this._rtc._state.published === false) {
                  yield this._rtc.setRole('host');
                  return this._rtc.publish();
              }
          });
      }
      static unpublish() {
          return __awaiter(this, void 0, void 0, function* () {
              if (!this._rtc)
                  return;
              if (this._rtc._state.published === true) {
                  return this._rtc.unpublish();
              }
          });
      }
      static leave() {
          return __awaiter(this, void 0, void 0, function* () {
              if (!this._rtc)
                  return;
              yield this.unpublish();
              yield this._rtc.stopLocalStream();
              yield this._rtc.leave();
              delete this._rtc;
          });
      }
      static subscribe(stream) {
          return __awaiter(this, void 0, void 0, function* () {
              if (!this._rtc)
                  return;
              return this._rtc.subscribe(stream);
          });
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
      subscribe(stream) {
          return __awaiter(this, void 0, void 0, function* () {
              return this._client.subscribe(stream, (err) => {
                  this._dispatch.emit("subscribeFailed", err);
              });
          });
      }
      play(id) {
          return __awaiter(this, void 0, void 0, function* () {
              return this._localStream.play(id, { fit: "cover" });
          });
      }
      stopLocalStream() {
          return __awaiter(this, void 0, void 0, function* () {
              this._localStream.close();
              return this._localStream.stop();
          });
      }
      publish() {
          return __awaiter(this, void 0, void 0, function* () {
              this._state.published = true;
              return this._client.publish(this._localStream, (err) => {
                  this._dispatch.emit("publishFailed", err);
              });
          });
      }
      unpublish() {
          return __awaiter(this, void 0, void 0, function* () {
              this._state.published = false;
              return this._client.unpublish(this._localStream, (err) => {
                  this._dispatch.emit("unpublishFailed", err);
              });
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

  const valid = (data, fields) => {
    for (let key of Object.keys(data)) {
      if (fields.indexOf(key) != -1) {
        if ((typeof data[key] == 'string' && !data[key])
          || null === data[key]
          || undefined === data[key]) {
          return false
        }
      }
    }
    return true;
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
    window.AgoraRTC = AgoraRTC;
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
    const resolution = new Select({
      id: "resolution",
      name: "resolution",
      items: [
        {
          value: "180p", name: "resolution: 320x180 15fps 140kbps"
        },
        {
          value: "360p", name: "resolution: 640x360 30fps 400kbps"
        },
        {
          value: "720p", name: "resolution: 1280x720 24fps 1130kbps"
        },
      ],
      class: 'custom-select'
    });
    resolution.onChange((evt) => {
      console.log("resolution", evt.current.value);
    });
    RTCClient.getDevices().then(({videos, audios}) => {
      microphone.items = audios;
      camera.items = videos;
      microphone.onChange(() => {});
      camera.onChange(() => {});
    });

    const requireFields = [
      "channelName",
      "appID",
      "transcodingUrl"
    ];

    document.querySelector("#create").addEventListener("click", (evt) => {
      const data = checkForm("#form", requireFields);
      console.log("data", valid(data, requireFields));
      if (!valid(data, requireFields)) return

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
        window.RTCClient = RTCClient;
        RTCClient.rtc.on("error", (err) => {
          console.log("errror", err);
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
        RTCClient.rtc.on("liveStreamingStarted", (evt) => {
          console.log("[cdn] liveStreamingStarted", evt);
        });
        RTCClient.rtc.on("liveStreamingFailed", (evt) => {
          console.log("[cdn] liveStreamingFailed", evt);
        });
        RTCClient.rtc.on("liveStreamingStopped", (evt) => {
          console.log("[cdn] liveStreamingStopped", evt);
        });
        RTCClient.rtc.on("liveTranscodingUpdated", (evt) => {
          console.log("[cdn] liveTranscodingUpdated", evt);
        });
      }).catch(err => {
        console.log(err);
      });
    });

    document.querySelector("#join").addEventListener("click", (evt) => {
      const data = checkForm("#form", requireFields);
      if (!valid(data, requireFields)) return

      RTCClient.join().then(_ => {
        return RTCClient.publish().then(_ => {
          Notice("加入频道成功");
          document.querySelector("#local_video_info").innerHTML = `
          <span class="uid">${data.uid}</span>
        `;
        })
      }).catch(err => {

        console.error(err);
      });
    });

    document.querySelector("#startLiveStreaming").addEventListener("click", (evt) => {
      const data = checkForm("#form", requireFields);
      if (!valid(data, requireFields)) return
      const liveTranscoding = {
        "180p": {
          width: 320,
          height: 180,
          videoBitrate: 140,
          videoFramerate: 15,
          lowLatency: false,
          audioSampleRate: AgoraRTC.AUDIO_SAMPLE_RATE_48000,
          audioBitrate: 48,
          audioChannels: 1,
          videoGop: 30,
          videoCodecProfile: AgoraRTC.VIDEO_CODEC_PROFILE_HIGH,
          userCount: 1,
          backgroundColor: 0x000000,
          transcodingUsers: [{
            uid: 1,
            alpha: 1,
            width: 10,
            height: 10,
            zOrder: 1,
            x: 10,
            y: 10
          }],
        },
        "360p": {
          width: 640,
          height: 360,
          videoBitrate: 400,
          videoFramerate: 30,
          lowLatency: false,
          audioSampleRate: AgoraRTC.AUDIO_SAMPLE_RATE_48000,
          audioBitrate: 48,
          audioChannels: 1,
          videoGop: 30,
          videoCodecProfile: AgoraRTC.VIDEO_CODEC_PROFILE_HIGH,
          userCount: 1,
          backgroundColor: 0x000000,
          transcodingUsers: [{
            uid: 1,
            alpha: 1,
            width: 10,
            height: 10,
            zOrder: 1,
            x: 10,
            y: 10
          }],
        },
        "720p": {
          width: 1280,
          height: 720,
          videoBitrate: 1130,
          videoFramerate: 24,
          lowLatency: false,
          audioSampleRate: AgoraRTC.AUDIO_SAMPLE_RATE_48000,
          audioBitrate: 48,
          audioChannels: 1,
          videoGop: 30,
          videoCodecProfile: AgoraRTC.VIDEO_CODEC_PROFILE_HIGH,
          userCount: 1,
          backgroundColor: 0x000000,
          transcodingUsers: [{
            uid: 1,
            alpha: 1,
            width: 10,
            height: 10,
            zOrder: 1,
            x: 10,
            y: 10
          }],
        }
      };
      const transcodingConfig = liveTranscoding[data.resolution];
      RTCClient.startLiveStreaming(data.transcodingUrl, transcodingConfig);
      Notice("CDN推流成功");
      console.log("invoke startLiveStreaming");
    });

    document.querySelector("#stopLiveStreaming").addEventListener("click", (evt) => {
      const data = checkForm("#form", requireFields);
      if (!valid(data, requireFields)) return
      RTCClient.stopLiveStreaming(data.transcodingUrl);
      Notice("CDN停止推流成功");
      console.log("invoke stopLiveStreaming");
    });

    document.querySelector("#leave").addEventListener("click", (evt) => {
      const data = checkForm("#form", requireFields);
      if (!valid(data, requireFields)) return

      RTCClient.leave().then(_ => {
        Notice("离开频道成功");
        console.log("invoke leave");
      }).catch(err => console.trace(err));
    });
  });

}));
