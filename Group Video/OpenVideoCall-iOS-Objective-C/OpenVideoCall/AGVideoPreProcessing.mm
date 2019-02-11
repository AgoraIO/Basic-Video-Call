//
//  AGVideoPreProcessing.m
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import "AGVideoPreProcessing.h"
#import <AgoraRtcEngineKit/IAgoraRtcEngine.h>
#import <AgoraRtcEngineKit/IAgoraMediaEngine.h>

class AgoraVideoFrameObserver : public agora::media::IVideoFrameObserver
{
public:
    virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) override
    {
        int height = videoFrame.height;
        
        memset(videoFrame.uBuffer, 128, videoFrame.uStride*height/2);
        memset(videoFrame.vBuffer, 128, videoFrame.vStride*height/2);
        return true;
    }
    virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame) override
    {
        return true;
    }
};

static AgoraVideoFrameObserver s_videoFrameObserver;

@implementation AGVideoPreProcessing

+ (int) registerVideoPreprocessing: (AgoraRtcEngineKit*) kit
{
    if (!kit) {
        return -1;
    }
    
    agora::rtc::IRtcEngine* rtc_engine = (agora::rtc::IRtcEngine*)kit.getNativeHandle;
    agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
    mediaEngine.queryInterface(rtc_engine, agora::AGORA_IID_MEDIA_ENGINE);
    if (mediaEngine)
    {
        mediaEngine->registerVideoFrameObserver(&s_videoFrameObserver);
    }
    return 0;
}

+ (int) deregisterVideoPreprocessing: (AgoraRtcEngineKit*) kit
{
    if (!kit) {
        return -1;
    }
    
    agora::rtc::IRtcEngine* rtc_engine = (agora::rtc::IRtcEngine*)kit.getNativeHandle;
    agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
    mediaEngine.queryInterface(rtc_engine, agora::AGORA_IID_MEDIA_ENGINE);
    if (mediaEngine)
    {
        mediaEngine->registerVideoFrameObserver(NULL);
    }
    return 0;
}

@end
