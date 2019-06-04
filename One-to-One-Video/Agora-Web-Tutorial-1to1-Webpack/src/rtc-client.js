import AgoraRTC from 'agora-rtc-sdk';
import {Toast, addView, removeView} from './common';

console.log("agora sdk version: " + AgoraRTC.VERSION + " compatible: " + AgoraRTC.checkSystemRequirements());

export default class RTCClient {
  constructor () {
    this._client = null;
    this._joined = false;
    this._published = false;
    this._localStream = null;
    this._remoteStreams = [];
    this._params = {};
  }

  handleEvents() {
    this._client.on("error", (err) => {
      console.log(err)
    })
    // Occurs when the peer user leaves the channel; for example, the peer user calls Client.leave.
    this._client.on("peer-leave", (evt) => {
      const id = evt.uid;
      if (id != this._params.uid) {
        removeView(id);
      }
      Toast.notice("peer leave")
      console.log('peer-leave', id);
    })
    // Occurs when the local stream is _published.
    this._client.on("stream-published", (evt) => {
      Toast.notice("stream published success")
      console.log("stream-published");
    })
    // Occurs when the remote stream is added.
    this._client.on("stream-added", (evt) => {  
      const remoteStream = evt.stream;
      const id = remoteStream.getId();
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
      remoteStream.stop();
      this._remoteStreams = this._remoteStreams.filter((stream) => {
        return stream.getId() !== id
      })
      removeView(id);
      console.log('stream-removed remote-uid: ', id);
    })
    this._client.on("onTokenPrivilegeWillExpire", () => {
      // After requesting a new token
      // this._client.renewToken(token);
      Toast.info("onTokenPrivilegeWillExpire")
      console.log("onTokenPrivilegeWillExpire")
    });
    this._client.on("onTokenPrivilegeDidExpire", () => {
      // After requesting a new token
      // client.renewToken(token);
      Toast.info("onTokenPrivilegeDidExpire")
      console.log("onTokenPrivilegeDidExpire")
    })
  }

  join (data) {
    return new Promise((resolve, reject) => {    
      if (this._joined) {
        Toast.error("Your already joined");
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
    
        /**
         * Joins an AgoraRTC Channel
         * This method joins an AgoraRTC channel.
         * Parameters
         * tokenOrKey: string | null
         *    Low security requirements: Pass null as the parameter value.
         *    High security requirements: Pass the string of the Token or Channel Key as the parameter value. See Use Security Keys for details.
         *  channel: string
         *    A string that provides a unique channel name for the Agora session. The length must be within 64 bytes. Supported character scopes:
         *    26 lowercase English letters a-z
         *    26 uppercase English letters A-Z
         *    10 numbers 0-9
         *    Space
         *    "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", "{", "}", "|", "~", ","
         *  uid: number | string | null
         *    The user ID, an integer or a string, ASCII characters only. Ensure this ID is unique. If you set the uid to null, the server assigns one and returns it in the onSuccess callback.
         *   Note:
         *      All users in the same channel should have the same type (number or string) of uid.
         *      If you use a number as the user ID, it should be a 32-bit unsigned integer with a value ranging from 0 to (232-1).
         *      If you use a string as the user ID, the maximum length is 255 characters.
        **/
        this._client.join(data.token ? data.token : null, data.channel, data.uid ? data.uid : null, (uid) => {
          this._params.uid = uid;
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

  publish () {
    if (!this._client) {
      Toast.error("Please Join First");
      return;
    }
    if (this._published) {
      Toast.error("Your already published");
      return;
    }
    const oldState = this._published;
  
    // publish localStream
    this._client.publish(this._localStream, (err) => {
      this._published = oldState;
      console.log("publish failed");
      Toast.error("publish failed")
      console.error(err);
    })
    Toast.info("publish")
    this._published = true
  }

  unpublish () {
    if (!this._client) {
      Toast.error("Please Join First");
      return;
    }
    if (!this._published) {
      Toast.error("Your didn't publish");
      return;
    }
    const oldState = this._published;
    this._client.unpublish(this._localStream, (err) => {
      this._published = oldState;
      console.log("unpublish failed");
      Toast.error("unpublish failed")
      console.error(err);
    })
    Toast.info("unpublish")
    this._published = false;
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
}

