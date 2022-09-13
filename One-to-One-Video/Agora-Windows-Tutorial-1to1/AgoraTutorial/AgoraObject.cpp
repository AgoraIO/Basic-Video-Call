#include "StdAfx.h"
#include <stdio.h>

#include "AgoraObject.h"

CAgoraObject *CAgoraObject::m_lpAgoraObject = NULL;
IRtcEngine *CAgoraObject::m_lpAgoraEngine = NULL;
CAGEngineEventHandler CAgoraObject::m_EngineEventHandler;

CAgoraObject::CAgoraObject(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
{
	m_strChannelName.Empty();
	m_bLocalVideoMuted = FALSE;
}

CAgoraObject::~CAgoraObject(void)
{
}

/**
   gets current SDK version number string 
*/
CString CAgoraObject::GetSDKVersion()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	CString strEngineVer;

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszEngineVer, -1, strEngineVer.GetBuffer(256), 256);
	strEngineVer.ReleaseBuffer();
#endif

	return strEngineVer;
}

/** 
    gets the Agora Engine instance 
*/
IRtcEngine *CAgoraObject::GetEngine()
{
	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	return m_lpAgoraEngine;
}

/** 
    create or return existing AgoraObject 
*/
CAgoraObject *CAgoraObject::GetAgoraObject(LPCTSTR lpAppId)
{
	if (m_lpAgoraObject == NULL)
		m_lpAgoraObject = new CAgoraObject();

	if (m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	if (lpAppId == NULL)
		return m_lpAgoraObject;

	RtcEngineContext ctx;

	ctx.eventHandler = &m_EngineEventHandler;
#ifdef UNICODE
	char szAppId[128];

	::WideCharToMultiByte(CP_ACP, 0, lpAppId, -1, szAppId, 128, NULL, NULL);
	ctx.appId = szAppId;
#else
	ctx.appId = lpAppId;
#endif

	m_lpAgoraEngine->initialize(ctx);

	m_lpAgoraEngine->enableVideo();
	m_lpAgoraEngine->enableAudio();

	return m_lpAgoraObject;
}

/** 
    clean up the AgoraObject 
*/
void CAgoraObject::CloseAgoraObject()
{
	if(m_lpAgoraEngine != NULL)
		m_lpAgoraEngine->release();

	if(m_lpAgoraObject != NULL)
		delete m_lpAgoraObject;

	m_lpAgoraEngine = NULL;
	m_lpAgoraObject = NULL;
}

/**
  Setting Message Handler 

Parameters:
    @param hWnd     A handle to the handler
 */
void CAgoraObject::SetMsgHandlerWnd(HWND hWnd)
{
	m_EngineEventHandler.SetMsgReceiver(hWnd);
}

/** 
    retrieve the handle of the message handler
*/
HWND CAgoraObject::GetMsgHandlerWnd()
{
	return m_EngineEventHandler.GetMsgReceiver();
}

/**
   Join a channel for streaming or communication.

Parameters:
@param lpChannelName    name of the channel to join
@param uUID     User ID
@param lpToken  token key
*/
BOOL CAgoraObject::JoinChannel(LPCTSTR lpChannelName, UINT nUID,LPCTSTR lpToken)
{
	int nRet = 0;

#ifdef UNICODE
	CHAR szChannelName[128];
	::WideCharToMultiByte(CP_UTF8, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);

	char szToken[128];
	::WideCharToMultiByte(CP_UTF8, 0, lpToken, -1, szToken, 128, NULL, NULL);

	ChannelMediaOptions options;
	options.channelProfile = agora::CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_LIVE_BROADCASTING;
	options.clientRoleType = CLIENT_ROLE_TYPE::CLIENT_ROLE_BROADCASTER;

	if(0 == _tcslen(lpToken))
		nRet = m_lpAgoraEngine->joinChannel(NULL, szChannelName, nUID, options);
	else
		nRet = m_lpAgoraEngine->joinChannel(szToken, szChannelName,  nUID, options);
#else
	if(0 == _tcslen(lpToken))
		nRet = m_lpAgoraEngine->joinChannel(NULL, lpChannelName, NULL, nUID);
	else
		nRet = m_lpAgoraEngine->joinChannel(lpToken, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;

	m_lpAgoraEngine->startPreview();

	return nRet == 0 ? TRUE : FALSE;
}

/**
    Leave the channel
*/
BOOL CAgoraObject::LeaveChannel()
{
	m_lpAgoraEngine->stopPreview();
	int nRet = m_lpAgoraEngine->leaveChannel();

	return nRet == 0 ? TRUE : FALSE;
}

/**
    get current channel name
*/
CString CAgoraObject::GetChanelName()
{
	return m_strChannelName;
}

/**
    turn video on/off
 Parameters:
    @param bEnable true if turn on else turn off
*/
BOOL CAgoraObject::EnableVideo(BOOL bEnable)
{
	int nRet = 0;

	if (bEnable)
		nRet = m_lpAgoraEngine->enableVideo();
	else
		nRet = m_lpAgoraEngine->disableVideo();

	if (nRet == 0)
		m_bVideoEnable = bEnable;

	return nRet == 0 ? TRUE : FALSE;
}

/**
    check if video enabled
*/
BOOL CAgoraObject::IsVideoEnabled()
{
	return m_bVideoEnable;
}
/**
    mute local audio on/off
 Parameters:
    @param bMuted true if muted else not muted
*/
BOOL CAgoraObject::MuteLocalAudio(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	int ret =  m_lpAgoraEngine->muteLocalAudioStream((bool)bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

/** 
    check if local audio is muted
*/
BOOL CAgoraObject::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}

/**
    mute local video on/off
 Parameters:
    @param bMuted true if muted else not muted
*/
BOOL CAgoraObject::MuteLocalVideo(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	int ret =  m_lpAgoraEngine->muteLocalVideoStream((bool)bMuted);
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}
/** 
    check if local video is muted
*/
BOOL CAgoraObject::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}
