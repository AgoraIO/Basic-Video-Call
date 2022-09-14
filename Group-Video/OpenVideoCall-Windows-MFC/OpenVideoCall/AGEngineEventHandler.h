#pragma once

using namespace agora::rtc;

class CAGEngineEventHandler :
	public IRtcEngineEventHandler
{
public:
	CAGEngineEventHandler(void) ;
	~CAGEngineEventHandler(void) ;

	void SetMsgReceiver(HWND hWnd = NULL) ;
	HWND GetMsgReceiver() {return m_hMainWnd;};

	virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override;
	virtual void onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override;
	virtual void onError(int err, const char* msg) override;
	virtual void onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost) override;
	virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume) override;
	
	virtual void onLeaveChannel(const RtcStats& stat) override;
	virtual void onRtcStats(const RtcStats& stat) override;
	
	virtual void onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) override;
	virtual void onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState) override;

	virtual void onLastmileQuality(int quality) override;
	virtual void onFirstLocalVideoFrame(VIDEO_SOURCE_TYPE source, int width, int height, int elapsed) override;
	virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed) override;
	virtual void onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed) override;
	virtual void onUserJoined(uid_t uid, int elapsed) override;
	virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override;
	virtual void onUserMuteAudio(uid_t uid, bool muted) override;
	virtual void onUserMuteVideo(uid_t uid, bool muted) override;
	virtual void onApiCallExecuted(int err, const char* api, const char* result) override;

	virtual void onStreamMessage(uid_t userId, int streamId, const char* data, size_t length, uint64_t sentTs) override;
	
	virtual void onLocalVideoStats(VIDEO_SOURCE_TYPE source, const LocalVideoStats& stats) override;
	virtual void onRemoteVideoStats(const RemoteVideoStats& stats) override;
	virtual void onCameraReady() override;
	virtual void onVideoStopped() override;
	virtual void onConnectionLost() override;
	virtual void onConnectionInterrupted() override;

	virtual void onUserEnableVideo(uid_t uid, bool enabled) override;

	virtual void onLocalVideoStateChanged(VIDEO_SOURCE_TYPE source, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error) override;

private:
	HWND		m_hMainWnd;
};
