#include <stdio.h>
#include <cstring>

#include "IAgoraRtcEngine.h"
#include "AGEngine.h"

AGEngine::AGEngine(IRtcEngineEventHandler* handler, const char* appId)
{
    m_agoraEngine = createAgoraRtcEngine(); 
    RtcEngineContext ctx;
    ctx.eventHandler = handler;
    ctx.appId = appId;
    m_agoraEngine->initialize(ctx);

    m_parameters = new RtcEngineParameters(m_agoraEngine);
}

AGEngine::~AGEngine()
{
    release();
}

const char* AGEngine::getSDKVersion()
{
    if(m_agoraEngine == NULL)
        return false;

    int buildNumber = 0;
    const char *engineVer = m_agoraEngine->getVersion(&buildNumber);

    return engineVer;
}

bool AGEngine::setLogFilePath(const char* logPath)
{
    RtcEngineParameters rep(*m_agoraEngine);

    string fileName = string(logPath) + "AgoraSdk.log";
    int ret = rep.setLogFile(fileName.c_str());

    return ret == 0 ? true : false;
}

bool AGEngine::joinChannel(const char* channelId, int uid)
{
    int ret = -1;
    if(m_agoraEngine) {
        ret = m_agoraEngine->joinChannel(NULL, channelId, NULL, uid);
        if(ret == 0) {
            AParameter msp(*m_agoraEngine);
            msp->setInt("che.video.local.camera_index", 0);
        }
    }

    return ret == 0 ? true : false;
}

bool AGEngine::setChannelProfile(int profile) {
    int ret = -1;
    if(profile){
        ret = m_agoraEngine->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
        if(ret == 0)
            ret = m_agoraEngine->setClientRole(CLIENT_ROLE_BROADCASTER);
    }
    else
        ret = m_agoraEngine->setChannelProfile(CHANNEL_PROFILE_COMMUNICATION);

    return ret == 0 ? true : false;
}


bool AGEngine::leaveChannel()
{
    //    m_agoraEngine->stopPreview();
    int ret = m_agoraEngine->leaveChannel();

    return ret == 0 ? true : false;
}

const char* AGEngine::getCallID()
{
    agora::util::AString uid;

    m_agoraEngine->getCallId(uid);

    return uid->c_str();
}

bool AGEngine::enableWebSdkInteroperability(bool enable)
{
    RtcEngineParameters rep(*m_agoraEngine);
    int ret = -1;
    ret = rep.enableWebSdkInteroperability(enable);

    return ret == 0 ? true : false;
}

bool AGEngine::enableVideo(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->enableVideo();
    else
        ret = m_agoraEngine->disableVideo();

    return ret == 0 ? true : false;
}

bool AGEngine::enableLocalVideo(bool enable)
{
    RtcEngineParameters rep(*m_agoraEngine);
    int ret = -1;
    ret = rep.enableLocalVideo(enable);

    return ret == 0 ? true : false;
}

bool AGEngine::enableAudio(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->enableAudio();
    else
        ret = m_agoraEngine->disableAudio();

    return ret == 0 ? true : false;
}

bool AGEngine::muteLocalAudio(bool mute)
{    
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteLocalAudioStream(mute);

    return ret == 0 ? true : false;
}

bool AGEngine::muteLocalVideo(bool mute)
{
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteLocalVideoStream(mute);

    return ret == 0 ? true : false;
}

bool AGEngine::enableNetworkTest(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->enableLastmileTest();
    else
        ret = m_agoraEngine->disableLastmileTest();

    return ret == 0 ? true : false;
}

bool AGEngine::enableEchoTest(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->startEchoTest();
    else
        ret = m_agoraEngine->stopEchoTest();

    return ret == 0 ? true : false;
}

bool AGEngine::setLogFilter(int type, const char* logPath)
{
    int ret = -1;
    RtcEngineParameters rep(*m_agoraEngine);

    ret = rep.setLogFilter((agora::LOG_FILTER_TYPE)type);

    ret |= rep.setLogFile(logPath);

    return ret == 0 ? true : false;
}

bool AGEngine::setEncryptionSecret(const char* key, int encryptType)
{
    switch (encryptType)
    {
        case 0:
            m_agoraEngine->setEncryptionMode("aes-128-xts");
            break;
        case 1:
            m_agoraEngine->setEncryptionMode("aes-256-xts");
            break;
        default:
            m_agoraEngine->setEncryptionMode("aes-128-xts");
            break;
    }
    int ret = m_agoraEngine->setEncryptionSecret(key);

    return ret == 0 ? true : false;
}

bool AGEngine::enableLocalRender(bool enable)
{
    int ret = 0;

    return ret == 0 ? true : false;
}

int AGEngine::createMessageStream()
{
    int dataStream = 0;
    m_agoraEngine->createDataStream(&dataStream, true, true);

    return dataStream;
}

bool AGEngine::sendChatMessage(int streamId, const char* chatMessage)
{
    int messageLen = strlen(chatMessage);

    int ret = m_agoraEngine->sendStreamMessage(streamId, chatMessage, messageLen);

    return ret == 0 ? true : false;
}

bool AGEngine::setVideoProfile(int videoProfile)
{
    int ret = m_agoraEngine->setVideoProfile((VIDEO_PROFILE_TYPE)videoProfile, false);

    return ret == 0 ? true : false;
}

bool AGEngine::setAudioProfile(int audioProfile, int audioScenario)
{
    int ret = m_agoraEngine->setAudioProfile((AUDIO_PROFILE_TYPE)audioProfile, (AUDIO_SCENARIO_TYPE)audioScenario);

    return ret == 0 ? true : false;
}

bool AGEngine::muteRemoteVideo(int uid, bool mute){
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteRemoteVideoStream(uid, mute);

    return ret == 0 ? true : false;
}

bool AGEngine::muteRemoteAudio(int uid, bool mute){
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteRemoteAudioStream(uid, mute);

    return ret == 0 ? true : false;
}

bool AGEngine::release()
{
    if(m_agoraEngine != NULL) {
        m_agoraEngine->release();
        m_agoraEngine = NULL;
    }

    if(m_parameters) {
        delete m_parameters;
        m_parameters = NULL;
    }

    return true;
}

