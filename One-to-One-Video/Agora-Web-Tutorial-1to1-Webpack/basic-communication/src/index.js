import RTCClient from './rtc-client';
import $ from 'jquery';
import {getDevices, serializeFormData, validator} from './common';
import "./assets/style.scss";

$(() => {
  let selects = null;

  getDevices(function (devices) {
    selects = devices;
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
    selects.resolutions = [
      {
        value: "180p", name: "resolution: 320x180 15fps 140kbps"
      },
      {
        value: "360p", name: "resolution: 640x360 30fps 400kbps"
      },
      {
        value: "720p", name: "resolution: 1280x720 24fps 1130kbps"
      }
    ]
    selects.resolutions.forEach(function (resolution) {
      $('<option/>', {
        value: resolution.value,
        text: resolution.name,
      }).appendTo("#resolution");
    })
  })

  const fields = ['appID', 'channel', 'uid'];

  let rtc = new RTCClient();

  $("#create").on("click", function () {
    console.log("create")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.join(params);
    }
  })

  $("#publish").on("click", function () {
    console.log("startLiveStreaming")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.publish();
    }
  });

  $("#unpublish").on("click", function () {
    console.log("stopLiveStreaming")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.unpublish();
    }
  });

  $("#leave").on("click", function () {
    console.log("leave")
    const params = serializeFormData();
    if (validator(params, fields)) {
      rtc.leave();
    }
  })
})