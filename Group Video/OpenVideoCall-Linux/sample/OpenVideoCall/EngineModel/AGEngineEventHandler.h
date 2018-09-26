#pragma once
#include"IAgoraRtcEngine.h"

using namespace agora::rtc;

class IAGEventReceiver;

class AGEngineEventHandler :
    public IRtcEngineEventHandler
{
    public:
        AGEngineEventHandler();
        ~AGEngineEventHandler();

        void setEventReceiver(IAGEventReceiver* receiver = NULL);

        virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
        virtual void onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
        virtual void onWarning(int warn, const char* msg);
        virtual void onError(int err, const char* msg);
        virtual void onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost);
        virtual void onLeaveChannel(const RtcStats& stat);
        virtual void onRtcStats(const RtcStats& stat);
        virtual void onUserJoined(uid_t uid, int elapsed);
        virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason);
        virtual void onUserMuteAudio(uid_t uid, bool muted);
        virtual void onUserMuteVideo(uid_t uid, bool muted);
        virtual void onCameraReady();
        virtual void onConnectionLost();
        virtual void onConnectionInterrupted();
        virtual void onUserEnableVideo(uid_t uid, bool enabled);
    private:
        IAGEventReceiver*        m_receiver;
};

