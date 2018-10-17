#include <stdio.h>
#include <iostream>
#include <string.h>
#include<sstream>

#include "AgoraDefs.h"
#include"CommandLineView.h"
#include"Controller/EngineController.h"
#include"EngineModel/AGEventDef.h"

#define MAXLINE 1024

using std::cout;
using std::endl;


CommandLineView::CommandLineView(){
    registerHandler(MSG_RTC_EVENT, (handler_ptr)&CommandLineView::onRtcEventMsg);
    m_printCallback = false;
}

CommandLineView::~CommandLineView() {
}

void CommandLineView::configure(const AppConfig& config) {
    if(m_controller)
        m_controller->sendMsg(this,MSG_CONFIGURE,(void*)&config);
}

void CommandLineView::run(bool openVideoCall) {

    if(openVideoCall) {
        open();
    }

    char buf[MAXLINE];
    memset(buf, 0, MAXLINE);

    cout<< "input command:" << endl;
    while(1) {
        if(fgets(buf, MAXLINE, stdin) == NULL){
            exit();
            break;
        }
            
        buf[strlen(buf)-1] = 0;

        stringstream ss(&buf[0]);
        string cmd;
        ss >> cmd;
        if(cmd == "exit") {
            exit();
            break;
        }
        else {
            if(!onCommand(cmd , ss)) {
                cout<< "failed to call command:" << cmd << endl;
            }
        }
        memset(buf, 0, MAXLINE);
    }

    cout<< "exit command line" << endl;
}

bool CommandLineView::onCommand(const string& cmd, stringstream& params) {

    if(cmd.compare("open") == 0 ) {
        return open();
    } 
    else if(cmd.compare("close") == 0) {
        return close();
    }
    else if(cmd.compare("enable_video") == 0) {
        string enable;
        params >> enable;

        int flag = 0;
        sscanf(enable.c_str(),"%d",&flag);
        return enableVideo((bool)flag);
    }
    else if(cmd.compare("enable_audio") == 0) {
        string enable;
        params >> enable;

        int flag = 0;
        sscanf(enable.c_str(),"%d",&flag);
        return enableAudio((bool)flag);
    }  
    else if(cmd.compare("mute_local_video") == 0) {
        string mute;
        params >> mute;

        int flag = 0;
        sscanf(mute.c_str(),"%d",&flag);
        return muteLocalVideo((bool)flag);
    }
    else if(cmd.compare("mute_local_audio") == 0) {
        string mute;
        params >> mute;

        int flag = 0;
        sscanf(mute.c_str(),"%d",&flag);
        return muteLocalAudio((bool)flag);
    }  
    else if(cmd.compare("enable_local_video") == 0) {
        string enable;
        params >> enable;

        int flag = 0;
        sscanf(enable.c_str(),"%d",&flag);
        return enableLocalVideo((bool)flag);
    }
    else if(cmd.compare("print_device_info") == 0) {
        return printDeviceInfo();
    }  
    else if(cmd.compare("set_cur_camera") == 0) {
        string cameraId;
        params >> cameraId;

        return setCurCamera(cameraId);
    }
    else if(cmd.compare("print_callback") == 0) {
        string enable;
        params >> enable;

        int flag = 0;
        sscanf(enable.c_str(),"%d",&flag);

        m_printCallback = flag;
        return true;
    }
    else if(cmd.compare("mute_remote_video") == 0) {
        string uid;
        params >> uid;

        uint32_t iUid = 0;
        sscanf(uid.c_str(),"%u",&iUid);

        string mute;
        int flag = 0;
        params >> mute;
        sscanf(mute.c_str(),"%d",&flag);

        return muteRemoteVideo(iUid, flag);
    }
    else if(cmd.compare("mute_remote_audio") == 0) {
        string uid;
        params >> uid;

        uint32_t iUid = 0;
        sscanf(uid.c_str(),"%u",&iUid);

        string mute;
        int flag = 0;
        params >> mute;
        sscanf(mute.c_str(),"%d",&flag);

        return muteRemoteAudio(iUid, flag);
    }
    else {
        cout<< "WARNING:invalid command: " << cmd <<endl;
        return false;
    }
}

bool CommandLineView::printDeviceInfo() {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_PRINT_DEVICE_INFO,NULL);
}

bool CommandLineView::open() {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_OPEN, NULL);
}

bool CommandLineView::close() {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_CLOSE, NULL);
}

bool CommandLineView::enableVideo(bool enable) {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_ENABLE_VIDEO , (void*)&enable);
}

bool CommandLineView::enableLocalVideo(bool enable) {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_ENABLE_LOCAL_VIDEO , (void*)&enable);
}

bool CommandLineView::enableAudio(bool enable) {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_ENABLE_AUDIO , (void*)&enable);
}

bool CommandLineView::muteLocalVideo(bool mute) {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_MUTE_LOCAL_VIDEO , (void*)&mute);
}

bool CommandLineView::muteLocalAudio(bool mute) {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_MUTE_LOCAL_AUDIO , (void*)&mute);
}

bool CommandLineView::setCurCamera(const string& cameraId) {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_SET_CUR_CAMERA , (void*)&cameraId);
}

bool CommandLineView::exit() {
    if(!m_controller)
        return false;

    return m_controller->sendMsg(this, MSG_EXIT , NULL);
}

bool CommandLineView::muteRemoteAudio(uint32_t uid, bool mute){
     if(!m_controller)
        return false;   

     muteRemoteMsg msg;
     msg.isVideo = false;
     msg.mute = mute;
     msg.uid = uid;

    return m_controller->sendMsg(this, MSG_MUTE_REMOTE ,(void*)&msg);
}

bool CommandLineView::muteRemoteVideo(uint32_t uid, bool mute){
     if(!m_controller)
        return false;   

     muteRemoteMsg msg;
     msg.isVideo = true;
     msg.mute = mute;
     msg.uid = uid;

    return m_controller->sendMsg(this, MSG_MUTE_REMOTE ,(void*)&msg);
}

bool CommandLineView::onRtcEventMsg(void* msg) {
    if(!m_printCallback)
        return true;

    eventWrapper* wrapper = reinterpret_cast<eventWrapper*>(msg);
    switch(wrapper->id) {
        case EID_JOINCHANNEL_SUCCESS: 
            {                              
                joinChannelSuccessData* data = reinterpret_cast<joinChannelSuccessData*>(wrapper->data);
                cout << "onJoinChannelSuccess uid:" << data->uid << " channel:" << data->channel << " elapsed:" << data->elapsed <<endl;
                break;
            }
        case EID_REJOINCHANNEL_SUCCESS:
            {
                rejoinChannelSuccessData* data = reinterpret_cast<rejoinChannelSuccessData*>(wrapper->data);
                cout << "onRejoinChannelSuccess uid:" << data->uid << " channel:" << data->channel << " elapsed:" << data->elapsed <<endl;
                break;
            }
        case EID_WARNING:
            {
                warningData* data = reinterpret_cast<warningData*>(wrapper->data);
                cout << "onWarning warn:" << data->warn << " msg:" << data->msg <<endl;
                break;
            }
        case EID_ERROR:
            {
                errorData* data = reinterpret_cast<errorData*>(wrapper->data);
                cout << "onError err:" << data->err << " msg:" << data->msg <<endl;
                break;
            }
        case EID_AUDIO_QUALITY:
            {
                audioQualityData* data = reinterpret_cast<audioQualityData*>(wrapper->data);
                cout << "onAudioQuality uid:" << data->uid << " quality:" << data->quality << " delay:"<< data->delay << " lost:" << data->lost <<endl;
                break;
            }
        case EID_LEAVE_CHANNEL:
            {
                leaveChannelData* data = reinterpret_cast<leaveChannelData*>(wrapper->data);
                cout << "onLeaveChannel" <<endl;
                break;
            }
        case EID_RTC_STAT:
            {
                cout << "onRtcStats" <<endl;
                break;
            }
        case EID_USER_JOINED:
            {
                userJoinedData* data = reinterpret_cast<userJoinedData*>(wrapper->data);
                cout << "onUserJoined uid:"<< data->uid << " elapsed:" << data->elapsed <<endl;
                break;
            }
        case EID_USER_OFFLINE:
            {
                userOfflineData* data = reinterpret_cast<userOfflineData*>(wrapper->data);
                cout << "onUserOffline uid:"<< data->uid << " reason:" << (int)data->reason <<endl;
                break;
            }
        case EID_USER_MUTE_AUDIO:
            {
                userMuteAudioData* data = reinterpret_cast<userMuteAudioData*>(wrapper->data);
                cout << "onUserMuteAudio uid:"<< data->uid << " muted:" << (int)data->muted <<endl;
                break;
            }
        case EID_USER_MUTE_VIDEO:
            {
                userMuteVideoData* data = reinterpret_cast<userMuteVideoData*>(wrapper->data);
                cout << "onUserMuteVideo uid:"<< data->uid << " muted:" << (int)data->muted <<endl;
                break;
            }
        case EID_CAMERA_READY:
            {
                cout << "onCameraReady"<<endl;
                break;           
            }
        case EID_CONNECTION_LOST:
            {
                cout << "onConnectionLost"<<endl;
                break; 
            }
        case EID_CONNECTION_INTERRUPTED:
            {
                cout << "onConnectionInterrupted"<<endl;
                break;
            }
        case EID_USER_ENABLE_VIDEO:
            {
                userEnableVideoData* data = reinterpret_cast<userEnableVideoData*>(wrapper->data);
                cout << "onUserEnableVideo uid:"<< data->uid << " enabled:" << (int)data->enabled <<endl;
                break;
            }
        default:
            break;
    }

    return true;
}

