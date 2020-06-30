<template>
  <div class="hello">
    <div class="agora-title-box">
      <div class="agora-title">Agora Basic Video Call</div>
    </div>
    <div class='agora-box'>
      <div class="agora-input">
        <div class="text-agora">* Appid</div>
        <el-input v-model="option.appid" placeholder="Appid" clearable style="width: 320px"></el-input>
      </div>
      <div class="agora-input">
        <div class="text-agora">Token</div>
        <el-input v-model="option.token" placeholder="Token" clearable style="width: 320px"></el-input>
      </div>
      <div class="agora-input">
        <div class="text-agora">* Channel Name</div>
        <el-input v-model="option.channel" placeholder="Channel Name" clearable style="width: 320px"></el-input>
      </div>
      <div class="agora-button">
        <el-row>
          <el-button type="primary" @click="joinEvent" :disabled='showJoin' style="width: 90px; margin: 10px">join</el-button>
          <el-button type="primary" @click='leaveEvent' plain :disabled='!showJoin' style="width: 90px; margin: 10px">leave</el-button>
        </el-row>
      </div>
    </div> 
    <div class="agora-view">
      <div class="agora-video">
        <StreamPlayer :stream="localStream" :domId="localStream.getId()" v-if="localStream"></StreamPlayer>
      </div>
      <div class="agora-video" :key="index" v-for="(remoteStream, index) in remoteStreams">
        <StreamPlayer :stream="remoteStream" :domId="remoteStream.getId()"></StreamPlayer>
      </div>
    </div>   
  </div>
</template>

<script>
import RTCClient from "../agora-rtc-client";
import StreamPlayer from "./stream-player";
import { log } from '../utils/utils'

export default {
  components: {
    StreamPlayer
  },
  data () {
    return {
      option: {
        appid: '',
        token: '',
        uid: null,
        channel: '',
      },
      showJoin: false,
      localStream: null,
      remoteStreams: [],
    }
  },
  props: {
    msg: String
  },
  
  methods: {
    joinEvent () {
      if(!this.option.appid) {
        this.judge('Appid')
        return
      }
      if(!this.option.channel) {
        this.judge('Channel Name')
        return
      }
      this.rtc.joinChannel(this.option).then(() => {
        this.$message({
          message: 'Join Success',
          type: 'success'
        });
        this.rtc.publishStream().then(() => {
          this.$message({
            message: 'Publish Success',
            type: 'success'
          });
          this.localStream = this.rtc.localStream
        }).catch((err) => {
          this.$message.error('Publish Failure');
          log('publish local error', err)
        })
      }).catch((err) => {
        this.$message.error('Join Failure');
        log('join channel error', err)
      })
      this.showJoin = true
    },
    leaveEvent () {
      this.showJoin = false
      this.rtc.leaveChannel().then(() => {
        this.$message({
          message: 'Leave Success',
          type: 'success'
        });
      }).catch((err) => {
        this.$message.error('Leave Failure')
        log('leave error', err)
      })
      this.localStream = null
      this.remoteStreams = []
    },
    judge(detail) {
      this.$notify({
        title: 'Notice',
        message: `Please enter the ${detail}`,
        position: 'top-left',
        type: 'warning'
      });
    },
  },
  created() {
    this.rtc = new RTCClient()
    let rtc = this.rtc
    rtc.on('stream-added', (evt) => {
      let {stream} = evt
      log("[agora] [stream-added] stream-added", stream.getId())
      rtc.client.subscribe(stream)
    })
      
    rtc.on('stream-subscribed', (evt) => {
      let {stream} = evt
      log("[agora] [stream-subscribed] stream-added", stream.getId())
      if (!this.remoteStreams.find((it) => it.getId() === stream.getId())) {
        this.remoteStreams.push(stream)
      }
    })

    rtc.on('stream-removed', (evt) => {
      let {stream} = evt
      log('[agora] [stream-removed] stream-removed', stream.getId())
      this.remoteStreams = this.remoteStreams.filter((it) => it.getId() !== stream.getId())
    }) 
  }
 }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
  .agora-box {

  }
  .agora-title {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    font-size: 32px;
    font-weight: bold;
    text-align: center;
    color: #2c3e50;
  }
  .agora-view {
    display: flex;
  }
  .agora-video {
    width: 320px;
    height: 240px;
    margin: 20px;
  }
  .agora-input {
    margin: 20px;
  }
  .agora-text {
    margin: 5px;
    font-size: 16px;
    font-weight: bold;
  }
  .agora-button {
    margin-left: 10px;
  }
  .agora-video {
    width: 320px;
    height: 240px;
  }
  .text-agora {
    font-weight: bold;
  }
</style>
