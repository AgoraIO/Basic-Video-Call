import RTCClient from './rtc-client';
import {getDevices, serializeFormData, validator, resolutions} from './common';
import "./assets/style.scss";
import * as M from 'materialize-css';
import {setFormData, parseFromSearch} from './searchParam';

$(() => {  
  getDevices(function (devices) {
    devices.audios.forEach(function (audio) {
      $('<option/>', {
        value: audio.value,
        text: audio.name,
      }).appendTo("#microphoneId");
    })
    devices.videos.forEach(function (video) {
      $('<option/>', {
        value: video.value,
        text: video.name,
      }).appendTo("#cameraId");
    })
    resolutions.forEach(function (resolution) {
      $('<option/>', {
        value: resolution.value,
        text: resolution.name
      }).appendTo("#cameraResolution");
    })
    M.AutoInit();
  })

  const fields = ['appID', 'channel'];

  let rtc = new RTCClient();

  $(".autoplay-fallback").on("click", function (e) {
    e.preventDefault()
    const id = e.target.getAttribute("id").split("video_autoplay_")[1]
    console.log("autoplay fallback")
    if (id === 'local') {
      rtc._localStream.resume().then(() => {
        Toast.notice("local resume")
        $(e.target).addClass("hide")
      }).catch((err) => {
        Toast.error("resume failed, please open console see more details")
        console.error(err)
      })
      return;
    }
    const remoteStream = rtc._remoteStreams.find((item) => `${item.getId()}` == id)
    if (remoteStream) {
      remoteStream.resume().then(() => {
        Toast.notice("remote resume")
        $(e.target).addClass("hide")
      }).catch((err) => {
        Toast.error("resume failed, please open console see more details")
        console.error(err)
      })
    }
  })

  $("#show_profile").on("change", function (e) {
    e.preventDefault();
    rtc.setNetworkQualityAndStreamStats(this.checked);
  })

  $("#join").on("click", function (e) {
    e.preventDefault();
    console.log("join")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.join(params).then(() => {
        rtc.publish();
      })
    }
  })

  $("#publish").on("click", function (e) {
    e.preventDefault();
    console.log("startLiveStreaming")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.publish();
    }
  });

  $("#unpublish").on("click", function (e) {
    e.preventDefault();
    console.log("stopLiveStreaming")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.unpublish();
    }
  });

  $("#leave").on("click", function (e) {
    e.preventDefault();
    console.log("leave")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.leave();
    }
  });
})