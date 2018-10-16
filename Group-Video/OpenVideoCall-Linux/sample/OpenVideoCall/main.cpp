#include"AgoraDefs.h"
#include"opt_parser.h"
#include"OpenVideoCallApp.h"

#include <iostream>
#include <sstream> 

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

int main(int argc, char * const argv[]) {

    opt_parser parser;

    uint32_t uid = 0;
    parser.add_long_opt("uid", &uid, "User Id default is 0/required", opt_parser::require_argu);

    string appId;
    parser.add_long_opt("appId", &appId, "App Id/required", opt_parser::require_argu);

    string channelId;
    parser.add_long_opt("channel", &channelId, "Channel Id/required", opt_parser::require_argu);

    uint32_t channelProfile = 0;
    parser.add_long_opt("channelProfile", &channelProfile, "channel profile:(0:COMMUNICATION),(1:broadcast) default is 0/option");

    uint32_t audioProfile = 0;
    parser.add_long_opt("audioProfile", &audioProfile, "audio profile:refer IAgoraRtcEngine.h/option");

    uint32_t audioScenario = 0;
    parser.add_long_opt("audioScenario", &audioScenario, "audio scenario:refer IAgoraRtcEngine.h/option");

    uint32_t videoProfile = 38;
    parser.add_long_opt("videoProfile", &videoProfile, "video profile:refer IAgoraRtcEngine.h/option");

    bool enableVideo = true;
    parser.add_long_opt("enableVideo", &enableVideo, "enable video/option");

    bool enableAudio = true;
    parser.add_long_opt("enableAudio", &enableAudio, "enable audio/option");

    bool muteLocalVideo = false;
    parser.add_long_opt("muteLocalVideo", &muteLocalVideo, "mute local video/option");

    bool muteLocalAudio = false;
    parser.add_long_opt("muteLocalAudio", &muteLocalAudio, "mute local audio/option");

    bool enableLocalVideo = true;
    parser.add_long_opt("enableLocalVideo", &enableLocalVideo, "enable local video/option");

    bool enableWebSdkInteroperability = false;
    parser.add_long_opt("enableWebSdkInteroperability", &enableWebSdkInteroperability, "enable web sdk interoperability/option");

    bool openVideoCall = false;
    parser.add_long_opt("openVideoCall", &openVideoCall, "open video call after launch app /option");

    if (!parser.parse_opts(argc, argv) || appId.empty() || channelId.empty()) {
        ostringstream sout;
        parser.print_usage(argv[0], sout);
        cout<<sout.str()<<endl;
        return -1;
    }

    AppConfig cfg;
    cfg.uid = uid;
    cfg.audioProfile = audioProfile; 
    cfg.audioScenario = audioScenario; 
    cfg.videoProfile = videoProfile; 
    cfg.enableVideo = enableVideo; 
    cfg.enableAudio = enableAudio; 
    cfg.muteLocalVideo = muteLocalVideo; 
    cfg.muteLocalAudio = muteLocalAudio;
    cfg.enableLocalVideo = enableLocalVideo; 
    cfg.appId = appId;
    cfg.channelId = channelId;
    cfg.channelProfile = channelProfile; 
    cfg.enableWebSdkInteroperability = enableWebSdkInteroperability;

    OpenVideoCallApp app;
    app.loadConfig(cfg);
    app.run(openVideoCall);

    return 0;
}
