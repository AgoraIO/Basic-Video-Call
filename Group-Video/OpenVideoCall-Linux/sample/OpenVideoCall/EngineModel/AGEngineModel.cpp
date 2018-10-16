#include"AGEngineModel.h"
#include"AGEngine.h"
#include"AGCameraManager.h"
#include"AGEventDef.h"
#include"Controller/EngineController.h"

#include<iostream>
#include<string>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

AGEngineModel AGEngineModel::m_model;

AGEngineModel* AGEngineModel::Get() {
    return &m_model;
}

AGEngineModel::AGEngineModel() {

    registerHandler(MSG_OPEN,(handler_ptr)&AGEngineModel::onOpenMsg);
    registerHandler(MSG_CLOSE, (handler_ptr)&AGEngineModel::onCloseMsg);
    registerHandler(MSG_CONFIGURE, (handler_ptr)&AGEngineModel::onConfigureMsg);
    registerHandler(MSG_ENABLE_VIDEO, (handler_ptr)&AGEngineModel::onEnableVideoMsg);
    registerHandler(MSG_ENABLE_AUDIO, (handler_ptr)&AGEngineModel::onEnableAudioMsg);
    registerHandler(MSG_ENABLE_LOCAL_VIDEO, (handler_ptr)&AGEngineModel::onEnableLocalVideoMsg);
    registerHandler(MSG_MUTE_LOCAL_VIDEO, (handler_ptr)&AGEngineModel::onMuteLocalVideoMsg);
    registerHandler(MSG_MUTE_LOCAL_AUDIO, (handler_ptr)&AGEngineModel::onMuteLocalAudioMsg);
    registerHandler(MSG_PRINT_DEVICE_INFO, (handler_ptr)&AGEngineModel::onPrintDeviceInfoMsg);
    registerHandler(MSG_SET_CUR_CAMERA, (handler_ptr)&AGEngineModel::onSetCurCameraMsg);
    registerHandler(MSG_EXIT, (handler_ptr)&AGEngineModel::onExitMsg);
    registerHandler(MSG_MUTE_REMOTE, (handler_ptr)&AGEngineModel::onMuteRemote);

    m_engineEventHandler.setEventReceiver(this);
}

AGEngineModel::~AGEngineModel() {
    release();
}

void AGEngineModel::initialize() {
    if(!m_engine){
        m_engine = new AGEngine(&m_engineEventHandler, m_cfg.appId.c_str());
    }

    m_cameraMgr = new AGCameraManager();
}

bool AGEngineModel::onOpenMsg(void* msg) {
    cout << "AgoraRtcEngine:open" <<endl;

    m_engine->setChannelProfile(m_cfg.channelProfile);

    m_engine->enableVideo(m_cfg.enableVideo);

    m_engine->enableAudio(m_cfg.enableAudio);

    m_engine->enableLocalVideo(m_cfg.enableLocalVideo);

    m_engine->muteLocalVideo(m_cfg.muteLocalVideo);

    m_engine->muteLocalAudio(m_cfg.muteLocalAudio);

    m_engine->setVideoProfile(m_cfg.videoProfile);

    m_engine->setAudioProfile(m_cfg.audioProfile, m_cfg.audioScenario);

    m_engine->enableWebSdkInteroperability(m_cfg.enableWebSdkInteroperability);

    int ret = m_engine->joinChannel(m_cfg.channelId.c_str(), m_cfg.uid); 

    return ret;
}

bool AGEngineModel::onCloseMsg(void* msg) {
    cout << "AgoraRtcEngine:close" <<endl;

    m_cameraMgr->close();
    return m_engine->leaveChannel(); 
}

bool AGEngineModel::onEnableVideoMsg(void* msg) {
    bool enable = *(reinterpret_cast<bool*>(msg));

    if(enable != m_cfg.enableVideo) {
        m_cfg.enableVideo =  enable;
        m_engine->enableVideo(enable);
        cout << "AgoraRtcEngine: enable video: " << enable <<endl;
    } else {
        cout << "AgoraRtcEngine: already enabled video: " << enable <<endl;
    }

    return true;
}

bool AGEngineModel::onEnableLocalVideoMsg(void* msg) {
    bool enable = *(reinterpret_cast<bool*>(msg));

    if(enable != m_cfg.enableLocalVideo) {
        m_cfg.enableLocalVideo =  enable;
        m_engine->enableLocalVideo(enable);
        cout << "AgoraRtcEngine: enable local video: " << enable <<endl;
    } else {
        cout << "AgoraRtcEngine: already enabled local video: " << enable <<endl;
    }

    return true;
}

bool AGEngineModel::onEnableAudioMsg(void* msg) {
    bool enable = *(reinterpret_cast<bool*>(msg));

    if(enable != m_cfg.enableAudio) {
        m_cfg.enableAudio =  enable;
        m_engine->enableAudio(enable);
        cout << "AgoraRtcEngine: enable audio: " << enable <<endl;
    } else {
        cout << "AgoraRtcEngine: already enabled audio: " << enable <<endl;
    }

    return true;
}

bool AGEngineModel::onMuteLocalVideoMsg(void* msg) {
    bool mute = *(reinterpret_cast<bool*>(msg));

    if(mute != m_cfg.muteLocalVideo) {
        m_cfg.muteLocalVideo =  mute;
        m_engine->muteLocalVideo(m_cfg.muteLocalVideo);
        cout << "AgoraRtcEngine: mute local video: " << mute <<endl;
    } else {
        cout << "AgoraRtcEngine: already muted local video: " << mute <<endl;
    }

    return true;
}

bool AGEngineModel::onMuteLocalAudioMsg(void* msg) {
    bool mute = *(reinterpret_cast<bool*>(msg));

    if(mute != m_cfg.muteLocalAudio) {
        m_cfg.muteLocalAudio =  mute;
        m_engine->muteLocalAudio(m_cfg.muteLocalAudio);
        cout << "AgoraRtcEngine: mute local audio: " << mute <<endl;
    } else {
        cout << "AgoraRtcEngine: already muted local audio: " << mute <<endl;
    }

    return true;
}

bool AGEngineModel::onConfigureMsg(void* msg) {
    AppConfig* cfg = reinterpret_cast<AppConfig*>(msg);
    m_cfg = *cfg;

    cout << "model onConfigureMsg"<<endl;
    return true;
}

bool AGEngineModel::onPrintDeviceInfoMsg(void* msg) {
    cout << "model onPrintDeviceInfoMsg"<<endl;

    int cameraCnt = m_cameraMgr->getDeviceCount();
    cout <<"camera device number is:" << cameraCnt<< endl;
    cout <<"camera device list:" << endl;
    for(int i = 0; i < cameraCnt; ++i) {
        string deviceId;
        string deviceName;
        if(m_cameraMgr->getDevice(i , deviceName, deviceId)) {
            cout <<"device index: "<< i<<",  device name: "<< deviceName <<",  device Id: " << deviceId << endl;
        }
    }
    string curDeviceId;
    if(m_cameraMgr->getCurDeviceId(curDeviceId)) 
        cout <<"current camera device id is:" << curDeviceId << endl;

    return true;
}

bool AGEngineModel::onExitMsg(void* msg) {
    m_cameraMgr->close();
    m_engine->leaveChannel();
    m_engine->release();

    release();

    return true;
}

bool AGEngineModel::onSetCurCameraMsg(void* msg) {
    string* deviceId = reinterpret_cast<string*>(msg);
    if(!m_cameraMgr)
        return false;

    return m_cameraMgr->setCurDevice(deviceId->c_str());
}

bool AGEngineModel::onMuteRemote(void* msg) {
    muteRemoteMsg* muteMsg = reinterpret_cast<muteRemoteMsg*>(msg);
    if(muteMsg->isVideo)
        return m_engine->muteRemoteVideo(muteMsg->uid, muteMsg->mute);
    else
        return m_engine->muteRemoteAudio(muteMsg->uid, muteMsg->mute);
}

void AGEngineModel::release() {
    if(m_cameraMgr) {
        delete m_cameraMgr;
        m_cameraMgr = NULL;
    }

    if(m_engine) {
        delete m_engine;
        m_engine = NULL;
    }
}


void AGEngineModel::onEvent(int id, void* pData) {
    switch(id) {
        case EID_JOINCHANNEL_SUCCESS: 
            m_cameraMgr->create(m_engine->getRtcEngine());
            break;
        default:
            break;
    }
    eventWrapper wrapper;
    wrapper.id = id;
    wrapper.data = pData;
    
    if(m_controller)
        m_controller->sendMsg(this, MSG_RTC_EVENT, (void*)&wrapper);
}

