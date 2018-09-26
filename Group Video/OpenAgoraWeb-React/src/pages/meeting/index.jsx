import React from 'react'
import * as Cookies from 'js-cookie'

import './meeting.css'
import AgoraVideoCall from '../../components/AgoraVideoCall'
import {AGORA_APP_ID} from '../../agora.config'

class Meeting extends React.Component {
  constructor(props) {
    super(props)
    this.videoProfile = (Cookies.get('videoProfile')).split(',')[0] || '480p_4',
      this.channel = Cookies.get('channel') || 'test',
      this.transcode = Cookies.get('transcode') || 'interop',
      this.attendeeMode = Cookies.get('attendeeMode') || 'video',
      this.baseMode = Cookies.get('baseMode') || 'avc'
    this.appId = AGORA_APP_ID
    if (!this.appId) {
      return alert('Get App ID first!')
    }
    this.uid = undefined

  }

  render() {
    return (
      <div className="wrapper meeting">
        <div className="ag-header">
          <div className="ag-header-lead">
            <img className="header-logo" src={require('../../assets/images/ag-logo.png')} alt="" />
            <span>AgoraWeb v2.1</span>
          </div>
          <div className="ag-header-msg">
            Room:&nbsp;<span id="room-name">{this.channel}</span>
          </div>
        </div>
        <div className="ag-main">
          <div className="ag-container">
            <AgoraVideoCall
              videoProfile={this.videoProfile}
              channel={this.channel}
              transcode={this.transcode}
              attendeeMode={this.attendeeMode}
              baseMode={this.baseMode}
              appId={this.appId}
              uid={this.uid}></AgoraVideoCall>
          </div>
        </div>
        <div className="ag-footer">
          <a className="ag-href" href="https://www.agora.io"><span>Powered By Agora</span></a>
          <span>Talk to Support: 400 632 6626</span>
        </div>
      </div>
    )
  }
}

export default Meeting