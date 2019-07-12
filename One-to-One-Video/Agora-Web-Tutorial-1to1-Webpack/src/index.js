import RTCClient from './rtc-client';
import {getDevices, serializeFormData, validator, resolutions} from './common';
import "./assets/style.scss";
import * as bs from 'bootstrap-material-design';


$(() => {
  let selects = null;

  $('body').bootstrapMaterialDesign();
  $("#settings").on("click", function (e) {
    e.preventDefault();
    $("#settings").toggleClass("btn-raised");
    $('#setting-collapse').collapse();
  });

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
    resolutions.forEach(function (resolution) {
      $("<option/>", {
        value: resolution.value,
        text: resolution.name
      }).appendTo("#cameraResolution");
    });
  })

  const fields = ['appID', 'channel'];

  let rtc = new RTCClient();

  $("#check_quality").on("change", function () {
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
  })
})
