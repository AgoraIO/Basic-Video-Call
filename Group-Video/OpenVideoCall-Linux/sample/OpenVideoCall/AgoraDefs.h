#pragma once
#include<cstdint>
#include<string>

typedef struct AppConfig {
    uint32_t uid;
    uint32_t channelProfile;
    uint32_t videoProfile;
    uint32_t audioProfile;
    uint32_t audioScenario;
    bool enableVideo;
    bool enableAudio;
    bool muteLocalVideo;
    bool muteLocalAudio;
    bool enableLocalVideo;
    bool enableWebSdkInteroperability;

    std::string appId;
    std::string channelId;
}AppConfig;

typedef struct muteRemoteMsg {
    bool isVideo;
    bool mute;
    uint32_t uid;
}muteRemoteMsg;
 

#define MSG_OPEN 0x1

#define MSG_CLOSE 0x2

#define MSG_CONFIGURE 0x3

#define MSG_ENABLE_VIDEO 0x4

#define MSG_ENABLE_AUDIO 0x5

#define MSG_MUTE_LOCAL_VIDEO 0x6

#define MSG_MUTE_LOCAL_AUDIO 0x7

#define MSG_PRINT_DEVICE_INFO 0x8

#define MSG_SET_CUR_CAMERA 0x9

#define MSG_EXIT 0xA

#define MSG_RTC_EVENT 0xB

#define MSG_ENABLE_LOCAL_VIDEO 0xC

#define MSG_MUTE_REMOTE 0xD
