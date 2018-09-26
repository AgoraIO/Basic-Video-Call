<template>
  <div class="wrapper index">
    <div class="ag-header"></div>
    <div class="ag-main">
      <section class="login-wrapper">
        <div class="login-header">
          <img :src="require('../assets/images/ag-logo.png')" alt="" />
          <p class="login-title">AgoraWeb v2.1</p>
          <p class="login-subtitle">Powering Real-Time Communications</p>
        </div>
        <div class="login-body">
          <div class="columns">
            <div class="column is-12">
                <InputChannel @change="handleChannel" placeholder="Input a room name here"></InputChannel>
            </div>
          </div>
          <div class="columns">
            <div class="column is-7">
              <BaseOptions 
                @change="handleBaseMode">
              </BaseOptions>
            </div>
            <div class="column is-5">
              <AdvancedOptions 
                :onRadioChange="handleTranscode" 
                :onSelectChange="handleVideoProfile">
              </AdvancedOptions>
            </div>
          </div>
          <div class="columns">
            <div class="column">
              <div id="attendeeMode" class="control">
                <label class="radio">
                  <input @click="handleAttendeeMode"
                  value="video" type="radio" 
                  name="attendee" checked />
                  <span class="radio-btn">
                  </span>
                  <span class="radio-img video">
                  </span>
                  <span class="radio-msg">Video Call : join with video call</span>
                </label>
                <br />
                <label class="radio">
                  <input @click="handleAttendeeMode"
                  value="audio-only" type="radio" 
                  name="attendee" />
                  <span class="radio-btn">
                  </span>
                  <span class="radio-img audio">
                  </span>
                  <span class="radio-msg">Audio-only : join with audio call</span>
                </label>
                <br />
                <label class="radio">
                  <input @click="handleAttendeeMode"
                  value="audience" type="radio" 
                  name="attendee" />
                  <span class="radio-btn">
                  </span>
                  <span class="radio-img audience">
                  </span>
                  <span class="radio-msg">Audience : join as an audience</span>
                </label>
              </div>
            </div>
          </div>
        </div>
        <div class="login-footer">
          <a id="joinBtn" 
            @click="handleJoin"
            :disabled="!joinBtn" 
            class="ag-rounded button is-info">Join
          </a>
        </div>
      </section>
    </div>
    <div class="ag-footer">
      <a class="ag-href" href="https://www.agora.io">
        <span>Powered By Agora</span>
      </a>
      <div>
      <span>Interested in Agora video call SDK? Contact </span>
      <span class="ag-contact">sales@agora.io</span>
      </div>
    </div>
  </div>
</template>

<script>
import * as Cookies from "js-cookie"
export default {
  components: {
    BaseOptions: () => import("@/components/BaseOptions"),
    AdvancedOptions: () => import("@/components/AdvancedOptions"),
    InputChannel: () => import("@/components/InputChannel")
  },

  data() {
    return {
      joinBtn: false,
      channel: "",
      baseMode: "avc",
      transcode: "interop",
      attendeeMode: "video",
      videoProfile: "480p_4"
    };
  },

  methods: {
    handleChannel(val, state) {
      this.channel = val;
      this.joinBtn = state;
    },

    handleBaseMode(val) {
      this.baseMode = val;
    },

    handleTranscode(val) {
      this.transcode = val;
    },

    handleVideoProfile(val) {
      this.videoProfile = val;
    },

    handleAttendeeMode(e) {
      this.attendeeMode = e.currentTarget.value;
    },

    handleJoin() {
      if (!this.joinBtn) {
        return;
      }
      Cookies.set("channel", this.channel);
      Cookies.set("baseMode", this.baseMode);
      Cookies.set("transcode", this.transcode);
      Cookies.set("attendeeMode", this.attendeeMode);
      Cookies.set("videoProfile", this.videoProfile);
      this.$router.push("/meeting");
    }
  },

  mounted() {
    this.$nextTick(() => {
      window.addEventListener("keypress", e => {
        e.keyCode === 13 && this.handleJoin();
      });
    });
  }
};
</script>

<style scoped>
.index.wrapper {
  background: url("../assets/images/ag-index-background.png") center no-repeat;
  background-size: cover;
  min-height: 100%;
  height: auto;
  display: flex;
  justify-content: center;
  align-items: center;
  flex-flow: column;
}

.index .ag-footer {
  display: flex;
  padding: 0 30px;
  justify-content: space-between;
  align-items: center;
}

.index .ag-main {
  flex-grow: 1;
}

.ag-contact {
  color: rgb(0, 182, 237);
  cursor: pointer;
}

.login-wrapper {
  color: rgb(216, 216, 216);
  width: 284px;
  font-size: 14px;
}

.login-header,
.login-footer {
  text-align: center;
  margin: 30px 0;
}

.login-header p {
  margin: 10px 0;
}

.login-header img {
  width: 50%;
}

.login-title {
  font-size: 16px;
  color: #fff;
}

.login-subtitle {
  color: #637c8e;
}

#joinBtn {
  width: 100%;
  font-size: 18px;
  padding: 10px 10px;
  background: rgb(0, 182, 237);
}

#joinBtn:hover {
  box-shadow: 0 2px 2px 1px #0056ed;
  transition: box-shadow 0.1s ease-in-out;
}

#attendeeMode input[type="radio"] {
  position: absolute;
  left: -99999px;
}

#attendeeMode {
  line-height: 1.3;
}

#attendeeMode .radio {
  display: flex;
  align-items: center;
  /* justify-content: space-around; */
}
#attendeeMode .radio-img {
  width: 30px;
  height: 30px;
  /* vertical-align: middle; */
}
#attendeeMode .radio-img.video {
  position: relative;
  left: 3px;
  background-size: cover;
  background: url("../assets/images/ag-video-s.png") no-repeat center;
}
#attendeeMode .radio-img.audio {
  background: url("../assets/images/ag-mic-s.png") no-repeat center;
}
#attendeeMode .radio-img.audience {
  background: url("../assets/images/ag-audience.png") no-repeat center;
}
#attendeeMode .radio-btn {
  width: 24px;
  height: 24px;
  margin-right: 10px;
  background: url("../assets/images/ag-oval.png") no-repeat center;
}
#attendeeMode .radio-msg {
  margin-left: 10px;
  color: #637c8e;
}

#attendeeMode input[type="radio"]:checked + .radio-btn {
  background: url("../assets/images/ag-oval-active.png") no-repeat center;
}
#attendeeMode input[type="radio"]:checked + .radio-btn + .radio-img.video {
  background: url("../assets/images/ag-video-active-s.png") no-repeat center;
}
#attendeeMode input[type="radio"]:checked + .radio-btn + .radio-img.audio {
  background: url("../assets/images/ag-mic-active-s.png") no-repeat center;
}
#attendeeMode input[type="radio"]:checked + .radio-btn + .radio-img.audience {
  background: url("../assets/images/ag-audience-active.png") no-repeat center;
}
#attendeeMode
  input[type="radio"]:checked
  + .radio-btn
  + .radio-img
  + .radio-msg {
  color: rgb(0, 182, 237);
  opacity: 1;
}
</style>
