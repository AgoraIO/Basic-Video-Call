import React, {useState, useEffect}  from 'react'
import { log } from '../utils/utils'
import "./Home.css";
import { Button , notification} from 'antd';
import { Input } from 'antd';
import { SmileOutlined } from '@ant-design/icons';
import {startBasicCall, leaveCall} from '../Agora_RTC'

export default function Home() {

  const [appid, setAppid] = useState('')
  const [channel, setChannel] = useState('')
  const [token, setToken] = useState('')
  const [isjoin, setIsJoin] = useState(false)
  

  const bindInputAppid = (event) => {
    setAppid(event.target.value)
  }

  const bindInputToken = (event) => {
    setToken(event.target.value)
  }

  const bindInputChannel = (event) => {
    setChannel(event.target.value)
  }

  const handleClickJoin = () => {
    if(!appid || !channel || !token) {
      if(!appid) {
        openNotification('appid')
      }
      if(!channel) {
        openNotification('channel')
      }
      if(!token) {
        openNotification('token')
      }
      return
    }

    let options = {
      appId: appid,
      channel: channel,
      token: token,
    }
    startBasicCall(options)
    log('join channel success')
    setIsJoin(true)
  }

  const handleClickLeave = () => {
    leaveCall()
    log('client leaves channel success')
    setIsJoin(false)
  }

  const openNotification = (placement) => {
    notification.open({
      message: 'Please enter complete information',
      description:
      `The ${placement} is empty`,
      icon: <SmileOutlined style={{ color: '#108ee9' }} />,
    });
  };

  return (
    <div className='home-box'>
      <div className='title-box'>
        <span id='title-agora'>Agora Basic Video Call</span>
        <a href="https://github.com/AgoraIO/API-Examples" className="aperture">
        <span className="github"></span>
        </a>
      </div>
      <div className='message-box'>
        <div>
          <span className='text-agora'>AppID</span>
          <div className='input-box'>
            <Input placeholder="Enter Appid" value={appid} onChange={bindInputAppid} allowClear='true'/>
          </div>
        </div>
        <div>
          <span className='text-agora'>Token</span>
          <div className='input-box'>
            <Input placeholder="Enter Token" value={token} onChange={bindInputToken} allowClear='true'/>
          </div>
        </div>
        <div>
          <span className='text-agora'>Channel</span>
          <div className='input-box'>
            <Input placeholder="Enter Channel Name" value={channel} onChange={bindInputChannel} allowClear='true'/>
          </div>
        </div>
        <div className='click-box'>
          <div className='joinButton'>
            <Button type="primary" onClick={handleClickJoin} disabled={isjoin}>Join</Button>
          </div>
          <div className='leaveButton'>
            <Button onClick={handleClickLeave} disabled={!isjoin}>Leave</Button>
          </div>
        </div>
      </div>
      <div className='video-agora-box'>
        <div id='video-agora-local'></div>
      </div>
    </div>
  )
}

