#include "AGEngineEventHandler.h"
#include "AGEventDef.h"
#include "IAGEventReceiver.h"
#include <cstring>
#include <iostream>

AGEngineEventHandler::AGEngineEventHandler()
{
    m_receiver = NULL;
}

AGEngineEventHandler::~AGEngineEventHandler()
{
}

void AGEngineEventHandler::setEventReceiver(IAGEventReceiver* receiver)
{
    m_receiver = receiver;
}

void AGEngineEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    if(m_receiver != NULL) {
        joinChannelSuccessData data;
        data.channel = std::string(channel);
        data.elapsed = elapsed;
        data.uid = uid;
        m_receiver->onEvent(EID_JOINCHANNEL_SUCCESS, &data);
    }
}

void AGEngineEventHandler::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    if(m_receiver != NULL) {
        rejoinChannelSuccessData data;
        data.channel = std::string(channel);
        data.elapsed = elapsed;
        data.uid = uid;
        m_receiver->onEvent(EID_REJOINCHANNEL_SUCCESS, &data);
    }
}

void AGEngineEventHandler::onWarning(int warn, const char* msg)
{
    if(m_receiver != NULL) {
        warningData data;
        data.msg = (msg == NULL)? "on warning" : std::string(msg); 
        data.warn = warn;
        m_receiver->onEvent(EID_WARNING, &data);
    }
}

void AGEngineEventHandler::onError(int err, const char* msg)
{
    if(m_receiver != NULL) {
        errorData data;
        data.err = err;
        data.msg = (msg == NULL)? "on error" : std::string(msg); 
        m_receiver->onEvent(EID_ERROR, &data);
    }
}

void AGEngineEventHandler::onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost)
{
    if(m_receiver != NULL) {
        audioQualityData data;
        data.uid = uid;
        data.quality = quality;
        data.delay = delay;
        data.lost = lost;
        m_receiver->onEvent(EID_AUDIO_QUALITY, &data); 
    }
} 

void AGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
    if(m_receiver != NULL) {
        leaveChannelData data;
        data.stat = stat;
        m_receiver->onEvent(EID_LEAVE_CHANNEL, &data);
    }
}

void AGEngineEventHandler::onRtcStats(const RtcStats& stat)
{
    if(m_receiver != NULL) 
        m_receiver->onEvent(EID_RTC_STAT, NULL);
}

void AGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
    if(m_receiver != NULL) {
        userJoinedData data;
        data.uid = uid;
        data.elapsed = elapsed;
        m_receiver->onEvent(EID_USER_JOINED, &data);
    }
}

void AGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
    if(m_receiver != NULL) {
        userOfflineData data;
        data.uid = uid;
        data.reason = reason;
        m_receiver->onEvent(EID_USER_OFFLINE, &data);
    }
}

void AGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{
    if(m_receiver != NULL) {
        userMuteAudioData data;
        data.uid = uid;
        data.muted = muted;
        m_receiver->onEvent(EID_USER_MUTE_AUDIO, &data);
    }
}

void AGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{
    if(m_receiver != NULL) {
        userMuteVideoData data;
        data.uid = uid;
        data.muted = muted;
        m_receiver->onEvent(EID_USER_MUTE_VIDEO, &data);
    }
}

void AGEngineEventHandler::onCameraReady()
{
    if(m_receiver != NULL)
        m_receiver->onEvent(EID_CAMERA_READY, NULL);
}

void AGEngineEventHandler::onConnectionLost()
{
    if(m_receiver != NULL)
        m_receiver->onEvent(EID_CONNECTION_LOST, NULL);
}

void AGEngineEventHandler::onConnectionInterrupted()
{
    if(m_receiver != NULL)
        m_receiver->onEvent(EID_CONNECTION_INTERRUPTED, NULL);
}

void AGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{
    if(m_receiver != NULL) {
        userEnableVideoData data;
        data.uid = uid;
        data.enabled = enabled;
        m_receiver->onEvent(EID_USER_ENABLE_VIDEO, &data);
    }
}

