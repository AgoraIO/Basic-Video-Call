# Agora 1-to-1 Tutorial for Windows

This tutorial enables you to quickly get started developing a Windows app with video chat. With this sample app you can:

* Join and leave a channel
* Mute and unmute audio
* Enable or disable video

## Prerequisites

* Visual Studio 2013 or higher
* Windows 7 or higher

## Quick Start
This section shows you how to prepare, build, and run the sample application.

### Create an Account and Obtain an App ID
To build and run the sample application you must obtain an App ID: 

1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/).
2. Upon logging in, use the dashboard tree on the left to navigate to  **Projects** > **Project List**.
3. Locate the file **AgoraTutorial/AgoraObject.h** and replace `YOUR-APP-ID` with the App ID in the dashboard.

```
#define APP_ID _T("YOUR-APP-ID") // put your app id here
```

### Integrate the Agora Video SDK into the sample project

1. Clone this repository.
2. Download the Agora Video SDK from [Agora.io SDK](https://www.agora.io/en/download/).
3. Unzip the downloaded SDK package.
4. Copy the `sdk` folder from the downloaded SDK package into the `AgoraTutorial` folder of the sample application.

### Obtain and Build the Sample Application 

1. Open the `AgoraTutorial.sln` file in Visual Studio.
2. Build and run the sample project.

## Steps to Create the Sample 

This sample app is written C++.

The key code for the sample application is in the `AgoraTutorial` folder. The following steps identify the code most relevant to using the Agora SDK.

- [Create the AgoraTutorialDlg Class](#create-the-agoratutorialdlg-class)
- [Create the AgoraObject Class](#create-the-agoraobject-class)
- [Create the AGEngineEventHandler Class](#create-the-agengineeventhandler-class)

For details about the APIs used to develop this sample, see the [Agora.io Documentation version 2.2](https://docs.agora.io/en/2.2).


### Create the AgoraTutorialDlg Class

The `AgoraTutorialDlg` class manages UI element interactions with the application and Agora SDK.

- [Add the OnInitDialog() Method](#add-the-oninitdialog-method)
- [Add the OnBnClickedBtnjoin() Method](#add-the-onbnclickedbtnjoin-method)
- [Add the OnBnClickedBtnleave() Method](#add-the-onbnclickedbtnleave-method)
- [Add the OnBnClickedBtnmutelocaud() Method](#add-the-onbnclickedbtnmutelocaud-method)
- [Add the OnBnClickedBtndisvid() Method](#add-the-onbnclickedbtndisvid-method)
- [Add the OnFirstRemoteVideoDecoded() Callback](#add-the-onfirstremotevideodecoded-callback)

#### Add the OnInitDialog() Method

The `OnInitDialog()` method initializes the sample application.

Retrieve the system menu using `GetSystemMenu()`. Verify that `pSysMenu` is not `NULL` and perform the following actions:

1. Declare a boolean `bNameValid` variable and a string `strAboutMenu` variable.
2. Obtain the value of `bNameValid` by passing `IDS_ABOUTBOX` into `strAboutMenu.LoadString()`.
3. Validate `bNameValid` and ensure `strAboutMenu` is not empty before appending the menu items using `pSysMenu->AppendMenu()`.

The remaining code in this section is within the `OnInitDialog()` method.

``` C++
BOOL CAgoraTutorialDlg::OnInitDialog()
{
	...

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	...
}
```

Set the application icons using `SetIcon()`. If `APP_ID` is not set, use `MessageBox()` to display a message telling the user to apply the App ID to the application, and exit the application using `ExitProcess()`.

``` C++
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	if (_tcslen(APP_ID) == 0) {
		MessageBox(_T("Please apply your own App ID to macro APP_ID"), _T("Notice"), MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
```

Begin the Agora engine initialization process.

1. Create and set `m_lpAgoraObject` with `APP_ID` using `GetAgoraObject()`.
2. Set the message handler using `m_lpAgoraObject->SetMsgHandlerWnd()`.
3. Enable video for the Agora engine using `m_lpAgoraObject->GetEngine()->enableVideo()`.
4. Set the default video canvas, user ID, view, and render mode properties `vc.uid`, `vc.view`, and `vc.renderMode`.
4. Set up local video with `vc` using `m_lpAgoraObject->GetEngine()->setupLocalVideo()`.

``` C++
	m_lpAgoraObject = CAgoraObject::GetAgoraObject(APP_ID);
	m_lpAgoraObject->SetMsgHandlerWnd(GetSafeHwnd());

	m_lpAgoraObject->GetEngine()->enableVideo();

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_wndLocal.GetSafeHwnd();
	vc.renderMode = RENDER_MODE_FIT;

	m_lpAgoraObject->GetEngine()->setupLocalVideo(vc);

	return TRUE;
```

#### Add the OnBnClickedBtnjoin() Method

The `OnBnClickedBtnjoin()` method triggers when the join UI button is clicked.

Invoke `UpdateData()`.

- If `m_strChannelName` is empty, display a prompt explaining that the channel name `must not be empty`.
- If `m_strChannelName` is not empty, join the channel specified by `m_strChannelName` using `m_lpAgoraObject->JoinChannel()`.

``` C++
void CAgoraTutorialDlg::OnBnClickedBtnjoin()
{
	UpdateData(TRUE);

	if (m_strChannelName.IsEmpty())
		MessageBox(_T("Channel name must not be empty"), _T("Notice"), MB_OK | MB_ICONERROR);
	else
		m_lpAgoraObject->JoinChannel(m_strChannelName);
}

```

#### Add the OnBnClickedBtnleave() Method

The `OnBnClickedBtnleave()` method triggers when the leave UI button is clicked. Leave the channel by invoking `m_lpAgoraObject->LeaveChannel()`.

``` C++
void CAgoraTutorialDlg::OnBnClickedBtnleave()
{
	m_lpAgoraObject->LeaveChannel();
}
```

#### Add the OnBnClickedBtnmutelocaud() Method

The `OnBnClickedBtnmutelocaud()` method triggers when the local audio button is clicked.

Retrieve the current local audio mute status using `m_lpAgoraObject->IsLocalAudioMuted()` and apply the reverse value to mute or unmute the local audio using `m_lpAgoraObject->MuteLocalAudio()`.

``` C++
void CAgoraTutorialDlg::OnBnClickedBtnmutelocaud()
{
	BOOL bStatValue = !m_lpAgoraObject->IsLocalAudioMuted();
	m_lpAgoraObject->MuteLocalAudio(bStatValue);
}
```

#### Add the OnBnClickedBtndisvid() Method

The `OnBnClickedBtndisvid()` method triggers when the enable or disable video button is clicked.

Retrieve the current video enable or disable status using `m_lpAgoraObject->IsVideoEnabled()` and apply the reverse value to enable or disable the video using `m_lpAgoraObject->MuteLocalVideo()`.

``` C++
void CAgoraTutorialDlg::OnBnClickedBtndisvid()
{
	BOOL bStatValue = !m_lpAgoraObject->IsVideoEnabled();
	m_lpAgoraObject->MuteLocalVideo(bStatValue);
}
```

#### Add the OnFirstRemoteVideoDecoded() Callback

The `OnFirstRemoteVideoDecoded()` method triggers when the first remote video frame is decoded.

1. Convert `wParam` into a `LPAGE_FIRST_REMOTE_VIDEO_DECODED` object.
2. Create a `VideoCanvas` object and set the render mode, user ID, and view properties using `vc.renderMode`, `vc.uid`, and `vc.view`.
3. Set up the remote video with `vc` using `m_lpAgoraObject->GetEngine()->setupRemoteVideo()`.
4. Clear `lpData` from memory using `delete` and return `0`.

``` C++
LRESULT CAgoraTutorialDlg::OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;

	VideoCanvas vc;

	vc.renderMode = RENDER_MODE_FIT;
	vc.uid = lpData->uid;
	vc.view = m_wndRemote.GetSafeHwnd();

	m_lpAgoraObject->GetEngine()->setupRemoteVideo(vc);

	delete lpData;

	return 0;
}
```

### Create the AgoraObject Class

The `AgoraObject` class manages communication with the Agora SDK.

- [Declare Global Variables and Initialize CAgoraObject](#declare-global-variables-and-initialize-cagoraobject)
- [Create Agora SDK and Engine Methods](#create-agora-sdk-and-engine-methods)
- [Create Message Handlers](#create-message-handlers)
- [Create Channel Methods](#create-channel-methods)
- [Create Video Methods](#create-video-methods)
- [Create Audio Methods](#create-audio-methods)

#### Declare Global Variables and Initialize CAgoraObject

Declare the global Agora SDK variables:

Variable|Value|Description
---|---|---
`m_lpAgoraObject`|`NULL`|The Agora object
`m_lpAgoraEngine`|`NULL`|The Agora engine
`m_EngineEventHandler`|N/A|The event handler for the Agora engine


``` C++
#include "StdAfx.h"
#include <stdio.h>

#include "AgoraObject.h"

CAgoraObject *CAgoraObject::m_lpAgoraObject = NULL;
IRtcEngine *CAgoraObject::m_lpAgoraEngine = NULL;
CAGEngineEventHandler CAgoraObject::m_EngineEventHandler;
```

When `CAgoraObject` is initialized, clear the channel name using `m_strChannelName.Empty()` and set `m_bLocalVideoMuted` to `FALSE`. 

``` C++
CAgoraObject::CAgoraObject(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
{
	m_strChannelName.Empty();
	m_bLocalVideoMuted = FALSE;
}
```

#### Create Agora SDK and Engine Methods

The methods in this section manage the Agora SDK and engine.

- [Add the GetSDKVersion() Method](#add-the-getsdkversion-method)
- [Add the GetEngine() Method](#add-the-getengine-method)
- [Add the GetAgoraObject() Method](#add-the-getagoraobject-method)
- [Add the CloseAgoraObject() Method](#add-the-closeagoraobject-method)

##### Add the GetSDKVersion() Method

The `GetSDKVersion()` method retrieves the version number of the Agora SDK used in the sample application.

Initialize `nBuildNumber` to `0` and retrieve the Agora RTC engine version number using `getAgoraRtcEngineVersion()`.

Convert `lpszEngineVer` to a `CString` object and return its value.

``` C++
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
```

##### Add the GetEngine() Method

The `GetEngine()` method retrieves the Agora engine object.

If `m_lpAgoraEngine` is `NULL`, the engine has not been created yet. Create it using `createAgoraRtcEngine()`.

Return the engine object `m_lpAgoraEngine`.

``` C++
IRtcEngine *CAgoraObject::GetEngine()
{
	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	return m_lpAgoraEngine;
}
```

##### Add the GetAgoraObject() Method

The `GetAgoraObject()` method retrieves the Agora SDK object.

- If `m_lpAgoraObject` is `NULL`, the Agora object has not been created yet. Create it using `new CAgoraObject()`.
- If `m_lpAgoraEngine` is `NULL`, the engine has not been created yet. Create it using `createAgoraRtcEngine()`.

If `lpAppId` is `NULL`, return `m_lpAgoraObject`.

Create a `RtcEngineContext` object and set the event handler and App ID properties `ctx.eventHandler` and `ctx.appId`. If unicode is enabled, use `szAppId` as the App ID. Otherwise, use `lpAppId `.

Initialize the engine with `ctx` using `m_lpAgoraEngine->initialize()` and return `m_lpAgoraObject`.

``` C++
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

	return m_lpAgoraObject;
}
```

##### Add the CloseAgoraObject() Method

The `CloseAgoraObject()` method clears the Agora object from memory.

- If `m_lpAgoraEngine` exists, release it from memory using `m_lpAgoraEngine->release()`.
- If `m_lpAgoraObject` exists, delete it using `delete`.

Set `m_lpAgoraEngine` and `m_lpAgoraObject` to `NULL`.

``` C++
void CAgoraObject::CloseAgoraObject()
{
	if(m_lpAgoraEngine != NULL)
		m_lpAgoraEngine->release();

	if(m_lpAgoraObject != NULL)
		delete m_lpAgoraObject;

	m_lpAgoraEngine = NULL;
	m_lpAgoraObject = NULL;
}
```

#### Create Message Handlers

The methods in this section manage the message receiver.

##### Add the SetMsgHandlerWnd() Method

The `SetMsgHandlerWnd()` method sends the message `hWnd` to the Agora engine's event handler using `m_EngineEventHandler.SetMsgReceiver()`.

``` C++
void CAgoraObject::SetMsgHandlerWnd(HWND hWnd)
{
	m_EngineEventHandler.SetMsgReceiver(hWnd);
}
```

##### Add the GetMsgHandlerWnd() Method

The `GetMsgHandlerWnd()` method retrieves the message receiver object from the Agora engine'e event handler using `m_EngineEventHandler.GetMsgReceiver()`.

``` C++
HWND CAgoraObject::GetMsgHandlerWnd()
{
	return m_EngineEventHandler.GetMsgReceiver();
}
```

#### Create Channel Methods

The methods in this section manage joining or leaving a channel and retrieving the channel name.

##### Add the JoinChannel() Method

The `JoinChannel()` method joins the user specified by `nUID` to the channel specified by `lpChannelName`.

Initialize `nRet` with a value of `0` and join the channel using `m_lpAgoraEngine->joinChannel()`. Use `szChannelName` if unicode is supported, and `lpChannelName` otherwise.

Ensure the channel join is successful by checking if `nRet` is equal to `0` and set `m_strChannelName` to `lpChannelName`.

``` C++
BOOL CAgoraObject::JoinChannel(LPCTSTR lpChannelName, UINT nUID)
{
	int nRet = 0;

#ifdef UNICODE
	CHAR szChannelName[128];

	::WideCharToMultiByte(CP_UTF8, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);
	nRet = m_lpAgoraEngine->joinChannel(NULL, szChannelName, NULL, nUID);
#else
	nRet = m_lpAgoraEngine->joinChannel(NULL, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;

	return nRet == 0 ? TRUE : FALSE;
}
```

##### Add the LeaveChannel() Method

The `LeaveChannel()` method leaves the channel.

Stop the preview using `m_lpAgoraEngine->stopPreview()`, then leave the channel using `m_lpAgoraEngine->leaveChannel()`.

If the leave channel action is successful, return `TRUE`. Otherwise, return `FALSE`.

``` C++
BOOL CAgoraObject::LeaveChannel()
{
	m_lpAgoraEngine->stopPreview();
	int nRet = m_lpAgoraEngine->leaveChannel();

	return nRet == 0 ? TRUE : FALSE;
}
```

##### Add the GetChanelName() Method

The `GetChanelName()` method retrieves the channel name, returning `m_strChannelName`.

``` C++
CString CAgoraObject::GetChanelName()
{
	return m_strChannelName;
}
```

#### Create Video Methods

The methods in this section manage the video.

##### Add the EnableVideo() Method

The `EnableVideo()` method enables the video for the sample application.

Initialize `nRet` with a value for `0`. If `bEnable` is `TRUE`, enable the video using `m_lpAgoraEngine->enableVideo()`. Otherwise, disable the video using `m_lpAgoraEngine->disableVideo()`.

Ensure the enable or disable video action is successful and set `m_bVideoEnable` to `bEnable`.

``` C++
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
```

##### Add the IsVideoEnabled() Method

The `IsVideoEnabled()` method indicates if video is enabled for the sample application and returns `m_bVideoEnable`.

``` C++
BOOL CAgoraObject::IsVideoEnabled()
{
	return m_bVideoEnable;
}
```

##### Add the MuteLocalVideo() Method

The `MuteLocalVideo()` method turns local video on or off.

Ensure `m_lpAgoraEngine` is not `NULL` before completing the remaining method actions.

Declare a `RtcEngineParameters` object and invoke its `muteLocalVideoStream()` method with `bMuted`.

If the mute or unmute local video is successful, set `m_bLocalVideoMuted` to `bMuted`.

``` C++
BOOL CAgoraObject::MuteLocalVideo(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalVideoStream((bool)bMuted);
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}
```

##### Add the IsLocalVideoMuted() Method

The `IsLocalVideoMuted()` method indicates if the local video is on or off for the sample application, returning `m_bLocalVideoMuted`.

``` C++
BOOL CAgoraObject::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}
```

#### Create Audio Methods

The methods in this section manage the audio.

##### Add the MuteLocalAudio() Method

The `MuteLocalAudio()` method mutes or unmutes the local audio.

Ensure `m_lpAgoraEngine` is not `NULL` before completing the remaining method actions.

Declare a `RtcEngineParameters` object and invoke its `muteLocalAudioStream()` method with `bMuted`.

If the mute or unmute local audio is successful, set `m_bLocalAudioMuted` to `bMuted`.

``` C++
BOOL CAgoraObject::MuteLocalAudio(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalAudioStream((bool)bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}
```

##### Add the IsLocalAudioMuted() Method

The `IsLocalAudioMuted()` method indicates if local audio is muted or unmuted for the sample application, returning `m_bLocalAudioMuted`.

``` C++
BOOL CAgoraObject::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}

```

### Create the AGEngineEventHandler Class

The methods in the `AGEngineEventHandler` class manage event listener callbacks.

- [Create Channel Callbacks](#create-channel-callbacks)
- [Create Engine and API Callbacks](#create-engine-and-api-callbacks)
- [Create Audio and Video Callbacks](#create-audio-and-video-callbacks)
- [Create Statistics Callbacks](#create-statistics-callbacks)
- [Create Device Callbacks](#create-device-callbacks)
- [Create User Callbacks](#create-user-callbacks)
- [Create Connection Callbacks](#create-connection-callbacks)
- [Create Recording Callbacks](#create-recording-callbacks)

#### Create Channel Callbacks

Changes in the channel trigger the callbacks in this section.
	
##### Add the onJoinChannelSuccess() Callback

The `onJoinChannelSuccess()` callback triggers when a channel is successfully joined.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_JOINCHANNEL_SUCCESS` object and set the channel name, user ID, and time elapsed values using `lpData->channel`, `lpData->uid`, and `lpData->elapsed`.

**Note:** `lpData->channel` requires memory to be allocated prior to setting the channel name using `strcpy_s()`.

Complete the method by displaying a join channel success message using `PostMessage()`. 

``` C++
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
```

##### Add the onRejoinChannelSuccess() Callback

The `onRejoinChannelSuccess()` callback triggers when a channel is successfully rejoined. 

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_REJOINCHANNEL_SUCCESS ` object and set the channel name, user ID, and time elapsed values using `lpData->channel`, `lpData->uid`, and `lpData->elapsed`.

**Note:** `lpData->channel` requires memory to be allocated prior to setting the channel name using `strcpy_s()`.

Complete the method by displaying a rejoin channel success message using `PostMessage()`. 

``` C++
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
```

##### Add the onLeaveChannel() Callback

The `onLeaveChannel()` callback triggers when a leave channel event occurs with statistics.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_LEAVE_CHANNEL` object and set the statistics property `lpData->rtcStat` with `stat` using `memcpy()`.

Complete the method by displaying a leave channel message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_LEAVE_CHANNEL lpData = new AGE_LEAVE_CHANNEL;

	memcpy(&lpData->rtcStat, &stat, sizeof(RtcStats));

	::PostMessage(m_hMainWnd, WM_MSGID(EID_LEAVE_CHANNEL), (WPARAM)lpData, 0);
}
```

#### Create Engine and API Callbacks

The callbacks in this section trigger when changes to Agora engine or API occur.

- [Add the onWarning() Callback](#add-the-onwarning-callback)
- [Add the onError() Callback](#add-the-onerror-callback)
- [Add the onMediaEngineEvent() Callback](#add-the-onmediaengineevent-callback)
- [Add the onApiCallExecuted() Callback](#add-the-onapicallexecuted-callback)

##### Add the onWarning() Callback

The `onWarning()` callback triggers when a warning occurs from the Agora engine.

Declare a `CString` object and set its value to `_T("onWarning")`.

``` C++
void CAGEngineEventHandler::onWarning(int warn, const char* msg)
{
	CString str;

	str = _T("onWarning");
}
```

##### Add the onError() Callback

The `onError()` callback triggers when an error occurs from the Agora engine.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_ERROR` object.

- If `msg` is not `NULL`, set `lpData->msg` to `msg`.

	**Note:** `lpData->msg` requires memory to be allocated prior to setting the message using `strcpy_s()`.

- If `msg` is `NULL`, set `lpData->msg` to `NULL`.


Complete the method by displaying an error message using `PostMessage()`. 

``` C++
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
```

##### Add the onMediaEngineEvent() Callback

The `onMediaEngineEvent()` callback triggers when an event occurs on the Agora media engine.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_MEDIA_ENGINE_EVENT` object and set the event property `lpData->evt` with `evt`.

Complete the method by displaying a media engine event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onMediaEngineEvent(int evt)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_MEDIA_ENGINE_EVENT lpData = new AGE_MEDIA_ENGINE_EVENT;

	lpData->evt = evt;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_MEDIA_ENGINE_EVENT), (WPARAM)lpData, 0);
}
```


##### Add the onApiCallExecuted() Callback

The `onApiCallExecuted()` callback triggers when an API call is executed.

Declare a new `LPAGE_APICALL_EXECUTED` object and set the api and error properties `lpData->api` and `lpData->error`.

**Note:** `lpData->api` requires memory allocation prior to setting `api` using `strcpy_s()`.

Complete the method by displaying an API call execution event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onApiCallExecuted(const char* api, int error)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_APICALL_EXECUTED lpData = new AGE_APICALL_EXECUTED;

	strcpy_s(lpData->api, 128, api);
	lpData->error = error;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_APICALL_EXECUTED), (WPARAM)lpData, 0);
}
```

#### Create Audio and Video Callbacks

The callbacks in this section trigger when changes to audio or video occur.

- [Add the onAudioQuality() Callback](#add-the-onaudioquality-callback)
- [Add the onAudioVolumeIndication() Callback](#add-the-onaudiovolumeindication-callback)
- [Add the onFirstLocalVideoFrame() Callback](#add-the-onfirstlocalvideoframe-callback)
- [Add the onFirstRemoteVideoDecoded() Callback](#add-the-onfirstremotevideodecoded-callback)
- [Add the onFirstRemoteVideoFrame() Callback](#add-the-onfirstremotevideoframe-callback)
- [Add the onVideoStopped() Callback](#add-the-onvideostopped-callback)

##### Add the onAudioQuality() Callback

The `onAudioQuality()` callback triggers when the audio quality changes for a user specified by `uid`.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_AUDIO_QUALITY` object and set the user ID, quality, time delay, and lost properties using `lpData->uid`, `lpData->quality`, `lpData->delay`, and `lpData->lost`.

Complete the method by displaying an audio quality change message using `PostMessage()`. 


``` C++
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

```

##### Add the onAudioVolumeIndication() Callback

The `onAudioVolumeIndication()` callback triggers when the audio volume indicator changes for a speaker specified by `speakerNumber`.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_AUDIO_VOLUME_INDICATION` object and set the speakers, speaker ID, and total volume properties using `lpData->speakers`, `lpData->speakerNumber`, and `lpData->totalVolume`.

**Note:** `lpData->speakers` requires a new `AudioVolumeInfo` object to be created with memory allocation, prior to setting `speakers` using `strcpy_s()`.

Complete the method by displaying an audio indication change message using `PostMessage()`. 

``` C++
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
```


##### Add the onFirstLocalVideoFrame() Callback

The `onFirstLocalVideoFrame()` callback triggers when the first local video frame has elapsed.

Declare a new `LPAGE_FIRST_LOCAL_VIDEO_FRAME` object and set the width, height, and time elapsed properties `lpData->width`, `lpData->height`, and `lpData->elapsed`.

Complete the method by displaying a first local video frame received event message using `PostMessage()`. 


``` C++
void CAGEngineEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = new AGE_FIRST_LOCAL_VIDEO_FRAME;

	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), (WPARAM)lpData, 0);

}
```

##### Add the onFirstRemoteVideoDecoded() Callback

The `onFirstRemoteVideoDecoded()` callback triggers when the first remote video is decoded.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_FIRST_REMOTE_VIDEO_DECODED` object and set the following properties:

Property|Value|Description
---|---|---
`lpData->uid`|`uid`|User ID
`lpData->width`|`width`|Width
`lpData->height`|`height`|Height
`lpData->elapsed`|`elapsed`|Time elapsed

Complete the method by displaying a first remote video decoded event message using `PostMessage()`. 

``` C++
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
```

##### Add the onFirstRemoteVideoFrame() Callback

The `onFirstRemoteVideoFrame()` callback triggers when the first remote video frame is received.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_FIRST_REMOTE_VIDEO_FRAME` object and set the following properties:

Property|Value|Description
---|---|---
`lpData->uid`|`uid`|User ID
`lpData->width`|`width`|Width
`lpData->height`|`height`|Height
`lpData->elapsed`|`elapsed`|Time elapsed

Complete the method by displaying a first remote video frame received event message using `PostMessage()`. 

``` C++
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
```

##### Add the onVideoStopped() Callback

The `onVideoStopped()` callback triggers when the video stops.

If `m_hMainWnd` is not `NULL`, display a video stopped event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onVideoStopped()
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_VIDEO_STOPPED), 0, 0);
}
```

#### Create Statistics Callbacks

The callbacks in this section trigger when statistic changes occur.

##### Add the onRtcStats() Callback

The `onRtcStats()` callback triggers when an RTC statistic changes.

Declare a `CString` variable `str` and set the value to `_T("stat")`.

``` C++
void CAGEngineEventHandler::onRtcStats(const RtcStats& stat)
{
	CString str;

	str = _T("stat");
}

```

##### Add the onLastmileQuality() Callback

The `onLastmileQuality()` callback triggers when the last mile quality test changes.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_LASTMILE_QUALITY` object and set the quality property `lpData->quality`.

Complete the method by displaying a last mile quality change event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onLastmileQuality(int quality)
{
	if (m_hMainWnd == NULL)
		return;

    LPAGE_LASTMILE_QUALITY lpData = new AGE_LASTMILE_QUALITY;

	lpData->quality = quality;

    ::PostMessage(m_hMainWnd, WM_MSGID(EID_LASTMILE_QUALITY), (WPARAM)lpData, 0);
}
```

##### Add the onLocalVideoStats() Callback

The `onLocalVideoStats()` callback triggers when local video statistics are received.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_LOCAL_VIDEO_STAT` object and set the bitrate and framerate properties `lpData->sentBitrate` and `lpData->sentFrameRate`.

Complete the method by displaying a local video statistics event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_LOCAL_VIDEO_STAT lpData = new AGE_LOCAL_VIDEO_STAT;

	lpData->sentBitrate = stats.sentBitrate;
	lpData->sentFrameRate = stats.sentFrameRate;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_LOCAL_VIDEO_STAT), (WPARAM)lpData, 0);
}
```

##### Add the onRemoteVideoStats() Callback

The `onRemoteVideoStats()` callback triggers when remote video statistics are received.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_REMOTE_VIDEO_STAT` object and set:

Property|Value|Description
---|---|---
`lpData->uid`|`stats.uid`|User ID
`lpData->delay`|`stats.delay`|Time delay
`lpData->width`|`stats.width`|Width
`lpData->height`|`stats.height`|Height
`lpData->sentBitrate`|`stats.receivedBitrate`|Bit rate
`lpData->sentFrameRate`|`stats.receivedFrameRate`|Frame rate

Complete the method by displaying a remote video statistics event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onRemoteVideoStats(const RemoteVideoStats& stats)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_REMOTE_VIDEO_STAT lpData = new AGE_REMOTE_VIDEO_STAT;

	lpData->uid = stats.uid;
	lpData->delay = stats.delay;
	lpData->width = stats.width;
	lpData->height = stats.height;
	lpData->receivedFrameRate = stats.receivedFrameRate;
	lpData->receivedBitrate = stats.receivedBitrate;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_REMOTE_VIDEO_STAT), (WPARAM)lpData, 0);
}
```

#### Create Device Callbacks

The callbacks in this section trigger when device changes occur.

##### Add the onAudioDeviceStateChanged() Callback

The `onAudioDeviceStateChanged()` callback triggers when an audio device state changes.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_AUDIO_DEVICE_STATE_CHANGED` object and set the device ID, device type, and device state properties `lpData->deviceId`, `lpData->deviceType`, and `lpData->deviceState`.

**Note:** `lpData->deviceId` requires memory allocation prior to setting `deviceId` using `strcpy_s()`.

Complete the method by displaying an audio device change event message using `PostMessage()`. 

``` C++
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
```

##### Add the onVideoDeviceStateChanged() Callback

The `onVideoDeviceStateChanged()` callback triggers when a video device state changes.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_VIDEO_DEVICE_STATE_CHANGED` object and set the device ID, device type, and device state properties `lpData->deviceId`, `lpData->deviceType`, and `lpData->deviceState`.

**Note:** `lpData->deviceId` requires memory allocation prior to setting `deviceId` using `strcpy_s()`.

Complete the method by displaying a video device change event message using `PostMessage()`. 

``` C++
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
```

##### Add the onCameraReady() Callback

The `onCameraReady()` callback triggers when the camera is ready for use.

If `m_hMainWnd` is not `NULL`, displaying a camera ready event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onCameraReady()
{
	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_CAMERA_READY), 0, 0);
}
```

#### Create User Callbacks

The callbacks in this section trigger when user changes occur.

- [Add the onUserJoined() Callback](#add-the-onuserjoined-callback)
- [Add the onUserOffline() Callback](#add-the-onuseroffline-callback)
- [Add the onUserMuteAudio() Callback](#add-the-onusermuteaudio-callback)
- [Add the onUserMuteVideo() Callback](#add-the-onusermutevideo-callback)
- [Add the onStreamMessage() Callback](#add-the-onstreammessage-callback)
- [Add the onUserEnableVideo() Callback](#add-the-onuserenablevideo-callback)

##### Add the onUserJoined() Callback

The `onUserJoined()` callback triggers when a user specified by `uid` joins the channel.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_USER_JOINED` object and set the user ID and time elapsed properties `lpData->uid` and `lpData->elapsed`.

Complete the method by displaying a user joined event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_JOINED lpData = new AGE_USER_JOINED;

	lpData->uid = uid;
	lpData->elapsed = elapsed;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_JOINED), (WPARAM)lpData, 0);
}
```

##### Add the onUserOffline() Callback

The `onUserOffline()` callback triggers when a user specified by `uid` goes offline.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_USER_OFFLINE` object and set the user ID and reason properties `lpData->uid` and `lpData->reason`.

Complete the method by displaying a user offline event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_OFFLINE lpData = new AGE_USER_OFFLINE;

	lpData->uid = uid;
	lpData->reason = reason;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_OFFLINE), (WPARAM)lpData, 0);
}
```

##### Add the onUserMuteAudio() Callback

The `onUserMuteAudio()` callback triggers when a user specified by `uid` mutes their audio.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_USER_MUTE_AUDIO` object and set the user ID and mute properties `lpData->uid` and `lpData->muted`.

Complete the method by displaying a user muted audio event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_MUTE_AUDIO lpData = new AGE_USER_MUTE_AUDIO;

	lpData->uid = uid;
	lpData->muted = muted;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_MUTE_AUDIO), (WPARAM)lpData, 0);
}
```

##### Add the onUserMuteVideo() Callback

The `onUserMuteAudio()` callback triggers when a user specified by `uid` turns off their video.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_USER_MUTE_VIDEO` object and set the user ID and on or off properties `lpData->uid` and `lpData->muted`.

Complete the method by displaying a user turned off video event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_USER_MUTE_VIDEO lpData = new AGE_USER_MUTE_VIDEO;

	lpData->uid = uid;
	lpData->muted = muted;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_MUTE_VIDEO), (WPARAM)lpData, 0);
}
```

##### Add the onStreamMessage() Callback

The `onStreamMessage()` callback triggers when a user specified by `uid` sends a message to the stream.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_STREAM_MESSAGE` object and set the user ID, stream ID, data, and length properties `lpData->uid`, `lpData->streamId`, `lpData->data`, and `lpData->length`.

Complete the method by displaying a user sent a stream message using `PostMessage()`. 

``` C++
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
```

##### Add the onUserEnableVideo() Callback

The `onUserEnableVideo()` callback triggers when a user specified by `uid` enables video. This method is not currently set up, but you may add code to update changes to your application or send a message about the event.

``` C++
void CAGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{
//	if (m_hMainWnd != NULL)
//		::PostMessage(m_hMainWnd, WM_MSGID(EID_CONNECTION_LOST), 0, 0);

}
```

#### Create Connection Callbacks

The callbacks in this section trigger when connection changes occur.

##### Add the onConnectionLost() Callback

The `onConnectionLost()` callback triggers when the connection is lost.

If `m_hMainWnd` is not `NULL`, display a connection lost event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onConnectionLost()
{
	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_CONNECTION_LOST), 0, 0);
}
```

##### Add the onConnectionInterrupted() Callback

The `onConnectionInterrupted()` callback triggers when the connection is interrupted.

Declare a `CString` object and set its value to `_T("onConnectionInterrupted")`. 

``` C++
void CAGEngineEventHandler::onConnectionInterrupted()
{
	CString str;

	str = _T("onConnectionInterrupted");
}
```

#### Create Recording Callbacks

The callbacks in this section trigger when changes occur during the recording service.

##### Add the onStartRecordingService() Callback

The `onStartRecordingService()` callback triggers when the recording service starts.

If `m_hMainWnd` is not `NULL`, display a recording service started event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onStartRecordingService(int error)
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_START_RCDSRV), 0, 0);

}
```

##### Add the onStopRecordingService() Callback

The `onStopRecordingService()` callback triggers when the recording service stops.

If `m_hMainWnd` is not `NULL`, display a recording service stopped event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onStopRecordingService(int error)
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_STOP_RCDSRV), 0, 0);

}
```

##### Add the onRefreshRecordingServiceStatus() Callback

The `onRefreshRecordingServiceStatus()` callback triggers when the recording service refreshes.

If `m_hMainWnd` is `NULL`, end the method.

Declare a new `LPAGE_RCDSRV_STATUS` object and set the status property `lpData->status`.

Display a recording service refreshed event message using `PostMessage()`. 

``` C++
void CAGEngineEventHandler::onRefreshRecordingServiceStatus(int status)
{
	if (m_hMainWnd == NULL)
		return;

	LPAGE_RCDSRV_STATUS lpData = new AGE_RCDSRV_STATUS;

	lpData->status = status;

	::PostMessage(m_hMainWnd, WM_MSGID(EID_REFREASH_RCDSRV), (WPARAM)lpData, 0);
}
```

## Resources

* You can find full API documentation at the [Document Center](https://docs.agora.io/en/).
* You can file bugs about this sample [here](https://github.com/AgoraIO/Agora-Windows-Tutorial-1to1/issues).

## Learn More

- 1 to 1 Video Tutorial for [Android](https://github.com/AgoraIO/Agora-Android-Tutorial-1to1)
- 1 to 1 Video Tutorial for [iOS/Swift](https://github.com/AgoraIO/Agora-iOS-Tutorial-Swift-1to1)
- 1 to 1 Video Tutorial for [MacOS](https://github.com/AgoraIO/Agora-macOS-Tutorial-Swift-1to1)

## License

This software is under the MIT License (MIT). [View the license](LICENSE.md).