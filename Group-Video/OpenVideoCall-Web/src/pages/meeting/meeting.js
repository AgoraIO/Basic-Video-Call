import AgoraRTC from "agora-rtc-sdk";
import "bulma";
import $ from "jquery";
import * as Cookies from "js-cookie";
import { merge } from "lodash";
import "@/assets/css/icons.css";

import "@/assets/global.scss";
import "./meeting.scss";
import ButtonControl from "@/utils/ButtonControl";
import {
  isSafari,
  isMobileSize,
  isChrome,
  isFirefox
} from "@/utils/BrowserCheck";
import Notify from "@/utils/Notify";
import Renderer from "@/utils/Render";
import { SHARE_ID, RESOLUTION_ARR, APP_ID, Token } from "@/utils/Settings";
import { logger, log } from "../../utils/Logger";
// eslint-disable-next-line
import Polyfill from "@/utils/Polyfill";

// If display a window to show video info
const DUAL_STREAM_DEBUG = false;
let options = {};
let client = {};
let localStream = {};
let streamList = [];
let shareClient = null;
let shareStream = null;
let mainId;
let mainStream;

const globalLog = logger.init("global", "blue");
const shareLog = logger.init("share", "yellow");
const localLog = logger.init("local", "green");

// Initialize the options from cookies (set from index.js and precall.js)

const optionsInit = () => {
  let options = {
    videoProfile: Cookies.get("videoProfile").split(",")[0] || "480p_4",
    videoProfileLow: Cookies.get("videoProfileLow"),
    cameraId: Cookies.get("cameraId"),
    microphoneId: Cookies.get("microphoneId"),
    channel: Cookies.get("channel") || "test",
    transcode: Cookies.get("transcode") || "interop",
    attendeeMode: Cookies.get("attendeeMode") || "video",
    baseMode: Cookies.get("baseMode") || "avc",
    displayMode: 1, // 0 Tile, 1 PIP, 2 screen share
    uid: undefined, // In default it is dynamically generated
    resolution: undefined
  };

  let tempProfile = RESOLUTION_ARR[Cookies.get("videoProfile")];
  options.resolution = tempProfile[0] / tempProfile[1] || 4 / 3;

  options.key = APP_ID;
  options.token = Token;

  return options;
};

// Initalizes the User Interface

const uiInit = options => {
  document.querySelector(
    ".ag-header-lead span"
  ).innerHTML = `AgoraWeb v${agoraVersion.slice(1)}`;
  Renderer.init("ag-canvas", 9 / 16, 8 / 5);
  // Mobile page should remove title and footer
  if (isMobileSize()) {
    Renderer.enterFullScreen();
  }
  // Only firefox and chrome support screen sharing
  if (!isFirefox() && !isChrome()) {
    ButtonControl.disable(".shareScreenBtn");
  }

  $("#room-name").html(options.channel);
  switch (options.attendeeMode) {
    case "audio-only":
      ButtonControl.hide([".videoControlBtn", ".shareScreenBtn"]);
      break;
    case "audience":
      ButtonControl.hide([
        ".videoControlBtn",
        ".audioControlBtn",
        ".shareScreenBtn"
      ]);
      break;
    default:
    case "video":
      break;
  }
};

// Intializes the Agora client object

const clientInit = (client, options) => {
  return new Promise((resolve, reject) => {
    // Initialize the agora client object with appid
    client.init(options.key, () => {
      globalLog("AgoraRTC client initialized");
      // Set low stream parameter
      // Read more here https://docs.agora.io/en/Video/API%20Reference/web/interfaces/agorartc.client.html#setlowstreamparameter
      let lowStreamParam = RESOLUTION_ARR[options.videoProfileLow];
      // Join the channel
      client.join(
        options.token,
        options.channel,
        options.uid,
        uid => {
          log(uid, "brown", `User ${uid} join channel successfully`);
          log(uid, "brown", new Date().toLocaleTimeString());
          client.setLowStreamParameter({
            width: lowStreamParam[0],
            height: lowStreamParam[1],
            framerate: lowStreamParam[2],
            bitrate: lowStreamParam[3]
          });
          // Create localstream
          resolve(uid);
        },
        err => {
          reject(err);
        }
      );
    });
  });
};

/**
 * @description Initializes the local stream
 * @param {*} uid User Id
 * @param {*} options global option
 * @param {*} config stream config
 */
const streamInit = (uid, options, config) => {
  let defaultConfig = {
    streamID: uid,
    audio: true,
    video: true,
    screen: false
  };

  // Modify default config based on attendee mode
  switch (options.attendeeMode) {
    case "audio-only":
      defaultConfig.video = false;
      break;
    case "audience":
      defaultConfig.video = false;
      defaultConfig.audio = false;
      break;
    default:
    case "video":
      break;
  }
  // Create an Agora stream using the above parameters
  // eslint-disable-next-line
  let stream = AgoraRTC.createStream(merge(defaultConfig, config));
  // Set the selected video profile
  stream.setVideoProfile(options.videoProfile);
  return stream;
};

// A function to stop and reset screen sharing related features.
const shareEnd = () => {
  try {
    shareClient && shareClient.unpublish(shareStream);
    shareStream && shareStream.close();
    shareClient &&
      shareClient.leave(
        () => {
          shareLog("Share client succeed to leave.");
        },
        () => {
          shareLog("Share client failed to leave.");
        }
      );
  } finally {
    shareClient = null;
    shareStream = null;
  }
};


// Start screen sharing
const shareStart = () => {
  ButtonControl.disable(".shareScreenBtn");

  // Create a new client for the screen sharing stream.

  // eslint-disable-next-line
  shareClient = AgoraRTC.createClient({
    mode: options.transcode
  });

  // Create a new options object for screen sharing.
  let shareOptions = merge(options, {
    uid: SHARE_ID
  });

  // Initalializes the client with the screen sharing options.
  clientInit(shareClient, shareOptions).then(uid => {
    // New config for screen sharing stream
    let config = {
      screen: true,
      video: false,
      audio: false,
      extensionId: "minllpmhdgpndnkomcoccfekfegnlikg",
      mediaSource: "application"
    };
    // Intialize the screen sharing stream with the necessary parameters.
    shareStream = streamInit(uid, shareOptions, config);
    shareStream.init(
      () => {
        // Once the stream is intialized, update the relevant ui and publish the stream.
        ButtonControl.enable(".shareScreenBtn");
        shareStream.on("stopScreenSharing", () => {
          shareEnd();
          shareLog("Stop Screen Sharing at" + new Date());
        });
        shareClient.publish(shareStream, err => {
          shareLog("Publish share stream error: " + err);
          shareLog("getUserMedia failed", err);
        });
      },
      err => {
        // Appropriate error handling if screen sharing doesn't work.
        ButtonControl.enable(".shareScreenBtn");
        shareLog("getUserMedia failed", err);
        shareEnd();
        if (isChrome()) {
          // If (!chrome.app.isInstalled) {
          let msg = `
            Please install chrome extension from 
            <a href="https://chrome.google.com/webstore/detail/minllpmhdgpndnkomcoccfekfegnlikg">Google Webstore</a> 
            before using sharing screen.   
          `;
          Notify.danger(msg, 5000);
          // }
        }
      }
    );
  });
};

// Relevant code if you are using the screen sharing extension for previous versions of chrome.
window.installSuccess = (...args) => {
  globalLog(...args);
};

window.installError = (...args) => {
  globalLog(...args);
  Notify.danger(
    "Failed to install the extension, please check the network and console.",
    3000
  );
};

// Helper function to remove stream from UI
const removeStream = id => {
  streamList.map((item, index) => {
    if (item.getId() === id) {
      streamList[index].close();
      $("#video-item-" + id).remove();
      streamList.splice(index, 1);
      return 1;
    }
    return 0;
  });
  if (streamList.length <= 4 && options.displayMode !== 2) {
    ButtonControl.enable(".displayModeBtn");
  }
  Renderer.customRender(streamList, options.displayMode, mainId);
};

// Helper function to add stream to UI
const addStream = (stream, push = false) => {
  let id = stream.getId();
  // Check for redundant
  let redundant = streamList.some(item => {
    return item.getId() === id;
  });
  if (redundant) {
    return;
  }
  // Do push for localStream and unshift for other streams
  push ? streamList.push(stream) : streamList.unshift(stream);
  if (streamList.length > 4) {
    options.displayMode = options.displayMode === 1 ? 0 : options.displayMode;
    ButtonControl.disable([".displayModeBtn", ".disableRemoteBtn"]);
  }
  Renderer.customRender(streamList, options.displayMode, mainId);
};

// Helper function to fetch video streams from the data structure
const getStreamById = id => {
  return streamList.filter(item => {
    return item.getId() === id;
  })[0];
};

// enable dual stream
const enableDualStream = () => {
  client.enableDualStream(
    function() {
      localLog("Enable dual stream success!");
    },
    function(e) {
      localLog(e);
    }
  );
};

// for setting the high stream in dual stream configuration.
const setHighStream = (prev, next) => {
  if (prev === next) {
    return;
  }
  let prevStream;
  let nextStream;
  // Get stream by id
  for (let stream of streamList) {
    let id = stream.getId();
    if (id === prev) {
      prevStream = stream;
    } else if (id === next) {
      nextStream = stream;
    } else {
      // Do nothing
    }
  }
  // Set prev stream to low
  prevStream && client.setRemoteVideoStreamType(prevStream, 1);
  // Set next stream to high
  nextStream && client.setRemoteVideoStreamType(nextStream, 0);
};

/**
 * Add callback for client event to control streams
 * @param {*} client
 * @param {*} streamList
 */
const subscribeStreamEvents = () => {
  client.on("stream-added", function(evt) {
    let stream = evt.stream;
    let id = stream.getId();
    localLog("New stream added: " + id);
    localLog(new Date().toLocaleTimeString());
    localLog("Subscribe ", stream);
    if (id === SHARE_ID) {
      options.displayMode = 2;
      mainId = id;
      mainStream = stream;
      if (!shareClient) {
        ButtonControl.disable(".shareScreenBtn");
      }
      ButtonControl.disable([".displayModeBtn", ".disableRemoteBtn"]);
    }
    if (id !== mainId) {
      if (options.displayMode === 2) {
        client.setRemoteVideoStreamType(stream, 1);
      } else {
        mainStream && client.setRemoteVideoStreamType(mainStream, 1);
        mainStream = stream;
        mainId = id;
      }
    }
    client.subscribe(stream, function(err) {
      localLog("Subscribe stream failed", err);
    });
  });

  // When a peer leaves
  client.on("peer-leave", function(evt) {
    let id = evt.uid;
    localLog("Peer has left: " + id);
    localLog(new Date().toLocaleTimeString());
    if (id === SHARE_ID) {
      options.displayMode = 0;
      if (options.attendeeMode === "video") {
        ButtonControl.enable(".shareScreenBtn");
      }
      ButtonControl.enable([".displayModeBtn", ".disableRemoteBtn"]);
      shareEnd();
    }
    if (id === mainId) {
      let next = options.displayMode === 2 ? SHARE_ID : localStream.getId();
      setHighStream(mainId, next);
      mainId = next;
      mainStream = getStreamById(mainId);
    }
    removeStream(evt.uid);
  });

  // when a stream is subscribed
  client.on("stream-subscribed", function(evt) {
    let stream = evt.stream;
    localLog("Got stream-subscribed event");
    localLog(new Date().toLocaleTimeString());
    localLog("Subscribe remote stream successfully: " + stream.getId());
    addStream(stream);
  });

  // when a stream is removed
  client.on("stream-removed", function(evt) {
    let stream = evt.stream;
    let id = stream.getId();
    localLog("Stream removed: " + id);
    localLog(new Date().toLocaleTimeString());
    if (id === SHARE_ID) {
      options.displayMode = 0;
      if (options.attendeeMode === "video") {
        ButtonControl.enable(".shareScreenBtn");
      }
      ButtonControl.enable([".displayModeBtn", ".disableRemoteBtn"]);
      shareEnd();
    }
    if (id === mainId) {
      let next = options.displayMode === 2 ? SHARE_ID : localStream.getId();
      setHighStream(mainId, next);
      mainId = next;
      mainStream = getStreamById(mainId);
    }
    removeStream(stream.getId());
  });
};

// Subscribe the various mouse events in the UI like clicks
const subscribeMouseEvents = () => {
  $(".displayModeBtn").on("click", function(e) {
    if (
      e.currentTarget.classList.contains("disabled") ||
      streamList.length <= 1
    ) {
      return;
    }
    // 1 refer to pip mode
    if (options.displayMode === 1) {
      options.displayMode = 0;
      ButtonControl.disable(".disableRemoteBtn");
    } else if (options.displayMode === 0) {
      options.displayMode = 1;
      ButtonControl.enable(".disableRemoteBtn");
    } else {
      // Do nothing when in screen share mode
    }
    Renderer.customRender(streamList, options.displayMode, mainId);
  });

  $(".exitBtn").on("click", function() {
    try {
      shareClient && shareEnd();
      client && client.unpublish(localStream);
      localStream && localStream.close();
      client &&
        client.leave(
          () => {
            localLog("Client succeed to leave.");
          },
          () => {
            localLog("Client failed to leave.");
          }
        );
    } finally {
      // Redirect to index
      window.location.href = "index.html";
    }
  });

  $(".videoControlBtn").on("click", function() {
    $(".videoControlBtn").toggleClass("off");
    localStream.isVideoOn()
      ? localStream.disableVideo()
      : localStream.enableVideo();
  });

  $(".audioControlBtn").on("click", function() {
    $(".audioControlBtn").toggleClass("off");
    localStream.isAudioOn()
      ? localStream.disableAudio()
      : localStream.enableAudio();
  });

  $(".shareScreenBtn").on("click", function(e) {
    if (e.currentTarget.classList.contains("disabled")) {
      return;
    }
    if (shareClient) {
      shareEnd();
    } else {
      shareStart();
    }
  });

  $(".disableRemoteBtn").on("click", function(e) {
    if (
      e.currentTarget.classList.contains("disabled") ||
      streamList.length <= 1
    ) {
      return;
    }
    $(".disableRemoteBtn").toggleClass("off");
    let list;
    let id = localStream.getId();
    list = Array.from(
      document.querySelectorAll(`.video-item:not(#video-item-${id})`)
    );
    list.map(item => {
      if (item.style.display === "none") {
        item.style.display = "block";
        return 1;
      }
      item.style.display = "none";
      return 0;
    });
  });

  $(window).resize(function(_) {
    if (isMobileSize()) {
      Renderer.enterFullScreen();
    } else {
      Renderer.exitFullScreen();
    }
    Renderer.customRender(streamList, options.displayMode, mainId);
  });

  // Dbl click to switch high/low stream
  $(".ag-container").dblclick(function(e) {
    let dom = e.target;
    while (!dom.classList.contains("video-item")) {
      dom = dom.parentNode;
      if (dom.classList.contains("ag-main")) {
        return;
      }
    }
    let id = parseInt(dom.id.split("-")[2], 10);
    if (id !== mainId) {
      let next = options.displayMode === 2 ? SHARE_ID : id;
      // Force to swtich
      setHighStream(mainId, next);
      mainId = next;
      mainStream = getStreamById(mainId);
    }
    Renderer.customRender(streamList, options.displayMode, mainId);
  });

  $(document).mousemove(function(_) {
    if (global._toolbarToggle) {
      clearTimeout(global._toolbarToggle);
    }
    $(".ag-btn-group").addClass("active");
    global._toolbarToggle = setTimeout(function() {
      $(".ag-btn-group").removeClass("active");
    }, 2500);
  });
};

// Get various scheduled network stats
const infoDetectSchedule = () => {
  let no = streamList.length;
  for (let i = 0; i < no; i++) {
    let item = streamList[i];
    let id = item.getId();
    let box = $(`#video-item-${id} .video-item-box`);
    let width;
    let height;
    let frameRate;
    let HighOrLow;
    // Whether high or low stream
    if (id === mainId) {
      HighOrLow = "High";
    } else {
      HighOrLow = "Low";
    }
    if (i === no - 1) {
      HighOrLow = "local";
    }
    item.getStats(function(e) {
      if (i === no - 1) {
        width = e.videoSendResolutionWidth;
        height = e.videoSendResolutionHeight;
        frameRate = e.videoSendFrameRate;
      } else {
        width = e.videoReceivedResolutionWidth;
        height = e.videoReceivedResolutionHeight;
        frameRate = e.videoReceiveFrameRate;
      }

      let str = `
        <p>uid: ${id}</p>
        <p>${width}*${height} ${frameRate}fps</p>
        <p>${HighOrLow}</p>
      `;
      box.html(str);
    });
  }
};

// ------------- start --------------
// ----------------------------------
options = optionsInit();
uiInit(options);
// eslint-disable-next-line
client = AgoraRTC.createClient({
  mode: options.transcode
});
subscribeMouseEvents();
subscribeStreamEvents();
clientInit(client, options).then(uid => {
  // Use selected device
  let config = isSafari()
    ? {}
    : {
        cameraId: options.cameraId,
        microphoneId: options.microphoneId
      };
  localStream = streamInit(uid, options, config);

  // Enable dual stream
  if (options.attendeeMode !== "audience") {
    // MainId default to be localStream's ID
    mainId = uid;
    mainStream = localStream;
  }
  enableDualStream();
  localStream.init(
    () => {
      if (options.attendeeMode !== "audience") {
        addStream(localStream, true);
        client.publish(localStream, err => {
          localLog("Publish local stream error: " + err);
        });
      }
    },
    err => {
      localLog("getUserMedia failed", err);
    }
  );
});

if (DUAL_STREAM_DEBUG) {
  setInterval(infoDetectSchedule, 1000);
}
