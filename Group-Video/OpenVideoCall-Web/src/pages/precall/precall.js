import AgoraRTC from "agora-rtc-sdk";
import "bulma";
import $ from "jquery";
import "bulma-switch/dist/bulma-switch.min.css";
import * as Cookies from "js-cookie";
import "@/assets/css/icons.css";
// eslint-disable-next-line
import Polyfill from "@/utils/Polyfill";
import { isSafari } from "../../utils/BrowserCheck";
import { RESOLUTION_ARR, APP_ID } from "@/utils/Settings";
import "@/assets/global.scss";
import "./precall.scss";

let stream;
let recvStream;
let client;
let receiver;
let key;
let _testChannel = String(
  Number.parseInt(new Date().getTime(), 10) + Math.floor(Math.random() * 1000)
);

// Initilize ui from the cookies set in index.html

const uiInit = () => {
  document.querySelector(
    ".ag-header-lead span"
  ).innerHTML = `AgoraWeb v${agoraVersion.slice(1)}`;
  return new Promise((resolve, reject) => {
    // Init info card
    let profile = RESOLUTION_ARR[Cookies.get("videoProfile")];
    // Get the trancoding value from the cookies
    let transcodeValue = Cookies.get("transcode") || "interop";
    // return a codec for the transcode profile
    let transcode = (() => {
      switch (transcodeValue) {
        case "":
          return "VP8-only";
        default:
        case "interop":
          return "VP8 &amp; H264";
        case "h264_interop":
          return "H264-only";
      }
    })();

    // Info object representing the user's video profile.
    let info = {
      videoProfile: `${profile[0]}x${profile[1]} ${profile[2]}fps ${
        profile[3]
      }kbps`,
      channel: Cookies.get("channel") || "test",
      transcode,
      attendeeMode: Cookies.get("attendeeMode") || "video", // video, audio or audience
      baseMode: Cookies.get("baseMode") || "avc"
    };
    // Init key
    if (info.baseMode === "avc") {
      key = APP_ID;
    }

    Object.entries(info).map(item => {
      // Find the necessary dom element and insert appropriate info
      return $("#" + item[0]).html(item[1]);
    });

    // Video-attendee's switch
    if (info.attendeeMode === "video") {
      $("#enableVideo").prop("checked", true);
    }

    // Init compatibility result
    // eslint-disable-next-line
    if (AgoraRTC.checkSystemRequirements()) {
      $("#compatibility").html("AgoraRTC supported.");
    } else {
      $("#compatibility").html(
        "AgoraRTC not fully supported and some functions may be lost."
      );
    }

    // Init device options
    if (isSafari()) {
      // If safari, disable set device since deviceId changes all the time
      $("#audioDevice")
        .parent()
        .remove();
      $("#videoDevice")
        .parent()
        .remove();
      resolve();
    } else {
      // eslint-disable-next-line
      AgoraRTC.getDevices(function(devices) {
        let videoHtml = "";
        let audioHtml = "";
        devices.forEach(function(item) {
          if (item.kind === "audioinput") {
            audioHtml +=
              "<option value=" + item.deviceId + ">" + item.label + "</option>";
          }
          if (item.kind === "videoinput") {
            videoHtml +=
              "<option value=" + item.deviceId + ">" + item.label + "</option>";
          }
        });
        $("#videoDevice").html(videoHtml);
        $("#audioDevice").html(audioHtml);
        resolve();
      });
    }
  });
};

// It schedules the precall test necessary to measure video statistics like packet loss
const Schedule = {
  DURATION: 10,
  volume: 0,
  volumeBar: $("#volume"),
  targetStream: {},
  getVolume(stream) {
    let vol = Math.round(stream.getAudioLevel() * 100);
    if (isNaN(vol)) {
      return 0;
    }
    return vol;
  },
  scheduleVolumeDetect: {},
  scheduleEnd: {},
  start() {
    let that = this;
    if (!this.targetStream) {
      console.error("Please init Schedule with a targetStream!");
      return;
    }
    $(".ag-connect-test").prepend('<div id="testDuration"></div>');
    // Init volume detector
    this.scheduleVolumeDetect = setInterval(function() {
      that.volume = that.getVolume(that.targetStream);
      that.volumeBar.val(that.volume);
    }, 100);
    // Init timer for detect
    this.scheduleEnd = setTimeout(function() {
      $("#testDuration").attr(
        "style",
        "animation-play-state:paused;background-color:#7ED321"
      );
      clearInterval(that.scheduleVolumeDetect);
      that.targetStream.getStats(function(e) {
        let [
          videoBytes,
          audioBytes,
          videoPackets,
          audioPackets,
          videoPacketsLost,
          audioPacketsLost
        ] = [
          e.videoReceiveBytes,
          e.audioReceiveBytes,
          e.videoReceivePackets,
          e.audioReceivePackets,
          e.videoReceivePacketsLost,
          e.audioReceivePacketsLost
        ];

        // Do calculate
        let videoBitrate =
          (videoBytes / 1000 / that.DURATION).toFixed(2) + "KB/s";
        let audioBitrate =
          (audioBytes / 1000 / that.DURATION).toFixed(2) + "KB/s";
        let vPacketLoss =
          ((videoPacketsLost / videoPackets) * 100).toFixed(2) + "%";
        let aPacketLoss =
          ((audioPacketsLost / audioPackets) * 100).toFixed(2) + "%";
        let sumPacketLoss = (
          (videoPacketsLost / videoPackets) * 100 +
          (audioPacketsLost / audioPackets) * 100
        ).toFixed(2);
        // Render result
        let videoCard = $("#videoCard .ag-card-body");
        let audioCard = $("#audioCard .ag-card-body");

        let videoCardHtml = `
          <div class="ag-test-result">
            <p>Video Bitrate: ${videoBitrate}</p>
            <p>Packet Loss: ${vPacketLoss}</p>
          </div>
        `;
        let audioCardHtml = `
          <div class="ag-test-result">
            <p>Audio Bitrate: ${audioBitrate}</p>
            <p>Packet Loss: ${aPacketLoss}</p>
          </div>
        `;
        let qualityHtml;
        if (sumPacketLoss < 1) {
          qualityHtml = "Excellent";
        } else if (sumPacketLoss < 5) {
          qualityHtml = "Good";
        } else if (sumPacketLoss < 10) {
          qualityHtml = "Poor";
        } else if (sumPacketLoss < 100) {
          qualityHtml = "Bad";
        } else {
          qualityHtml = "Get media failed.";
        }

        videoCard.find(".initial").hide();
        videoCard.find(".result").html(videoCardHtml);
        audioCard.find(".initial").hide();
        audioCard.find(".result").html(audioCardHtml);
        $("#testDuration")
          .empty()
          .after(`<span style="">${qualityHtml}</span>`);
      });
      // Update to step 2
      $("#stepTwo").addClass("active");
      $("#stepOne").removeClass("active");
    }, this.DURATION * 1000);
  },
  reset() {
    $("#testDuration").remove();
    this.volume = 0;
    this.volumeBar.val(0);
    if (this.scheduleVolumeDetect) {
      clearInterval(this.scheduleVolumeDetect);
    }
    if (this.scheduleEnd) {
      clearTimeout(this.scheduleEnd);
    }
    this.targetStream = null;
  },
  init(stream, duration) {
    this.targetStream = stream;
    this.DURATION = duration;
  }
};

// Initialize client and local stream with appropriate parameters like codec, video profile etc.
const clientInit = () => {
  return new Promise((resolve, reject) => {
    // eslint-disable-next-line
    client = AgoraRTC.createClient({
      mode: Cookies.get("transcode") || "interop"
    });

    client.init(key, () => {
      client.join(key, _testChannel, undefined, uid => {
        // Init stream
        let defaultConfig = {
          streamID: uid,
          audio: true,
          video: true,
          screen: false,
          cameraId: $("#videoDevice").val(),
          microphoneId: $("#audioDevice").val()
        };
        // eslint-disable-next-line
        stream = AgoraRTC.createStream(defaultConfig);
        stream.setVideoProfile(
          Cookies.get("videoProfile").split(",")[0] || "480p_4"
        );
        stream.init(
          () => {
            client.publish(stream);
            if (!$("#enableVideo").prop("checked")) {
              stream.disableVideo();
            }
            resolve();
          },
          err => {
            console.log("getUserMedia failed", err);
            reject(err);
          }
        );
      });
    });
  });
};
// Initialize receiver to recieve remote streams
const receiverInit = () => {
  return new Promise((resolve, reject) => {
    // eslint-disable-next-line
    receiver = AgoraRTC.createClient({
      mode: Cookies.get("transcode") || "interop"
    });
    // When a remote stream is added to the channel
    receiver.on("stream-added", function(evt) {
      let stream = evt.stream;
      receiver.subscribe(stream, function(err) {
        console.log("Subscribe stream failed", err);
      });
    });

    // When a remote stream is subscribed
    receiver.on("stream-subscribed", function(evt) {
      if (recvStream) {
        recvStream.stop();
      }
      Schedule.reset();
      recvStream = evt.stream;
      Schedule.init(recvStream, 10);
      recvStream.play("videoItem");
      Schedule.start();
    });

    // When a remote leaves the channel
    receiver.on("peer-leave", function(_) {
      $("#videoItem").empty();
    });

    // When a remote stream is removed
    receiver.on("stream-removed", function(_) {
      $("#videoItem").empty();
    });


    receiver.init(key, () => {
      receiver.join(
        key,
        _testChannel,
        undefined,
        uid => {
          resolve(uid);
        },
        err => {
          reject(err);
        }
      );
    });
  });
};

// Video call preview screen with the selected video source.
const setDevice = () => {
  if (!stream) {
    throw Error("Stream not existed!");
  }
  return new Promise((resolve, reject) => {
    let id = stream.getId();
    client.unpublish(stream);
    stream.stop();
    stream.close();
    // Reinit stream
    let defaultConfig = {
      streamID: id,
      audio: true,
      video: true,
      screen: false,
      cameraId: $("#videoDevice").val(),
      microphoneId: $("#audioDevice").val()
    };
    // eslint-disable-next-line
    stream = AgoraRTC.createStream(defaultConfig);
    stream.setVideoProfile(
      Cookies.get("videoProfile").split(",")[0] || "480p_4"
    );
    stream.init(
      () => {
        if (!$("#enableVideo").prop("checked")) {
          stream.disableVideo();
        }
        client.publish(stream);
        resolve();
      },
      err => {
        console.log("getUserMedia failed", err);
        reject(err);
      }
    );
  });
};

// Subscribe to events from buttons and dropdowns
const subscribeEvents = () => {
  $("#quickJoinBtn").on("click", function() {
    Cookies.set("cameraId", $("#videoDevice").val());
    Cookies.set("microphoneId", $("#audioDevice").val());
    try {
      client && client.unpublish(stream);
      stream && stream.close();
      client &&
        client.leave(
          () => {
            console.log("Client succeed to leave.");
          },
          () => {
            console.log("Client failed to leave.");
          }
        );
    } finally {
      // Redirect to index
      window.location.href = "meeting.html";
    }
  });

  // When a video device changes
  $("#videoDevice").change(function(_) {
    Schedule.reset();
    setDevice();
  });

  // When an audio device changes
  $("#audioDevice").change(function(_) {
    Schedule.reset();
    setDevice();
  });

  // When the user toggles between audio and video mode
  $("#enableVideo").change(function(_) {
    if ($("#enableVideo").prop("checked")) {
      stream.enableVideo();
    } else {
      stream.disableVideo();
    }
  });
};

// ---------------  start ----------------
uiInit().then(() => {
  subscribeEvents();
  clientInit();
  receiverInit();
});
