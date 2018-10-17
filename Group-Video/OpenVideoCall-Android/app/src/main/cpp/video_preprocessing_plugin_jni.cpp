#include <jni.h>
#include <android/log.h>
#include <cstring>
#include <agora/AgoraBase.h>

#include "agora/IAgoraRtcEngine.h"
#include "agora/IAgoraMediaEngine.h"

#include "video_preprocessing_plugin_jni.h"

class AgoraVideoFrameObserver : public agora::media::IVideoFrameObserver
{
public:
    virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) override
    {
        int width = videoFrame.width;
        int height = videoFrame.height;

        memset(videoFrame.uBuffer, 128, videoFrame.uStride * height / 2);
        memset(videoFrame.vBuffer, 128, videoFrame.vStride * height / 2);

        return true;
	}

    virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame) override
    {
        return true;
    }
};

static AgoraVideoFrameObserver s_videoFrameObserver;
static agora::rtc::IRtcEngine* rtcEngine = NULL;

#ifdef __cplusplus
extern "C" {
#endif

int __attribute__((visibility("default"))) loadAgoraRtcEnginePlugin(agora::rtc::IRtcEngine* engine)
{
    __android_log_print(ANDROID_LOG_ERROR, "plugin", "plugin loadAgoraRtcEnginePlugin");
    rtcEngine = engine;
    return 0;
}

void __attribute__((visibility("default"))) unloadAgoraRtcEnginePlugin(agora::rtc::IRtcEngine* engine)
{
    __android_log_print(ANDROID_LOG_ERROR, "plugin", "plugin unloadAgoraRtcEnginePlugin");
    rtcEngine = NULL;
}

JNIEXPORT void JNICALL Java_io_agora_propeller_preprocessing_VideoPreProcessing_enablePreProcessing
  (JNIEnv *env, jobject obj, jboolean enable)
{
    if (!rtcEngine)
        return;
    agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
    mediaEngine.queryInterface(rtcEngine, agora::AGORA_IID_MEDIA_ENGINE);
    if (mediaEngine) {
        if (enable) {
            mediaEngine->registerVideoFrameObserver(&s_videoFrameObserver);
        } else {
            mediaEngine->registerVideoFrameObserver(NULL);
        }
    }
}

#ifdef __cplusplus
}
#endif
