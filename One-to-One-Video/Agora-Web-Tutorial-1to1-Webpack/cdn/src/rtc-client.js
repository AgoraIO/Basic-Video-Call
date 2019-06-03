import AgoraRTC from 'agora-rtc-sdk';
import {Toast, addView, removeView} from './common';

console.log("agora sdk version: " + AgoraRTC.VERSION + " compatible: " + AgoraRTC.checkSystemRequirements());

export default class RTCClient {
  constructor () {
    this._client = null;
    this._joined = false;
    this._published = false;
    this._localStream = null,
    this._remoteStreams = [];
    this._params = {};
  }

  handleEvents() {
    this._client.on("error", (err) => {
      console.log(err)
    })
    // Occurs when the peer user leaves the channel; for example, the peer user calls Client.leave.
    this._client.on("peer-leave", (evt) => {
      var id = evt.uid;
      if (id != this._params.uid) {
        removeView(id);
      }
      Toast.notice("peer leave")
      console.log('peer-leave', id);
    })
    // Occurs when the local stream is _published.
    this._client.on("stream-_published", (evt) => {
      Toast.notice("stream _published success")
      console.log("stream-_published");
    })
    // Occurs when the remote stream is added.
    this._client.on("stream-added", (evt) => {  
      var remoteStream = evt.stream;
      var id = remoteStream.getId();
      Toast.info("stream-added uid: " + id)
      if (id !== this._params.uid) {
        this._client.subscribe(remoteStream, (err) => {
          console.log("stream subscribe failed", err);
        })
      }
      console.log('stream-added remote-uid: ', id);
    });
    // Occurs when a user subscribes to a remote stream.
    this._client.on("stream-subscribed", (evt) => {
      const remoteStream = evt.stream;
      const id = remoteStream.getId();
      this._remoteStreams.push(remoteStream);
      addView(id);
      remoteStream.play("remote_video_" + id, {fit: "cover", muted: true});
      Toast.info('stream-subscribed remote-uid: ' + id);
      console.log('stream-subscribed remote-uid: ', id);
    })
    // Occurs when the remote stream is removed; for example, a peer user calls Client.unpublish.
    this._client.on("stream-removed", (evt) => {
      const remoteStream = evt.stream;
      const id = remoteStream.getId();
      Toast.info("stream-removed uid: " + id)
      remoteStream.stop("remote_video_" + id);
      this._remoteStreams = this._remoteStreams.filter((stream) => {
        return stream.getId() !== id
      })
      removeView(id);
      console.log('stream-removed remote-uid: ', id);
    })
    this._client.on("onTokenPrivilegeWillExpire", () => {
      //After requesting a new token
      // this._client.renewToken(token);
      Toast.info("onTokenPrivilegeWillExpire")
      console.log("onTokenPrivilegeWillExpire")
    });
    this._client.on("onTokenPrivilegeDidExpire", () => {
      //After requesting a new token
      // client.renewToken(token);
      Toast.info("onTokenPrivilegeDidExpire")
      console.log("onTokenPrivilegeDidExpire")
    })
    // Occurs when the live streaming starts.
    this._client.on("liveStreamingStarted", (evt) => {
      Toast.info("liveStreamingStarted")
      console.log("liveStreamingStarted", evt)
    })
    // Occurs when the live streaming fails.
    this._client.on("liveStreamingFailed", (evt) => {
      Toast.error("liveStreamingFailed")
      console.log("liveStreamingFailed", evt)
    })
    // Occurs when the live streaming stops.
    this._client.on("liveStreamingStopped", (evt) => {
      Toast.info("liveStreamingStopped")
      console.log("liveStreamingStopped", evt)
    })
    // Occurs when the live transcoding setting is updated.
    this._client.on("liveTranscodingUpdated", (evt) => {
      Toast.info("liveTranscodingUpdated")
      console.log("liveTranscodingUpdated", evt)
    })
  }

  join (data) {
    return new Promise((resolve, reject) => {
      if (this._client) {
        Toast.error("Your already create client");
        return;
      }
    
      if (this._joined) {
        Toast.error("Your already _joined");
        return;
      }
    
      /**
       * A class defining the properties of the config parameter in the createClient method.
       * Note:
       *    Ensure that you do not leave mode and codec as empty.
       *    Ensure that you set these properties before calling Client.join.
       *  You could find more detail here. https://docs.agora.io/en/Video/API%20Reference/web/interfaces/agorartc.clientconfig.html
      **/
      this._client = AgoraRTC.createClient({mode: 'rtc', codec: 'h264'});
    
      this._params = data;
    
      // handle AgoraRTC client event
      this.handleEvents();
    
      // init client
      this._client.init(data.appID, () => {
        console.log("init success");
    
        // join client
        this._client.join(data.token ? data.token : null, data.channel, data.uid, (uid) => {
          Toast.notice("join channel: " + data.channel + " success, uid: " + uid);
          console.log("join channel: " + data.channel + " success, uid: " + uid);
          this._joined = true;
    
    
          // create local stream
          this._localStream = AgoraRTC.createStream({
            streamID: data.uid,
            audio: true,
            video: true,
            screen: false,
            microphoneId: data.microphoneId,
            cameraId: data.cameraId
          })
    
          // init local stream
          this._localStream.init(() => {
            console.log("init local stream success");
            // play stream with html element id "local_stream"
            this._localStream.play("local_stream")
    
            // run callback
            resolve();
          }, (err) =>  {
            console.error("init local stream failed ", err);
          })
        }, function(err) {
          console.error("client join failed", err)
        })
      }, (err) => {
        console.error(err);
      });
    })
  }
  
  leave () {
    if (!this._client) {
      Toast.error("Please Join First!");
      return;
    }
    if (!this._joined) {
      Toast.error("You are not in channel");
      return;
    }
    // leave channel
    this._client.leave(() => {
      // close stream
      this._localStream.close();
      // stop stream
      this._localStream.stop();
      while (this._remoteStreams.length > 0) {
        const stream = this._remoteStreams.shift();
        const id = stream.getId()
        stream.stop();
        removeView(id);
      }
      this._localStream = null;
      this._remoteStreams = [];
      this._client = null;
      console.log("client leaves channel success");
      this._published = false;
      this._joined = false;
      Toast.notice("leave success")
    }, (err) => {
      console.log("channel leave failed");
      Toast.error("leave success")
      console.error(err);
    })
  }
  
  startLiveStreaming () {
    const uid = this._params.uid;
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
          uid,
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
          uid,
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
          uid,
          alpha: 1,
          width: 10,
          height: 10,
          zOrder: 1,
          x: 10,
          y: 10
        }],
      }
    }
    const transcodingConfig = liveTranscoding[this._params.resolution];
    this._client.setLiveTranscoding(transcodingConfig);
    this._client.startLiveStreaming(this._params.url);
  }
    
  stopLiveStreaming () {
    this._client.stopLiveStreaming(this._params.url);
  }
}

