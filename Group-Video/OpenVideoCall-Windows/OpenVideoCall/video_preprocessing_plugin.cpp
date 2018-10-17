#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <cstdint>

static void LogMessage(char *msg)
{
  OutputDebugStringA(msg);
}

class AgoraAudioFrameObserver : public agora::media::IAudioFrameObserver
{
public:
  virtual bool onRecordAudioFrame(AudioFrame& audioFrame) override
  {
    char msg[256];
    sprintf_s(msg, 256, "%s: samples=%d, channel=%d, fs=%d\n", __FUNCTION__, audioFrame.samples, audioFrame.channels, audioFrame.samplesPerSec);
    LogMessage(msg);
    return true;
  }
  virtual bool onPlaybackAudioFrame(AudioFrame& audioFrame) override
  {
    char msg[256];
    sprintf_s(msg, "%s: samples=%d, channel=%d, fs=%d\n", __FUNCTION__, audioFrame.samples, audioFrame.channels, audioFrame.samplesPerSec);
    LogMessage(msg);
    return true;
  }
  virtual bool onMixedAudioFrame(AudioFrame& audioFrame)
  {
	  char msg[256];
	  sprintf_s(msg, "%s: samples=%d, channel=%d, fs=%d\n", __FUNCTION__, audioFrame.samples, audioFrame.channels, audioFrame.samplesPerSec);
	  LogMessage(msg);
	  return true;
  }
  virtual bool onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame) override
  {
    char msg[256];
    sprintf_s(msg, "%s: uid=%u, samples=%d, channel=%d, fs=%d\n", __FUNCTION__, uid, audioFrame.samples, audioFrame.channels, audioFrame.samplesPerSec);
    LogMessage(msg);
    return true;
  }
};

class AgoraVideoFrameObserver : public agora::media::IVideoFrameObserver
{
public:
  virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) override
  {
    char msg[256];
    sprintf_s(msg, 256, "%s: width=%d, height=%d\n", __FUNCTION__, videoFrame.width, videoFrame.height);
    LogMessage(msg);

    int width = videoFrame.width;
    int height = videoFrame.height;

    memset(videoFrame.uBuffer, 128, videoFrame.uStride*height / 2);
    memset(videoFrame.vBuffer, 128, videoFrame.vStride*height / 2);
    return true;
  }
  virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame) override
  {
    char msg[256];
    sprintf_s(msg, 256, "%s: uid=%u, width=%d, height=%d\n", __FUNCTION__, uid, videoFrame.width, videoFrame.height);
    LogMessage(msg);
    return true;
  }
};

static AgoraAudioFrameObserver s_audioFrameObserver;
static AgoraVideoFrameObserver s_videoFrameObserver;

int load_preprocessing_plugin(agora::rtc::IRtcEngine* engine)
{
  agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
  mediaEngine.queryInterface(engine, agora::AGORA_IID_MEDIA_ENGINE);
  if (mediaEngine)
  {
    mediaEngine->registerAudioFrameObserver(&s_audioFrameObserver);
    mediaEngine->registerVideoFrameObserver(&s_videoFrameObserver);
  }
  return 0;
}

int unload_preprocessing_plugin(agora::rtc::IRtcEngine* engine)
{
  agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
  mediaEngine.queryInterface(engine, agora::AGORA_IID_MEDIA_ENGINE);
  if (mediaEngine)
  {
    mediaEngine->registerAudioFrameObserver(NULL);
    mediaEngine->registerVideoFrameObserver(NULL);
  }
  return 0;
}
