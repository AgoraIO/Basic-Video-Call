#pragma once

#include "IAgoraRtcEngine.h"
#include "AGEngineEventHandler.h"

// #define ENABLE_CODEC	1

using namespace agora::rtc;


#define APP_ID _T("") // put your app id here
#define MEDIA_TOKEN _T("")  //put your media token

class CAgoraObject
{
public:
	~CAgoraObject(void);

	void SetMsgHandlerWnd(HWND hWnd);
	HWND GetMsgHandlerWnd();

	BOOL JoinChannel(LPCTSTR lpChannelName, UINT nUID = 0,LPCTSTR lpToken = NULL);
	BOOL LeaveChannel();
	CString GetChanelName();

	BOOL EnableVideo(BOOL bEnable = TRUE);
	BOOL IsVideoEnabled();

	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();

	BOOL MuteLocalVideo(BOOL bMuted = TRUE);
	BOOL IsLocalVideoMuted();

	static IRtcEngine *GetEngine();
	
	static CString GetSDKVersion();    

protected:
	CAgoraObject(void);

private:
	DWORD	m_dwEngineFlag;
	static  CAgoraObject	*m_lpAgoraObject;
	static	IRtcEngine		*m_lpAgoraEngine;
	
	CString		m_strChannelName;
	BOOL		m_bVideoEnable;

	BOOL		m_bLocalAudioMuted;
	BOOL		m_bLocalVideoMuted;

public:
	static CAgoraObject *GetAgoraObject(LPCTSTR lpVendorKey = NULL);
	static void CloseAgoraObject();

	static CAGEngineEventHandler m_EngineEventHandler;
};
