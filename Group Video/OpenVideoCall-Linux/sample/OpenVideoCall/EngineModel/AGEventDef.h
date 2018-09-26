#pragma once
#include<cstdint>
#include<string>
#include"IAgoraRtcEngine.h"

using namespace agora::rtc;

typedef struct eventWrapper {
    uint32_t id;
    void* data;
} eventWrapper;

typedef struct joinChannelSuccessData {
    std::string channel;
    uint32_t uid;
    int elapsed;
} joinChannelSuccessData;

typedef struct rejoinChannelSuccessData {
    std::string channel;
    uint32_t uid;
    int elapsed;
} rejoinChannelSuccessData;

typedef struct warningData {
    std::string msg;
    int warn;
} warningData;

typedef struct errorData {
    std::string msg;
    int err;
} errorData;

typedef struct audioQualityData {
    uint32_t uid;
    int quality;
    unsigned short delay;
    unsigned short lost;
} audioQualityData;

typedef struct leaveChannelData {
    RtcStats stat;
} leaveChannelData;

typedef struct userJoinedData {
    uint32_t uid;
    int elapsed;
} userJoinedData;

typedef struct userOfflineData {
    uint32_t uid;
    USER_OFFLINE_REASON_TYPE reason;
} userOfflineData;

typedef struct userMuteAudioData {
    uint32_t uid;
    bool muted;
} userMuteAudioData;

typedef struct userMuteVideoData {
    uint32_t uid;
    bool muted;
} userMuteVideoData;

typedef struct userEnableVideoData {
    uint32_t uid;
    bool enabled;
} userEnableVideoData;

#define EID_JOINCHANNEL_SUCCESS			    0x00000001
#define EID_REJOINCHANNEL_SUCCESS		    0x00000002
#define EID_ERROR						    0x00000003
#define EID_AUDIO_QUALITY				    0x00000004
#define EID_AUDIO_TRANSPORT_QUALITY		`   0x00000005
#define EID_VUDIO_TRANSPORT_QUALITY		    0x00000006
#define EID_RECAP						    0x00000007
#define EID_CONNECTION_INTERRUPTED			0x00000008
#define EID_LEAVE_CHANNEL					0x00000009
#define EID_UPDATE_SESSION_STATS			0x0000000A
#define EID_USER_ENABLE_VIDEO				0x0000000B
#define EID_AUDIO_DEVICE_STATE_CHANGED		0x0000000C
#define EID_RTC_STAT        				0x0000000D
#define EID_FIRST_LOCAL_VIDEO_FRAME			0x0000000E
#define EID_FIRST_REMOTE_VIDEO_DECODED		0x0000000F
#define EID_FIRST_REMOTE_VIDEO_FRAME		0x00000010
#define EID_USER_JOINED						0x00000011
#define EID_USER_OFFLINE					0x00000012
#define EID_USER_MUTE_AUDIO					0x00000013
#define EID_USER_MUTE_VIDEO					0x00000014
#define EID_AUDIO_RECORDER_EXCEPTION		0x00000015
#define EID_LOCAL_VIDEO_STAT				0x00000016
#define EID_REMOTE_VIDEO_STAT				0x00000017
#define EID_CAMERA_READY					0x00000018
#define EID_CONNECTION_LOST					0x00000019
#define EID_VENDOR_MESSAGE					0x0000001A
#define EID_VIDEO_DEVICE_STATE_CHANGED		0x0000001B
#define EID_VIDEO_STOPPED					0x0000001C
#define EID_START_RCDSRV					0x0000001D
#define EID_STOP_RCDSRV						0x0000001E
#define EID_REFREASH_RCDSRV					0x0000001F
#define EID_APICALL_EXECUTED				0x00000020
#define EID_WARNING                         0x00000021


