console.log("agora sdk version: " + AgoraRTC.VERSION + " compatible: " + AgoraRTC.checkSystemRequirements());

var Toast = {
  info: function (msg) {
    Toastify({
      text: msg,
      backgroundColor: "#3498db"
    }).showToast();
  },
  notice: function (msg) {
    Toastify({
      text: msg,
      backgroundColor: "#07bc0c"
    }).showToast();
  },
  error: function (msg) {
    Toastify({
      text: msg,
      backgroundColor: "#e74c3c"
    }).showToast();
  }
}

function validator(formData, fields) {
  var keys = Object.keys(formData);
  for (let key in keys) {
    var keyName = keys[key];
    if (fields.indexOf(keyName) != -1) {
      if (!formData[keyName]) {
        Toast.error("Please Enter " + keyName);
        return false;
      }
    }
  }
  return true;
}

function serializeFormData() {
  var formData = $("#form").serializeArray();
  let obj = {}
  for (var item in formData) {
    var key = formData[item].name;
    var val = formData[item].value;
    obj[key] = val;
  }
  return obj;
}

function addView (id) {
  if (!$("#" + id)[0]) {
    $("<div/>", {
      id: "remote_video_" + id,
      class: "video-view",
    }).appendTo("#video");
  }
}

function removeView (id) {
  if ($("#remote_video_" + id)[0]) {
    $("#remote_video_"+id).remove();
  }
}

function getDevices (next) {
  AgoraRTC.getDevices(function (items) {
    items.filter(function (item) {
      return ['audioinput', 'videoinput'].indexOf(item.kind) !== -1
    })
    .map(function (item) {
      return {
      name: item.label,
      value: item.deviceId,
      kind: item.kind,
      }
    });
    var videos = [];
    var audios = [];
    for (var i = 0; i < items.length; i++) {
      var item = items[i];
      if ('videoinput' == item.kind) {
        var name = item.label;
        var value = item.deviceId;
        if (!name) {
          name = "camera-" + videos.length;
        }
        videos.push({
          name: name,
          value: value,
          kidn: item.kind
        });
      }
      if ('audioinput' == item.kind) {
        var name = item.label;
        var value = item.deviceId;
        if (!name) {
          name = "microphone-" + audios.length;
        }
        audios.push({
          name: name,
          value: value,
          kidn: item.kind
        });
      }
    }
    next({videos: videos, audios: audios});
  });
}

var rtc = {
  client: null,
  joined: false,
  published: false,
  localStream: null,
  remoteStreams: [],
  params: {}
};

function handleEvents (rtc) {
  // Occurs when an error message is reported and requires error handling.
  rtc.client.on("error", (err) => {
    console.log(err)
  })
  // Occurs when the peer user leaves the channel; for example, the peer user calls Client.leave.
  rtc.client.on("peer-leave", function (evt) {
    var id = evt.uid;
    console.log("id", evt);
    if (id != rtc.params.uid) {
      removeView(id);
    }
    Toast.notice("peer leave")
    console.log('peer-leave', id);
  })
  // Occurs when the local stream is published.
  rtc.client.on("stream-published", function (evt) {
    Toast.notice("stream published success")
    console.log("stream-published");
  })
  // Occurs when the remote stream is added.
  rtc.client.on("stream-added", function (evt) {  
    var remoteStream = evt.stream;
    var id = remoteStream.getId();
    Toast.info("stream-added uid: " + id)
    if (id !== rtc.params.uid) {
      rtc.client.subscribe(remoteStream, function (err) {
        console.log("stream subscribe failed", err);
      })
    }
    console.log('stream-added remote-uid: ', id);
  });
  // Occurs when a user subscribes to a remote stream.
  rtc.client.on("stream-subscribed", function (evt) {
    var remoteStream = evt.stream;
    var id = remoteStream.getId();
    rtc.remoteStreams.push(remoteStream);
    addView(id);
    remoteStream.play("remote_video_" + id, {fit: "cover", muted: true});
    Toast.info('stream-subscribed remote-uid: ' + id);
    console.log('stream-subscribed remote-uid: ', id);
  })
  // Occurs when the remote stream is removed; for example, a peer user calls Client.unpublish.
  rtc.client.on("stream-removed", function (evt) {
    var remoteStream = evt.stream;
    var id = remoteStream.getId();
    Toast.info("stream-removed uid: " + id)
    remoteStream.stop("remote_video_" + id);
    rtc.remoteStreams = rtc.remoteStreams.filter(function (stream) {
      return stream.getId() !== id
    })
    removeView(id);
    console.log('stream-removed remote-uid: ', id);
  })
  rtc.client.on("onTokenPrivilegeWillExpire", function(){
    //After requesting a new token
    // rtc.client.renewToken(token);
    Toast.info("onTokenPrivilegeWillExpire")
    console.log("onTokenPrivilegeWillExpire")
  });
  rtc.client.on("onTokenPrivilegeDidExpire", function(){
    //After requesting a new token
    // client.renewToken(token);
    Toast.info("onTokenPrivilegeDidExpire")
    console.log("onTokenPrivilegeDidExpire")
  })
  // Occurs when the live streaming starts.
  rtc.client.on("liveStreamingStarted", function (evt) {
    Toast.info("liveStreamingStarted")
    console.log("liveStreamingStarted", evt)
  })
  // Occurs when the live streaming fails.
  rtc.client.on("liveStreamingFailed", function (evt) {
    Toast.error("liveStreamingFailed")
    console.log("liveStreamingFailed", evt)
  })
  // Occurs when the live streaming stops.
  rtc.client.on("liveStreamingStopped", function (evt) {
    Toast.info("liveStreamingStopped")
    console.log("liveStreamingStopped", evt)
  })
  // Occurs when the live transcoding setting is updated.
  rtc.client.on("liveTranscodingUpdated", function (evt) {
    Toast.info("liveTranscodingUpdated")
    console.log("liveTranscodingUpdated", evt)
  })
}

function join (rtc, data, next) {
  if (rtc.client) {
    Toast.error("Your already create client");
    return;
  }

  if (rtc.joined) {
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
  rtc.client = AgoraRTC.createClient({mode: 'rtc', codec: 'h264'});

  rtc.params = data;

  // handle AgoraRTC client event
  handleEvents(rtc);

  // init client
  rtc.client.init(data.appID, function () {
    console.log("init success");

    // join client
    rtc.client.join(data.token ? data.token : null, data.channel, data.uid, function (uid) {
      Toast.notice("join channel: " + data.channel + " success, uid: " + uid);
      console.log("join channel: " + data.channel + " success, uid: " + uid);
      rtc.joined = true;


      // create local stream
      rtc.localStream = AgoraRTC.createStream({
        streamID: data.uid,
        audio: true,
        video: true,
        screen: false,
        microphoneId: data.microphoneId,
        cameraId: data.cameraId
      })

      // init local stream
      rtc.localStream.init(function () {
        console.log("init local stream success");
        // play stream with html element id "local_stream"
        rtc.localStream.play("local_stream")

        // run callback
        if (next) {
          next(rtc)
        }
      }, function (err)  {
        console.error("init local stream failed ", err);
      })
    }, function(err) {
      console.error("client join failed", err)
    })
  }, (err) => {
    console.error(err);
  });
}

function publish (rtc) {
  if (!rtc.client) {
    Toast.error("Please Create Channel First");
    return;
  }
  if (rtc.published) {
    Toast.error("Your already published");
    return;
  }
  var oldState = rtc.published;

  // publish localStream
  rtc.client.publish(rtc.localStream, function (err) {
    rtc.published = oldState;
    console.log("publish failed");
    Toast.error("publish failed")
    console.error(err);
  })
  Toast.info("publish")
  rtc.published = true
}

function unpublish (rtc) {
  if (!rtc.client) {
    Toast.error("Please Create Channel First");
    return;
  }
  if (!rtc.published) {
    Toast.error("Your didn't unpublished");
    return;
  }
  var oldState = rtc.published;
  rtc.client.unpublish(rtc.localStream, function (err) {
    rtc.published = oldState;
    console.log("unpublish failed");
    Toast.error("unpublish failed")
    console.error(err);
  })
  Toast.info("unpublish")
  rtc.published = false;
}

function leave (rtc) {
  if (!rtc.client) {
    Toast.error("Please Join First!");
    return;
  }
  if (!rtc.joined) {
    Toast.error("You are not in channel");
    return;
  }
  // leave channel
  rtc.client.leave(function () {
    // close stream
    rtc.localStream.close();
    // stop stream
    rtc.localStream.stop();
    while (rtc.remoteStreams.length > 0) {
      var stream = rtc.remoteStreams.shift();
      var id = stream.getId()
      stream.stop();
      remoteView(id);
    }
    rtc.localStream = null;
    rtc.remoteStreams = [];
    rtc.client = null;
    console.log("client leaves channel success");
    rtc.published = false;
    rtc.joined = false;
    Toast.notice("leave success")
  }, function (err) {
    console.log("channel leave failed");
    Toast.error("leave success")
    console.error(err);
  })
}

function startLiveStreaming (rtc) {
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
  }
  var transcodingConfig = liveTranscoding[rtc.params.resolution];
  rtc.client.setLiveTranscoding(transcodingConfig);
  rtc.client.startLiveStreaming(rtc.params.url);
}

function stopLiveStreaming (rtc) {
  rtc.client.stopLiveStreaming(rtc.params.url);
}