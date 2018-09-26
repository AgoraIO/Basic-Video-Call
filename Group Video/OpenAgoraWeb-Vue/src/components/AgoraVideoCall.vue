<template>
  <div id="ag-canvas">
    <div class="ag-btn-group">
      <span 
        @click="handleExit"
        class="ag-btn exitBtn"
        :class="{'disabled': !readyState}"
        title="Exit">
        <i class="ag-icon ag-icon-leave"></i>
      </span>
      <span v-if="attendeeMode === 'video'"
        @click="handleCamera"
        class="ag-btn videoControlBtn" 
        title="Enable/Disable Video">
        <i class="ag-icon ag-icon-camera"></i>
        <i class="ag-icon ag-icon-camera-off"></i>
      </span>
      <span v-if="attendeeMode !== 'audience'"
        @click="handleMic"
        class="ag-btn audioControlBtn" 
        title="Enable/Disable Audio">
        <i class="ag-icon ag-icon-mic"></i>
        <i class="ag-icon ag-icon-mic-off"></i>
      </span>
      <span 
        @click="switchDisplay"
        class="ag-btn displayModeBtn"
        :class="{'disabled': streamList.length > 4}"
        title="Switch Display Mode">
        <i class="ag-icon ag-icon-switch-display"></i>
      </span>
      <span 
        class= "ag-btn disableRemoteBtn"
        :class="{'disabled': streamList.length > 4 || displayMode !== 'pip'}"
        @click="hideRemote"
        title="Hide Remote Stream">
        <i class="ag-icon ag-icon-remove-pip"></i>
      </span>
    </div>
  </div>
</template>

<script>
import { merge } from "lodash";

const tile_canvas = {
  "1": ["span 12/span 24"],
  "2": ["span 12/span 12/13/25", "span 12/span 12/13/13"],
  "3": ["span 6/span 12", "span 6/span 12", "span 6/span 12/7/19"],
  "4": [
    "span 6/span 12",
    "span 6/span 12",
    "span 6/span 12",
    "span 6/span 12/7/13"
  ],
  "5": [
    "span 3/span 4/13/9",
    "span 3/span 4/13/13",
    "span 3/span 4/13/17",
    "span 3/span 4/13/21",
    "span 9/span 16/10/21"
  ],
  "6": [
    "span 3/span 4/13/7",
    "span 3/span 4/13/11",
    "span 3/span 4/13/15",
    "span 3/span 4/13/19",
    "span 3/span 4/13/23",
    "span 9/span 16/10/21"
  ],
  "7": [
    "span 3/span 4/13/5",
    "span 3/span 4/13/9",
    "span 3/span 4/13/13",
    "span 3/span 4/13/17",
    "span 3/span 4/13/21",
    "span 3/span 4/13/25",
    "span 9/span 16/10/21"
  ]
};

export default {
  data() {
    return {
      client: {},
      localStream: {},
      shareClient: {},
      shareStream: {},
      displayMode: "pip",
      streamList: [],
      readyState: false
    };
  },

  props: [
    "transcode",
    "attendeeMode",
    "videoProfile",
    "channel",
    "baseMode",
    "appId",
    "uid"
  ],

  methods: {
    streamInit(uid, attendeeMode, videoProfile, config) {
      let defaultConfig = {
        streamID: uid,
        audio: true,
        video: true,
        screen: false
      };

      switch (attendeeMode) {
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

      let stream = AgoraRTC.createStream(merge(defaultConfig, config));
      stream.setVideoProfile(videoProfile);
      return stream;
    },

    subscribeStreamEvents() {
      let rt = this;
      rt.client.on("stream-added", function(evt) {
        let stream = evt.stream;
        console.log("New stream added: " + stream.getId());
        console.log("At " + new Date().toLocaleTimeString());
        console.log("Subscribe ", stream);
        rt.client.subscribe(stream, function(err) {
          console.log("Subscribe stream failed", err);
        });
      });

      rt.client.on("peer-leave", function(evt) {
        console.log("Peer has left: " + evt.uid);
        console.log(new Date().toLocaleTimeString());
        console.log(evt);
        rt.removeStream(evt.uid);
      });

      rt.client.on("stream-subscribed", function(evt) {
        let stream = evt.stream;
        console.log("Got stream-subscribed event");
        console.log(new Date().toLocaleTimeString());
        console.log("Subscribe remote stream successfully: " + stream.getId());
        console.log(evt);
        rt.addStream(stream);
      });

      rt.client.on("stream-removed", function(evt) {
        let stream = evt.stream;
        console.log("Stream removed: " + stream.getId());
        console.log(new Date().toLocaleTimeString());
        console.log(evt);
        rt.removeStream(stream.getId());
      });
    },

    removeStream(uid) {
      this.streamList.map((item, index) => {
        if (item.getId() === uid) {
          item.close();
          let element = document.querySelector("#ag-item-" + uid);
          if (element) {
            element.parentNode.removeChild(element);
          }
          let tempList = [...this.streamList];
          tempList.splice(index, 1);
          this.streamList = tempList;
        }
      });
    },

    addStream(stream, push = false) {
      let repeatition = this.streamList.some(item => {
        return item.getId() === stream.getId();
      });
      if (repeatition) {
        return;
      }
      if (push) {
        this.streamList = this.streamList.concat([stream]);
      } else {
        this.streamList = [stream].concat(this.streamList);
      }
    },

    handleCamera(e) {
      e.currentTarget.classList.toggle("off");
      this.localStream.isVideoOn()
        ? this.localStream.disableVideo()
        : this.localStream.enableVideo();
    },

    handleMic(e) {
      e.currentTarget.classList.toggle("off");
      this.localStream.isAudioOn()
        ? this.localStream.disableAudio()
        : this.localStream.enableAudio();
    },

    switchDisplay(e) {
      if (
        e.currentTarget.classList.contains("disabled") ||
        this.streamList.length <= 1
      ) {
        return;
      }
      if (this.displayMode === "pip") {
        this.displayMode = "tile";
      } else if (this.displayMode === "tile") {
        this.displayMode = "pip";
      } else if (this.displayMode === "share") {
        // do nothing or alert, tbd
      } else {
        console.error("Display Mode can only be tile/pip/share");
      }
    },

    hideRemote(e) {
      if (
        e.currentTarget.classList.contains("disabled") ||
        this.streamList.length <= 1
      ) {
        return;
      }
      let list;
      let id = this.streamList[this.streamList.length - 1].getId();
      list = Array.from(
        document.querySelectorAll(`.ag-item:not(#ag-item-${id})`)
      );
      list.map(item => {
        if (item.style.display !== "none") {
          item.style.display = "none";
        } else {
          item.style.display = "block";
        }
      });
    },

    handleExit(e) {
      if (e.currentTarget.classList.contains("disabled")) {
        return;
      }
      try {
        this.client && this.client.unpublish(this.localStream);
        this.localStream && this.localStream.close();
        this.client &&
          this.client.leave(
            () => {
              console.log("Client succeed to leave.");
            },
            () => {
              console.log("Client failed to leave.");
            }
          );
      } finally {
        this.readyState = false;
        this.client = null;
        this.localStream = null;
        // redirect to index
        this.$router.push("/");
      }
    }
  },

  created() {
    let $ = this;
    // init AgoraRTC local client
    $.client = AgoraRTC.createClient({ mode: $.transcode });
    $.client.init($.appId, () => {
      console.log("AgoraRTC client initialized");
      $.subscribeStreamEvents();
      $.client.join($.appId, $.channel, $.uid, uid => {
        console.log("User " + uid + " join channel successfully");
        console.log("At " + new Date().toLocaleTimeString());
        // create local stream
        // It is not recommended to setState in function addStream
        $.localStream = this.streamInit(uid, $.attendeeMode, $.videoProfile);
        $.localStream.init(
          () => {
            if ($.attendeeMode !== "audience") {
              $.addStream($.localStream, true);
              $.client.publish($.localStream, err => {
                console.log("Publish local stream error: " + err);
              });
            }
            $.readyState = true;
          },
          err => {
            console.log("getUserMedia failed", err);
            $.readyState = true;
          }
        );
      });
    });
  },

  mounted() {
    this.$nextTick(() => {
      // add listener to control btn group
      let canvas = document.querySelector("#ag-canvas");
      let btnGroup = document.querySelector(".ag-btn-group");
      canvas.addEventListener("mousemove", () => {
        if (global._toolbarToggle) {
          clearTimeout(global._toolbarToggle);
        }
        btnGroup.classList.add("active");
        global._toolbarToggle = setTimeout(function() {
          btnGroup.classList.remove("active");
        }, 2000);
      });
    });
  },

  beforeUpdate() {
    let $ = this;
    // rerendering
    let canvas = document.querySelector("#ag-canvas");
    // pip mode (can only use when less than 4 people in channel)
    if ($.displayMode === "pip") {
      let no = $.streamList.length;
      if (no > 4) {
        $.displayMode = "tile";
        return;
      }
      $.streamList.map((item, index) => {
        let id = item.getId();
        let dom = document.querySelector("#ag-item-" + id);
        if (!dom) {
          dom = document.createElement("section");
          dom.setAttribute("id", "ag-item-" + id);
          dom.setAttribute("class", "ag-item");
          canvas.appendChild(dom);
          item.play("ag-item-" + id);
        }
        if (index === no - 1) {
          dom.setAttribute("style", `grid-area: span 12/span 24/13/25`);
        } else {
          dom.setAttribute(
            "style",
            `grid-area: span 3/span 4/${4 + 3 * index}/25;
          z-index:1;width:calc(100% - 20px);height:calc(100% - 20px)`
          );
        }
        item.player.resize && item.player.resize();
      });
    } else if ($.displayMode === "tile") {
      // tile mode
      let no = $.streamList.length;
      $.streamList.map((item, index) => {
        let id = item.getId();
        let dom = document.querySelector("#ag-item-" + id);
        if (!dom) {
          dom = document.createElement("section");
          dom.setAttribute("id", "ag-item-" + id);
          dom.setAttribute("class", "ag-item");
          canvas.appendChild(dom);
          item.play("ag-item-" + id);
        }
        dom.setAttribute("style", `grid-area: ${tile_canvas[no][index]}`);
        item.player.resize && item.player.resize();
      });
    } else if ($.displayMode === "share") {
      // screen share mode (tbd)
    }
  },

  beforeDestroy () {
    this.client && this.client.unpublish(this.localStream);
    this.localStream && this.localStream.close();
    this.client &&
      this.client.leave(
        () => {
          console.log("Client succeed to leave.");
        },
        () => {
          console.log("Client failed to leave.");
        }
      );
  }
};
</script>

<style>
#ag-canvas {
  height: 100%;
  display: grid;
  grid-gap: 10px;
  align-items: center;
  justify-items: center;
  grid-template-rows: repeat(12, auto);
  grid-template-columns: repeat(24, auto);
}

.ag-item :first-child {
  border-radius: 6px;
}
.ag-item {
  border-radius: 6px;
  border: 2px #00b6ed solid;
  background: url("../assets/images/avatar.png") center no-repeat;
  height: 100%;
  width: 100%;
}

#ag-resize-container {
  background-image: none !important;
  background-color: black;
  display: flex;
  justify-content: center;
  height: center;
}

#ag-resize-container .ag-item {
  border: none !important;
}

/* button group */
.ag-btn-group {
  height: 70px;
  width: calc(100% - 60px);
  margin: 0 auto;
  position: absolute;
  bottom: 0;
  background: transparent;
  z-index: 11;
  display: flex;
  justify-content: space-around;
  align-items: center;
}

.ag-btn-group:hover,
.ag-btn-group.active {
  background: rgba(18, 74, 99, 0.8);
}

.ag-btn {
  z-index: 12;
  opacity: 0;
  color: white;
  /* width: 35px;
    height: 35px; */
  cursor: pointer;
  font-size: 60px;
  display: inline-flex;
  justify-content: center;
  align-items: center;
}
.ag-btn:hover {
  color: #00b6ed;
}

.ag-btn.disabled,
.ag-btn.disabled:hover {
  color: #637c8e;
  cursor: not-allowed;
}

.ag-btn-group:hover .ag-btn,
.ag-btn-group.active .ag-btn {
  opacity: 1;
}
.ag-btn.off.videoControlBtn .ag-icon-camera {
  display: none;
}
.ag-btn.videoControlBtn .ag-icon-camera-off {
  display: none;
}
.ag-btn.videoControlBtn .ag-icon-camera {
  display: inline-block;
}
.ag-btn.off.videoControlBtn .ag-icon-camera-off {
  display: inline-block;
}
.ag-btn.off.audioControlBtn .ag-icon-mic {
  display: none;
}
.ag-btn.audioControlBtn .ag-icon-mic-off {
  display: none;
}
.ag-btn.audioControlBtn .ag-icon-mic {
  display: inline-block;
}
.ag-btn.off.audioControlBtn .ag-icon-mic-off {
  display: inline-block;
}

video {
  max-width: unset !important;
}
</style>

