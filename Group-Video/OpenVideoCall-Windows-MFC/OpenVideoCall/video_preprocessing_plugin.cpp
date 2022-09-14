#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <cstdint>
#include "video_preprocessing_plugin.h"

static void LogMessage(char *msg)
{
  OutputDebugStringA(msg);
}

class AgoraAudioFrameObserver : public agora::media::IAudioFrameObserver
{
public:

  // Í¨¹ý IAudioFrameObserver ¼Ì³Ð
  virtual bool onRecordAudioFrame(const char* channelId, AudioFrame& audioFrame) override
  {
	  char msg[256];
	  sprintf_s(msg, "%s: samples=%d, channel=%d, fs=%d\n", __FUNCTION__, audioFrame.samplesPerChannel, audioFrame.channels, audioFrame.samplesPerSec);
	  LogMessage(msg);
	  return true;
  }
  virtual bool onPlaybackAudioFrame(const char* channelId, AudioFrame& audioFrame) override
  {
	  char msg[256];
	  sprintf_s(msg, "%s: samples=%d, channel=%d, fs=%d\n", __FUNCTION__, audioFrame.samplesPerChannel, audioFrame.channels, audioFrame.samplesPerSec);
	  LogMessage(msg);
	  return true;
  }
  virtual bool onMixedAudioFrame(const char* channelId, AudioFrame& audioFrame) override
  {
	  char msg[256];
	  sprintf_s(msg, "%s: samples=%d, channel=%d, fs=%d\n", __FUNCTION__, audioFrame.samplesPerChannel, audioFrame.channels, audioFrame.samplesPerSec);
	  LogMessage(msg);
	  return true;
  }
  virtual bool onPlaybackAudioFrameBeforeMixing(const char* channelId, uid_t uid, AudioFrame& audioFrame) override
  {
	  char msg[256];
	  sprintf_s(msg, "%s: uid=%u, samples=%d, channel=%d, fs=%d\n", __FUNCTION__, uid, audioFrame.samplesPerChannel, audioFrame.channels, audioFrame.samplesPerSec);
	  LogMessage(msg);
	  return true;
  }

  virtual int getObservedAudioFramePosition() override
  {
      return agora::media::IAudioFrameObserver::AUDIO_FRAME_POSITION_RECORD || agora::media::IAudioFrameObserver::AUDIO_FRAME_POSITION_PLAYBACK;
  }
  virtual AudioParams getPlaybackAudioParams() override
  {
      return AudioParams();
  }
  virtual AudioParams getRecordAudioParams() override
  {
      return AudioParams();
  }
  virtual AudioParams getMixedAudioParams() override
  {
      return AudioParams();
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

  virtual bool onRenderVideoFrame(const char* channelId, uid_t remoteUid, VideoFrame& videoFrame) override
  {
	  char msg[256];
	  sprintf_s(msg, 256, "%s: uid=%u, width=%d, height=%d\n", __FUNCTION__, remoteUid, videoFrame.width, videoFrame.height);
	  LogMessage(msg);
	  return true;
  }

  virtual bool onPreEncodeVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  virtual bool onSecondaryCameraCaptureVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  virtual bool onSecondaryPreEncodeCameraVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  virtual bool onScreenCaptureVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  virtual bool onPreEncodeScreenVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  virtual bool onMediaPlayerVideoFrame(VideoFrame& videoFrame, int mediaPlayerId) override
  {
      return false;
  }
  virtual bool onSecondaryScreenCaptureVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  virtual bool onSecondaryPreEncodeScreenVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
  
  virtual bool onTranscodedVideoFrame(VideoFrame& videoFrame) override
  {
      return false;
  }
};

static AgoraAudioFrameObserver s_audioFrameObserver;
static AgoraVideoFrameObserver s_videoFrameObserver;

int load_preprocessing_plugin(agora::rtc::IRtcEngine* engine)
{
	agora::util::AutoPtr<agora::media::IMediaEngine> mediaEngine;
	mediaEngine.queryInterface(engine, INTERFACE_ID_TYPE::AGORA_IID_MEDIA_ENGINE);
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
  mediaEngine.queryInterface(engine, INTERFACE_ID_TYPE::AGORA_IID_MEDIA_ENGINE);
  if (mediaEngine)
  {
    mediaEngine->registerAudioFrameObserver(NULL);
    mediaEngine->registerVideoFrameObserver(NULL);
  }
  return 0;
}
