#pragma once
#include"Controller/MsgHandler.h"

using std::string;
using std::stringstream;

struct AppConfig;

class CommandLineView : public MsgHandler{
    public:
        CommandLineView();
        ~CommandLineView();

        void run(bool openVideoCall);
        void configure(const AppConfig& config);

    private:

        bool onCommand(const string& cmd, stringstream& params);

        bool open(); 

        bool close(); 

        bool enableAudio(bool enable); 

        bool enableVideo(bool enable);

        bool muteLocalAudio(bool mute); 

        bool muteLocalVideo(bool mute);

        bool muteRemoteAudio(uint32_t uid, bool mute);

        bool muteRemoteVideo(uint32_t uid, bool mute); 

        bool printDeviceInfo(); 

        bool enableLocalVideo(bool enable);

        bool setCurCamera(const string& cameraId); 

        bool exit(); 

        bool onRtcEventMsg(void* msg);

   private:
        bool m_printCallback;

};
