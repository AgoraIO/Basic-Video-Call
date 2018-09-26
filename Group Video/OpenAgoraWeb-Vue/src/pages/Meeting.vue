<template>
  <div class="wrapper meeting">
    <div class="ag-header">
      <div class="ag-header-lead">
        <img class="header-logo" :src="require('../assets/images/ag-logo.png')" alt="" />
        <span>AgoraWeb v2.1</span>
      </div>
      <div class="ag-header-msg">
          Room:&nbsp;<span id="room-name">{{channel}}</span>
      </div>
    </div>
    <div class="ag-main">
      <div class="ag-container">
        <AgoraVideoCall 
          :videoProfile="videoProfile"
          :channel="channel"
          :transcode="transcode"
          :attendeeMode="attendeeMode"
          :baseMode="baseMode"
          :appId="appId"
          :uid="uid"></AgoraVideoCall>
      </div>  
    </div>
    <div class="ag-footer">
      <a class="ag-href" href="https://www.agora.io"><span>Powered By Agora</span></a>
      <span>Talk to Support: 400 632 6626</span>
    </div>
  </div>
</template>

<script>
import * as Cookies from "js-cookie";
import AgoraVideoCall from "@/components/AgoraVideoCall";
import {AGORA_APP_ID} from "@/agora.config"
export default {
  components: {
    AgoraVideoCall
  },
  data() {
    return {
      videoProfile: Cookies.get("videoProfile").split(",")[0] || "480p_4",
      channel: Cookies.get("channel") || "test",
      transcode: Cookies.get("transcode") || "interop",
      attendeeMode: Cookies.get("attendeeMode") || "video",
      baseMode: Cookies.get("baseMode") || "avc",
      uid: undefined
    };
  },

  created() {
    this.appId = AGORA_APP_ID
    if (!this.appId) {
      return alert('Get App ID first!')
    }
  }
};
</script>

<style scoped>
.meeting.wrapper {
  background: rgb(12, 43, 64);
  height: 100%;
}

.meeting .ag-header {
  padding: 20px 30px;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.ag-header-lead {
  font-size: 16px;
  line-height: 35px;
}

.meeting .ag-footer {
  padding: 0 30px;
  display: flex;
  align-items: center;
}

.meeting .ag-main {
  position: relative;
}

.meeting .ag-footer :first-child {
  margin-right: 80px;
}

.header-logo {
  width: 60px;
  height: 35px;
  margin-right: 12px;
}

.ag-container {
  width: calc(100% - 60px);
  height: 100%;
  margin: 0 auto;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 12px;
}
</style>

