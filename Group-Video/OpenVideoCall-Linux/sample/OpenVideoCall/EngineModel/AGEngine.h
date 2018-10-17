#pragma once
#include <cstdint>
#include <string>

namespace agora {
    namespace rtc {
        class IRtcEngineEventHandler;
        class IRtcEngine;
        class RtcEngineParameters;
    }
}

using namespace agora::rtc;
using std::string;

class AGEngine 
{
    public:
        AGEngine(IRtcEngineEventHandler* handler, const char* appId);
        ~AGEngine();

        const char* getSDKVersion();

        bool setLogFilePath(const char* logPath = NULL);

        bool joinChannel(const char* channelId, int uid);

        bool leaveChannel();

        const char* getChannelName();

        const char* getCallID();

        bool enableVideo(bool enable = true);

        bool enableLocalVideo(bool enable = true);

        bool enableAudio(bool enable = true);

        bool setVideoProfile(int videoProfile);

        bool setAudioProfile(int audioProfile, int audioScenario);

        bool muteLocalAudio(bool mute = true);

        bool muteLocalVideo(bool mute = true);

        bool muteRemoteVideo(int uid, bool mute);

        bool muteRemoteAudio(int uid, bool mute);

        bool enableAudioRecording(bool enable, const char* filePath);

        bool enableNetworkTest(bool bEnable);

        bool enableEchoTest(bool enable = true);

        bool setLogFilter(int type, const char* logPath);

        bool setEncryptionSecret(const char* key, int encryptType = 0);

        bool enableLocalRender(bool enable);

        int createMessageStream();

        bool sendChatMessage(int streamId, const char* chatMessage);
        
        bool enableWebSdkInteroperability(bool enable);

        bool setChannelProfile(int profile);

        bool release();

        IRtcEngine* getRtcEngine() { return m_agoraEngine;} 

    private:
        IRtcEngine*     m_agoraEngine;
        RtcEngineParameters* m_parameters;
};
