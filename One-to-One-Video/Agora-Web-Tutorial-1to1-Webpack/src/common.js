import AgoraRTC from 'agora-rtc-sdk';
import * as M from 'materialize-css';
import { replaceSearchString, stringify } from './searchParam'

export const resolutions = [
  {
    name: 'default',
    value: 'default',
  },
  {
    name: '480p',
    value: '480p',
  },
  {
    name: '720p',
    value: '720p',
  },
  {
    name: '1080p',
    value: '1080p'
  }
];

function Toastify (options) {
  M.toast({html: options.text, classes: options.classes});
}

export const Toast = {
  info: (msg) => {
    Toastify({
      text: msg,
      classes: "info-toast"
    })
  },
  notice: (msg) => {
    Toastify({
      text: msg,
      classes: "notice-toast"
    })
  },
  error: (msg) => {
    Toastify({
      text: msg,
      classes: "error-toast"
    })
  }
};

export function validator(formData, fields) {
  const keys = Object.keys(formData);
  for (let key of keys) {
    if (fields.indexOf(key) != -1) {
      if (!formData[key]) {
        Toast.error("Please Enter " + key);
        return false;
      }
    }
  }
  return true;
}

export function serializeFormData() {
  const formData = $("#form").serializeArray();
  const obj = {}
  for (var item of formData) {
    var key = item.name;
    var val = item.value;
    obj[key] = val;
  }
  replaceSearchString(stringify(obj))
  return obj;
}

export function addView (id, show) {
  if (!$("#" + id)[0]) {
    $("<div/>", {
      id: "remote_video_panel_" + id,
      class: "video-view",
    }).appendTo("#video");

    $("<div/>", {
      id: "remote_video_" + id,
      class: "video-placeholder",
    }).appendTo("#remote_video_panel_" + id);

    $("<div/>", {
      id: "remote_video_info_" + id,
      class: "video-profile " + (show ? "" :  "hide"),
    }).appendTo("#remote_video_panel_" + id);

    $("<div/>", {
      id: "video_autoplay_"+ id,
      class: "autoplay-fallback hide",
    }).appendTo("#remote_video_panel_" + id);
  }
}

export function removeView (id) {
  if ($("#remote_video_panel_" + id)[0]) {
    $("#remote_video_panel_"+id).remove();
  }
}

export function getDevices (next) {
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