#include "StdAfx.h"
#include "AGEngineEventHandler.h"
#include "AGEventDef.h"

CAGEngineEventHandler::CAGEngineEventHandler(void)
{
}

CAGEngineEventHandler::~CAGEngineEventHandler(void)
{
}

void CAGEngineEventHandler::SetMsgReceiver(HWND hWnd)
{
	m_hMainWnd = hWnd;
}

/**
 onJoinChannelSuccess: Occurs when a user joins a channel.
 
 This callback notifies the application that a user joins a specified channel when the application calls the joinChannel method.
 
 The channel name assignment is based on channelName specified in the joinChannel method.
 
 If the uid is not specified in the joinChannel method, the server automatically assigns a uid.
 
 Parameters
 @param channel	Pointer to the channel name.
 @param uid	User ID of the user joining the channel.
 @param elapsed	Time elapsed (ms) from the user calling the joinChannel method until the SDK triggers this callback.
*/
void CAGEngineEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_JOINCHANNEL_SUCCESS lpData = new AGE_JOINCHANNEL_SUCCESS;

	int nChannelLen = strlen(channel) + 1;
	lpData->channel = new char[nChannelLen];
	lpData->uid = uid;
	lpData->elapsed = elapsed;

	strcpy_s(lpData->channel, nChannelLen, channel);
	::PostMessage(m_hMainWnd, WM_MSGID(EID_JOINCHANNEL_SUCCESS), (WPARAM)lpData, 0);
}

/**
Occurs when a user rejoins the channel after disconnection due to network problems.

When a user loses connection with the server because of network problems, the SDK 
automatically tries to reconnect and triggers this callback upon reconnection.

Parameters
@param channel	Pointer to the channel name.
@param uid	User ID of the user rejoining the channel.
@param elapsed	Time elapsed (ms) from starting to reconnect until the SDK triggers this callback.
*/
void CAGEngineEventHandler::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_REJOINCHANNEL_SUCCESS lpData = new AGE_REJOINCHANNEL_SUCCESS;

	int nChannelLen = strlen(channel) + 1;
	lpData->channel = new char[nChannelLen];
	lpData->uid = uid;
	lpData->elapsed = elapsed;

	strcpy_s(lpData->channel, nChannelLen, channel);

	::PostMessage(m_hMainWnd, WM_MSGID(EID_REJOINCHANNEL_SUCCESS), (WPARAM)lpData, 0);
}

/**
Reports a warning during SDK runtime.

In most cases, the application can ignore the warning reported by the SDK because 
the SDK can usually fix the issue and resume running. For example, when losing 
connection with the server, the SDK may report WARN_LOOKUP_CHANNEL_TIMEOUT and 
automatically try to reconnect.

Parameters
@param warn	Warning code: WARN_CODE_TYPE. 
        See: https://docs.agora.io/en/Video/API%20Reference/cpp/namespaceagora.html#a32d042123993336be6646469da251b21
@param msg	Pointer to the warning message.
*/
void CAGEngineEventHandler::onWarning(int warn, const char* msg)
{
	CString str;

	str = _T("onWarning");
}

/**
Reports an error during SDK runtime.

In most cases, the SDK cannot fix the issue and resume running. The SDK requires the application to take action or informs the user about the issue.

For example, the SDK reports an ERR_START_CALL error when failing to initialize a call. The application informs the user that the call initialization failed and invokes the leaveChannel method to leave the channel.

Parameters
@param err	Error code: ERROR_CODE_TYPE.
@param msg	Pointer to the error message.

*/
void CAGEngineEventHandler::onError(int err, const char* msg)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_ERROR lpData = new AGE_ERROR;

	int nMsgLen = 0;

	// attention: the pointer of msg maybe NULL!!!
	if(msg != NULL) {
		nMsgLen = strlen(msg) + 1;
		lpData->msg = new char[nMsgLen];
		strcpy_s(lpData->msg, nMsgLen, msg);
	}
	else
		lpData->msg = NULL;
	
	lpData->err = err;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_ERROR), (WPARAM)lpData, 0);
}

/**
Reports the statistics of the audio stream from each remote user/host.

Deprecated as of v2.3.2. Use the onRemoteAudioStats callback instead.

The SDK triggers this callback once every two seconds to report the audio quality 
of each remote user/host sending an audio stream. If a channel has multiple 
users/hosts sending audio streams, the SDK triggers this callback as many times.

Parameters
@param uid	User ID of the speaker.
@param quality	Audio quality of the user: QUALITY_TYPE.
@param delay	Time delay (ms) of sending the audio packet from the sender to the 
    receiver, including the time delay of audio sampling pre-processing, transmission, 
    and the jitter buffer.
@param lost	Packet loss rate (%) of the audio packet sent from the sender to the receiver.
*/
void CAGEngineEventHandler::onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_AUDIO_QUALITY lpData = new AGE_AUDIO_QUALITY;

	lpData->uid = uid;
	lpData->quality = quality;
	lpData->delay = delay;
	lpData->lost = lost;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_AUDIO_QUALITY), (WPARAM)lpData, 0);
}


/**
Reports which users are speaking, the speakers' volume and whether the local user 
is speaking.

This callback reports the IDs and volumes of the loudest speakers (at most 3 users) 
at the moment in the channel, and whether the local user is speaking.

By default, this callback is disabled. You can enable it by calling the 
enableAudioVolumeIndication method. Once enabled, this callback is triggered at the 
set interval, regardless of whether a user speaks or not.

The SDK triggers two independent onAudioVolumeIndication callbacks at one time,
which separately report the volume information of the local user and all the remote 
speakers. For more information, see the online documentation.

Parameters
@param speakers	A pointer to AudioVolumeInfo:
@param speakerNumber	Total number of speakers. The value range is [0, 3].
*/
void CAGEngineEventHandler::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_AUDIO_VOLUME_INDICATION lpData = new AGE_AUDIO_VOLUME_INDICATION;

	lpData->speakers = new AudioVolumeInfo[speakerNumber];
	memcpy(lpData->speakers, speakers, speakerNumber*sizeof(AudioVolumeInfo));
	lpData->speakerNumber = speakerNumber;
	lpData->totalVolume = totalVolume;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_AUDIO_VOLUME_INDICATION), (WPARAM)lpData, 0);
}

/**
Occurs when a user leaves the channel.

This callback notifies the application that a user leaves the channel when the application calls the leaveChannel method.

The application retrieves information, such as the call duration and statistics.

Parameters
@param stats	Pointer to the statistics of the call: RtcStats.
*/
void CAGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_LEAVE_CHANNEL lpData = new AGE_LEAVE_CHANNEL;

	memcpy(&lpData->rtcStat, &stat, sizeof(RtcStats));

	::PostMessage(m_hMainWnd, WM_MSGID(EID_LEAVE_CHANNEL), (WPARAM)lpData, 0);
}

/**
Reports the statistics of the current call.

The SDK triggers this callback once every two seconds after the user joins the channel.

Parameters
@param stats	Statistics of the IRtcEngine: RtcStats.
*/
void CAGEngineEventHandler::onRtcStats(const RtcStats& stat)
{
	CString str;

	str = _T("stat");
}


/**
    Reports a media engine event. (Deprecated)
*/
void CAGEngineEventHandler::onMediaEngineEvent(int evt)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_MEDIA_ENGINE_EVENT lpData = new AGE_MEDIA_ENGINE_EVENT;

	lpData->evt = evt;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_MEDIA_ENGINE_EVENT), (WPARAM)lpData, 0);
}

/**
Occurs when the audio device state changes.

This callback notifies the application that the system's audio device state is 
changed. For example, a headset is unplugged from the device.

Parameters
@param deviceId	Pointer to the device ID.
@param deviceType	Device type: MEDIA_DEVICE_TYPE.
@param deviceState	Device state: MEDIA_DEVICE_STATE_TYPE.
*/
void CAGEngineEventHandler::onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_AUDIO_DEVICE_STATE_CHANGED lpData = new AGE_AUDIO_DEVICE_STATE_CHANGED;

	int nDeviceIDLen = strlen(deviceId) + 1;

	lpData->deviceId = new char[nDeviceIDLen];

	strcpy_s(lpData->deviceId, nDeviceIDLen, deviceId);
	lpData->deviceType = deviceType;
	lpData->deviceState = deviceState;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_AUDIO_DEVICE_STATE_CHANGED), (WPARAM)lpData, 0);
}

/**
Occurs when the video device state changes.

Note
On a Windows device with an external camera for video capturing, the video 
disables once the external camera is unplugged.
Parameters
@param deviceId	Pointer to the device ID of the video device that changes state.
@param deviceType	Device type: MEDIA_DEVICE_TYPE.
@param deviceState	Device state: MEDIA_DEVICE_STATE_TYPE.
*/
void CAGEngineEventHandler::onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_VIDEO_DEVICE_STATE_CHANGED lpData = new AGE_VIDEO_DEVICE_STATE_CHANGED;

	int nDeviceIDLen = strlen(deviceId) + 1;

	lpData->deviceId = new char[nDeviceIDLen];

	strcpy_s(lpData->deviceId, nDeviceIDLen, deviceId);
	lpData->deviceType = deviceType;
	lpData->deviceState = deviceState;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_VIDEO_DEVICE_STATE_CHANGED), (WPARAM)lpData, 0);
}

/**
Reports the last mile network quality of the local user once every two seconds 
before the user joins the channel.

Last mile refers to the connection between the local device and Agora's edge server. 
After the application calls the enableLastmileTest method, this callback reports
once every two seconds the uplink and downlink last mile network conditions of the
local user before the user joins the channel.

Parameters
@param quality	The last mile network quality: QUALITY_TYPE.
*/
void CAGEngineEventHandler::onLastmileQuality(int quality)
{
	if (m_hMainWnd == NULL)
		return;

    LPAGE_LASTMILE_QUALITY lpData = new AGE_LASTMILE_QUALITY;

	lpData->quality = quality;

    ::PostMessage(m_hMainWnd, WM_MSGID(EID_LASTMILE_QUALITY), (WPARAM)lpData, 0);
}


/**
Occurs when the first local video frame is displayed/rendered on the local video view.

Parameters
@param width	Width (px) of the first local video frame.
@param height	Height (px) of the first local video frame.
@param elapsed	Time elapsed (ms) from the local user calling the joinChannel method 
    until the SDK triggers this callback. If you call the startPreview method before 
    calling the joinChannel method, then elapsed is the time elapsed from calling the 
    startPreview method until the SDK triggers this callback.
*/
void CAGEngineEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = new AGE_FIRST_LOCAL_VIDEO_FRAME;

	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), (WPARAM)lpData, 0);

}

/**
This callback is triggered in either of the following scenarios:

The remote user joins the channel and sends the video stream.
The remote user stops sending the video stream and re-sends it after 15 seconds. Reasons for such an interruption include:
The remote user leaves the channel.
The remote user drops offline.
The remote user calls the muteLocalVideoStream method to stop sending the video stream.
The remote user calls the disableVideo method to disable video.
The application can configure the user view settings in this callback.

Parameters
@param uid	User ID of the remote user sending the video stream.
@param width	Width (px) of the video stream.
@param height	Height (px) of the video stream.
@param elapsed	Time elapsed (ms) from the local user calling the joinChannel method until the SDK triggers this callback.
*/
void CAGEngineEventHandler::onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = new AGE_FIRST_REMOTE_VIDEO_DECODED;

	lpData->uid = uid;
	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), (WPARAM)lpData, 0);
}

/**
Occurs when the first remote video frame is rendered.

The SDK triggers this callback when the first frame of the remote video is displayed in the user's video window. The application can retrieve the time elapsed from a user joining the channel until the first video frame is displayed.

Parameters
@param uid	User ID of the remote user sending the video stream.
@param width	Width (px) of the video frame.
@param height	Height (px) of the video stream.
@param elapsed	Time elapsed (ms) from the local user calling the joinChannel method until the SDK triggers this callback.

*/
void CAGEngineEventHandler::onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_FIRST_REMOTE_VIDEO_FRAME lpData = new AGE_FIRST_REMOTE_VIDEO_FRAME;

	lpData->uid = uid;
	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_REMOTE_VIDEO_FRAME), (WPARAM)lpData, 0);
}

/**
Occurs when a remote user (Communication)/ host (Live Broadcast) joins the channel.

Communication profile: This callback notifies the application that another user joins the channel. If other users are already in the channel, the SDK also reports to the application on the existing users.
Live-broadcast profile: This callback notifies the application that the host joins the channel. If other hosts are already in the channel, the SDK also reports to the application on the existing hosts. We recommend limiting the number of hosts to 17.
The SDK triggers this callback under one of the following circumstances:

A remote user/host joins the channel by calling the joinChannel method.
A remote user switches the user role to the host by calling the setClientRole method after joining the channel.
A remote user/host rejoins the channel after a network interruption.
The host injects an online media stream into the channel by calling the addInjectStreamUrl method.
Note
In the Live-broadcast profile:
The host receives this callback when another host joins the channel.
The audience in the channel receives this callback when a new host joins the channel.
When a web application joins the channel, the SDK triggers this callback as long as the web application publishes streams.
Parameters
@param uid	User ID of the user or host joining the channel.
@param elapsed	Time delay (ms) from the local user calling the joinChannel method until the SDK triggers this callback.
*/
void CAGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_JOINED lpData = new AGE_USER_JOINED;

	lpData->uid = uid;
	lpData->elapsed = elapsed;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_JOINED), (WPARAM)lpData, 0);
}

/**
Occurs when a remote user (Communication)/host (Live Broadcast) leaves the channel.

Reasons why the user is offline:

Leave the channel: When the user/host leaves the channel, the user/host sends a goodbye message. When the message is received, the SDK assumes that the user/host leaves the channel.
Drop offline: When no data packet of the user or host is received for a certain period of time, the SDK assumes that the user/host drops offline. Unreliable network connections may lead to false detections, so we recommend using the Agora RTM SDK for more reliable offline detection.
Parameters
@param uid	User ID of the user leaving the channel or going offline.
@param reason	Reason why the user is offline: USER_OFFLINE_REASON_TYPE.
*/
void CAGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_OFFLINE lpData = new AGE_USER_OFFLINE;

	lpData->uid = uid;
	lpData->reason = reason;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_OFFLINE), (WPARAM)lpData, 0);
}

/**
Occurs when a remote user's audio stream playback pauses/resumes.

The SDK triggers this callback when the remote user stops or resumes sending the audio stream by calling the muteLocalAudioStream method.

Note
This callback returns invalid when the number of users in a channel exceeds 20.
Parameters
@param uid	User ID of the remote user.
@param muted	Whether the remote user's audio stream is muted/unmuted:
    true: Muted.
    false: Unmuted.
*/
void CAGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_MUTE_AUDIO lpData = new AGE_USER_MUTE_AUDIO;

	lpData->uid = uid;
	lpData->muted = muted;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_MUTE_AUDIO), (WPARAM)lpData, 0);
}

/**
Occurs when a remote user's video stream playback pauses/resumes.

You can also use the onRemoteVideoStateChanged callback with the following parameters:

REMOTE_VIDEO_STATE_STOPPED (0) and REMOTE_VIDEO_STATE_REASON_REMOTE_MUTED (5).
REMOTE_VIDEO_STATE_DECODING (2) and REMOTE_VIDEO_STATE_REASON_REMOTE_UNMUTED (6).
The SDK triggers this callback when the remote user stops or resumes sending the video stream by calling the muteLocalVideoStream method.

Note
This callback returns invalid when the number of users in a channel exceeds 20.
Parameters
@param uid	User ID of the remote user.
@param muted	Whether the remote user's video stream playback is paused/resumed:
        true: Paused.
        false: Resumed.
*/
void CAGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_MUTE_VIDEO lpData = new AGE_USER_MUTE_VIDEO;

	lpData->uid = uid;
	lpData->muted = muted;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_MUTE_VIDEO), (WPARAM)lpData, 0);
}

/**
Occurs when the local user receives the data stream from the remote user within five seconds.

The SDK triggers this callback when the local user receives the stream message that the remote user sends by calling the sendStreamMessage method.

Parameters
@param uid	User ID of the remote user sending the message.
@param streamId	Stream ID.
@param data	Pointer to the data received by the local user.
@param length	Length of the data in bytes.
*/
void CAGEngineEventHandler::onStreamMessage(uid_t uid, int streamId, const char* data, size_t length)
{
	if (m_hMainWnd == NULL)
		return;

    LPAGE_STREAM_MESSAGE lpData = new AGE_STREAM_MESSAGE;

    lpData->uid = uid;
    lpData->streamId = streamId;
    lpData->data = new char[length];
    lpData->length = length;

    memcpy_s(lpData->data, length, data, length);

    ::PostMessage(m_hMainWnd, WM_MSGID(EID_STREAM_MESSAGE), (WPARAM)lpData, 0);
}

/**
Occurs when a method is executed by the SDK.

Parameters
@param err	The error code (ERROR_CODE_TYPE) returned by the SDK when a method call fails. If the SDK returns 0, then the method call is successful.
@param api	Pointer to the method executed by the SDK.
@param result	Pointer to the result of the method call.
*/
void CAGEngineEventHandler::onApiCallExecuted(const char* api, int error)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_APICALL_EXECUTED lpData = new AGE_APICALL_EXECUTED;

	strcpy_s(lpData->api, 128, api);
	lpData->error = error;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_APICALL_EXECUTED), (WPARAM)lpData, 0);
}

/**
Reports the statistics of the local video stream.

The SDK triggers this callback once every two seconds for each user/host. If there are multiple users/hosts in the channel, the SDK triggers this callback as many times.

Note
If you have called the enableDualStreamMode method, the onLocalVideoStats callback reports the statistics of the high quality video stream (high bitrate, and high-resolution video stream).
Parameters
@param stats	Statistics of the local video stream. See LocalVideoStats.

*/
void CAGEngineEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_LOCAL_VIDEO_STAT lpData = new AGE_LOCAL_VIDEO_STAT;

	lpData->sentBitrate = stats.sentBitrate;
	lpData->sentFrameRate = stats.sentFrameRate;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_LOCAL_VIDEO_STAT), (WPARAM)lpData, 0);
}

/**
Reports the statistics of the video stream from each remote user/host.

The SDK triggers this callback once every two seconds for each remote user/host. If a channel includes multiple remote users, the SDK triggers this callback as many times.

Parameters
@param stats	Statistics of the remote video stream. See RemoteVideoStats.
*/
void CAGEngineEventHandler::onRemoteVideoStats(const RemoteVideoStats& stats)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_REMOTE_VIDEO_STAT lpData = new AGE_REMOTE_VIDEO_STAT;

	lpData->uid = stats.uid;
	lpData->delay = stats.delay;
	lpData->width = stats.width;
	lpData->height = stats.height;
	lpData->receivedFrameRate = stats.decoderOutputFrameRate;
	lpData->receivedBitrate = stats.receivedBitrate;
	lpData->receivedFrameRate = stats.decoderOutputFrameRate;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_REMOTE_VIDEO_STAT), (WPARAM)lpData, 0);
}

/**
DEPRECATED Occurs when the camera turns on and is ready to capture the video.

If the camera fails to turn on, fix the error reported in the onError callback.

Deprecated as of v2.4.1. Use LOCAL_VIDEO_STREAM_STATE_CAPTURING (1) in the onLocalVideoStateChanged callback instead.
*/
void CAGEngineEventHandler::onCameraReady()
{
	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_CAMERA_READY), 0, 0);
}

/**

*/
void CAGEngineEventHandler::onVideoStopped()
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_VIDEO_STOPPED), 0, 0);
}

/**
DEPRECATED Occurs when the video stops playing.

The application can use this callback to change the configuration of the view (for example, displaying other pictures in the view) after the video stops playing.

Deprecated as of v2.4.1. Use LOCAL_VIDEO_STREAM_STATE_STOPPED(0) in the onLocalVideoStateChanged callback instead.
*/
void CAGEngineEventHandler::onConnectionLost()
{
	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_CONNECTION_LOST), 0, 0);
}

/**

virtual void agora::rtc::IRtcEngineEventHandler::onConnectionInterrupted	(		)	
inlinevirtual
DEPRECATED Occurs when the connection between the SDK and the server is interrupted.

Deprecated as of v2.3.2. Replaced by the onConnectionStateChanged(CONNECTION_STATE_RECONNECTING, CONNECTION_CHANGED_INTERRUPTED) callback.

The SDK triggers this callback when it loses connection with the server for more than four seconds after the connection is established.

After triggering this callback, the SDK tries reconnecting to the server. You can use this callback to implement pop-up reminders.

This callback is different from onConnectionLost:

The SDK triggers the onConnectionInterrupted callback when it loses connection with the server for more than four seconds after it successfully joins the channel.
The SDK triggers the onConnectionLost callback when it loses connection with the server for more than 10 seconds, whether or not it joins the channel.
If the SDK fails to rejoin the channel 20 minutes after being disconnected from Agora's edge server, the SDK stops rejoining the channel.


*/
void CAGEngineEventHandler::onConnectionInterrupted()
{
	CString str;

	str = _T("onConnectionInterrupted");
}

/**

*/
void CAGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{
//	if (m_hMainWnd != NULL)
//		::PostMessage(m_hMainWnd, WM_MSGID(EID_CONNECTION_LOST), 0, 0);

}

/**
Occurs when a specific remote user enables/disables the video module.

Deprecated:
This callback is deprecated and replaced by the onRemoteVideoStateChanged callback with the following parameters:
REMOTE_VIDEO_STATE_STOPPED (0) and REMOTE_VIDEO_STATE_REASON_REMOTE_MUTED (5).
REMOTE_VIDEO_STATE_DECODING (2) and REMOTE_VIDEO_STATE_REASON_REMOTE_UNMUTED (6).
*/
void CAGEngineEventHandler::onStartRecordingService(int error)
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_START_RCDSRV), 0, 0);

}

/**
Deprecated
*/
void CAGEngineEventHandler::onStopRecordingService(int error)
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_STOP_RCDSRV), 0, 0);

}

/**
Deprecated
*/
void CAGEngineEventHandler::onRefreshRecordingServiceStatus(int status)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_RCDSRV_STATUS lpData = new AGE_RCDSRV_STATUS;

	lpData->status = status;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_REFREASH_RCDSRV), (WPARAM)lpData, 0);
}