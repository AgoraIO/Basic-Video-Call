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