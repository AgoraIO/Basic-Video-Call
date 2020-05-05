import "bulma";
import $ from "jquery";
import "bulma-checkradio/dist/bulma-checkradio.min.css";
import "@/assets/css/icons.css";
import * as Cookies from "js-cookie";

import "@/assets/global.scss";
import "./index.scss";
import { RESOLUTION_ARR } from "@/utils/Settings";
// eslint-disable-next-line
import Polyfill from "@/utils/Polyfill";
import Validator from "@/utils/Validate";

// Parses the query params from the url

const getParameterByName = (name, url) => {
  if (!url) {
    url = window.location.href;
  }
  name = name.replace(/[[\]]/g, "\\$&");
  let regex = new RegExp("[?&]" + name + "(=([^&#]*)|&|#|$)");
  let results = regex.exec(url);
  if (!results) return null;
  if (!results[2]) return "";
  return decodeURIComponent(results[2].replace(/\+/g, " "));
};


// Intializes the ui with necessary video profile options. (resolution, framerate, bitrate etc.)
const uiInit = () => {
  document.querySelector(
    ".login-title"
  ).innerHTML = `AgoraWeb v${agoraVersion.slice(1)}`;
  let profileContainer = $("#videoProfile");
  let profileLowContainer = $("#videoProfileLow");
  let lowResolutionArr = Object.entries(RESOLUTION_ARR).slice(0, 7);
  Object.entries(RESOLUTION_ARR).map(item => {
    let html =
      "<option " +
      (item[0] === "480p_4" ? "selected" : "") +
      ' value="' +
      item[0] +
      '">' +
      item[1][0] +
      "x" +
      item[1][1] +
      ", " +
      item[1][2] +
      "fps, " +
      item[1][3] +
      "kbps</option>";
    return profileContainer.append(html);
  });
  lowResolutionArr.map(item => {
    let html =
      "<option " +
      (item[0] === "180p_4" ? "selected" : "") +
      ' value="' +
      item[0] +
      '">' +
      item[1][0] +
      "x" +
      item[1][1] +
      ", " +
      item[1][2] +
      "fps, " +
      item[1][3] +
      "kbps</option>";
    return profileLowContainer.append(html);
  });
  let audienceOnly = getParameterByName("audienceOnly") === "true";
  if (audienceOnly) {
    $("#attendeeMode label.audience")
      .siblings()
      .hide();
    $("#attendeeMode label.audience input").prop("checked", true);
  }
};

// Channel name validator

const validate = channelName => {
  if (Validator.isNonEmpty(channelName)) {
    return "Cannot be empty!";
  }
  if (Validator.minLength(channelName, 1)) {
    return "No shorter than 1!";
  }
  if (Validator.maxLength(channelName, 16)) {
    return "No longer than 16!";
  }
  if (Validator.validChar(channelName)) {
    return 'Only capital or lower-case letter, number and "_" are permitted!';
  }

  return "";
};



const subscribeMouseEvent = () => {
  // Click Join and go to the meeting room
  $("#joinBtn").on("click", () => {
    let validateRst = validate(
      $("#channel")
        .val()
        .trim()
    );
    let validateIcon = $(".validate-icon");
    validateIcon.empty();
    if (validateRst) {
      let msg = `Input Error: ${validateRst}`;
      $("#channel").addClass("is-danger");
      validateIcon.append(`<i class="ag-icon icon-wrong"></i>`);
      $(".validate-msg")
        .html(msg)
        .show();
      return 0;
    }
    $("#channel").addClass("is-success");
    validateIcon.append(`<i class="ag-icon icon-correct"></i>`);

    // This config data is added to the cookies to be used in other pages.
    let postData = {
      baseMode: document.querySelector("#baseMode").dataset.value,
      transcode: $('input[name="transcode"]:checked').val(),
      attendeeMode: $('input[name="attendee"]:checked').val(),
      videoProfile: $("#videoProfile").val(),
      channel: $("#channel")
        .val()
        .trim(),
      videoProfileLow: $("#videoProfileLow").val()
    };
    Object.entries(postData).map(item => {
      return Cookies.set(item[0], item[1]);
    });
    window.location.href = "precall.html";
  });
  // Press Enter to trigger Join
  window.addEventListener("keypress", e => {
    e.keyCode === 13 && $("#joinBtn").trigger("click");
  });
  // Add input check for room name
  $("#channel").on("input", () => {
    $("#channel").removeClass("is-danger");
    $("#channel").removeClass("is-success");
    $(".validate-msg").hide();
    let validateRst = validate(
      $("#channel")
        .val()
        .trim()
    );
    let validateIcon = $(".validate-icon");
    validateIcon.empty();
    if (validateRst) {
      let msg = `Input Error: ${validateRst}`;
      $("#channel").addClass("is-danger");
      validateIcon.append(`<i class="ag-icon icon-wrong"></i>`);
      $(".validate-msg")
        .html(msg)
        .show();
      return 0;
    }
    $("#channel").addClass("is-success");
    validateIcon.append(`<i class="ag-icon icon-correct"></i>`);
  });
  // BaseMode dropdown control
  $("#baseMode").click(e => {
    e.stopPropagation();
    $(".dropdown").removeClass("is-active");
    $("#baseModeDropdown").toggleClass("is-active");
  });

  $("#baseModeOptions .dropdown-item").click(e => {
    e.stopPropagation();
    let [val, label] = [
      e.currentTarget.dataset.value,
      e.currentTarget.dataset.msg
    ];
    $("#baseMode").data("value", val);
    $("#baseModeDropdown").removeClass("is-active");
    $("#baseModeLabel").html(label);
  });
  // AdvancedProfile dropdown control
  $("#advancedProfile").click(e => {
    e.stopPropagation();
    $(".dropdown").removeClass("is-active");
    $("#advanceProfileDropdown").toggleClass("is-active");
  });
  $("#advancedOptions").click(e => {
    e.stopPropagation();
  });
  // global click will close dropdown
  $(window).click(_ => {
    $(".dropdown").removeClass("is-active");
  });
};

// ----------------start-------------------
// ----------------------------------------

uiInit();
subscribeMouseEvent();
