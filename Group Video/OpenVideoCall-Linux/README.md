# Open Video Call Linux

This sample application enables you to quickly get started using the Agora Video SDK to integrate video chat into your Linux application.

With this sample app, you can:

- Join or leave a channel
- Mute or unmute audio
- Enable or disable video
- Change the camera
- Set up the resolution, frame rate, and bit rate


## Prerequisites
- Ubuntu 12.04+ x64 or CentOS 6.5+ x64 (CentOS 7+ recommended)
- GCC 4.4+
- ARS IP (public IP)
- 1MB+ bandwidth for each simultaneous recording channel
- Server access for `qos.agoralab.co` 

	**Note:** If server access is denied, the Agora SDK may fail to transfer the required data.

## Quick Start

This section shows you how to prepare and build the sample application.

- [Create an Account and Obtain an App ID](#create-an-account-and-obtain-an-app-id)
- [Integrate the Agora Video SDK](#integrate-the-agora-video-sdk)
- [Build and Run the Sample Application](#build-and-run-the-sample-application) 


### Create an Account and Obtain an App ID

1. Sign up for a developer account at [agora.io](https://dashboard.agora.io/signin/).
2. Upon logging in, use the dashboard tree on the left to navigate to **Projects** > **Project List**.
3. Save the App ID from the Agora dashboard. The App ID is needed to run the sample application.
4. Open *sample/OpenVideoCall/run.sh* file and add the App ID from the Agora dashboard.

```
./openVideoCall  --appId "YOUR-APP-ID" \
```


### Integrate the Agora Video SDK

1. Download the [Agora SDK](https://www.agora.io/en/download/).
2. Unzip the downloaded SDK package.
3. Copy the `libagora_rtc_sdk.so` file from the downloaded SDK package to the `libs` folder of the sample application.

### Build and Run the Sample Application 

Use the `make` command to build the sample application.

Type `./run.sh` to run the demo.

The sample is a command line application. Below are a list of available commands:

Command|Description|Options
---|---|---
`open`|Open a video call|N/A
`close`|Close the video call|N/A
`enable_video 0` / `enable_video 1`|Enable or disable video|`0` to disable video. `1` to enable video.
`enable_audio 0` / `enable_audio 1`|Enable or disable audio|`0` to disable audio. `1` to enable audio.
`enable_local_video 0` / `enable_local_video 1`|Enable or disable local video|`0` to disable local video (local camera). `1` to enable local video (local camera).
`mute_local_video 0` / `mute_local_video 1`|Mute or unmute local video| `0` to unmute local video (local camera). `1` to mute local video.
`mute_local_audio 0` / `mute_local_audio 1`|Mute or unmute local audio|`0` to unmute local audio (local audio device). `1` to mute local audio.
`print_device_info`|Print video and audio device information|N/A
`set_cur_camera deviceId`|Sets current camera|`deviceId` is the id of the camera device. You can retrieve the `deviceId` by using `print_device_info`.
`exit`|Exit the app|N/A


## Steps to Create the Sample

The sample application is written in C++.

The key code for the sample application is in the `sample/OpenVideoCall/EngineModel` folder. The following steps identify the code most relevant to using the Agora SDK.

- [Create the AGCameraManager Class](#create-the-agcameramanager-class)
- [Create the AGEngine Class](#create-the-agengine-class)
- [Create the AGEngineEventHandler Class](#create-the-agengineeventhandler-class)
- [Create the AGEngineModel Class](#create-the-agenginemodel-class)

### Create the AGCameraManager Class

The AGCameraManager class manages the camera devices.

- [Add the create() Method](#add-the-create-method)
- [Add the close() Method](#add-the-close-method)
- [Add the getDeviceCount() Method](#add-the-getdeviceCount-method)
- [Add the getCurDeviceId() Method](#add-the-getcurDeviceId-method)
- [Add the getDevice() Method](#add-the-getdevice-method)
- [Add the setCurDevice() Method](#add-the-setcurdevice-method)


#### Add the create() Method

The `create()` method initializes the class from the Agora `IRtcEngine` object.

1. Create an `AVideoDeviceManager` object.
2. If `m_deviceManager` is valid, retrieve the list of device objects using `(*m_deviceManager)->enumerateVideoDevices()`.
3. If `m_collection` is `NULL`, delete `m_deviceManager` and set it to `NULL`.
4. Complete the method by returning `true` if `m_collection` is not `NULL` and `false` if it is.

``` C++
bool AGCameraManager::create(IRtcEngine *rtcEngine) {
    m_deviceManager = new AVideoDeviceManager(rtcEngine);
    if (m_deviceManager == NULL || m_deviceManager->get() == NULL)
        return false;

    m_collection = (*m_deviceManager)->enumerateVideoDevices();
    if (m_collection == NULL) {
        delete m_deviceManager;
        m_deviceManager = NULL;
    }

    return m_collection != NULL ? true : false;
}
```

#### Add the close() Method

The `close()` method releases `m_collection` and `m_deviceManager` from memory using `delete` and sets them both to `NULL`.

``` C++
void AGCameraManager::close() {
    if (m_collection != NULL){
        m_collection->release();
        m_collection = NULL;
    }

    if (m_deviceManager != NULL) {
        delete m_deviceManager;
        m_deviceManager = NULL;
    }
}
```

#### Add the getDeviceCount() Method

The `getDeviceCount()` method returns the number of devices using `m_collection->getCount()`.

``` C++
uint8_t AGCameraManager::getDeviceCount() {
    if (m_collection == NULL)
        return 0;

    return (uint8_t)m_collection->getCount();
}
```

#### Add the getCurDeviceId() Method

The `getCurDeviceId()` method retrieves the device Id currently used by the application. 

1. Initialize `deviceId`.
2. Ensure `m_deviceManager` is not `NULL` and contains devices or return `false`.
3. Allocate memory for `deviceId` with a length of `MAX_DEVICE_ID_LENGTH`.
4. Retrieve the device Id using `(*m_deviceManager)->getDevice(deviceId)`.
5. Set `id` to `deviceId` using `id.assign(deviceId)` and return `true`.

``` C++
bool AGCameraManager::getCurDeviceId(string& id) {
    char deviceId[MAX_DEVICE_ID_LENGTH];

    if (m_deviceManager == NULL || m_deviceManager->get() == NULL)
        return false;

    memset(deviceId, 0x00, MAX_DEVICE_ID_LENGTH);
    if (m_deviceManager != NULL && m_deviceManager->get() != NULL)
        (*m_deviceManager)->getDevice(deviceId);

    id.assign(deviceId);

    return true;
}
```

#### Add the getDevice() Method

The `getDevice()` method retrieves the device at the location `index`. 

1. Initialize `name` and `Id`.
2. Ensure `index` is less than the number of devices and `m_collection` is not `NULL` or return `false`.
3. Retrieve the device for the specified `index` using `m_collection->getDevice()`.
4. Ensure the device is successfully retrieved by checking if `ret` equals `0`. Otherwise, return `false`.
5. Set `deviceName` to `name` and `deviceId` to `Id` using `assign()` and return `true`.

``` C++
bool AGCameraManager::getDevice(uint8_t index, string& deviceName, string& deviceId) {
    char name[MAX_DEVICE_ID_LENGTH];
    char id[MAX_DEVICE_ID_LENGTH];

    if (index >= getDeviceCount())
        return false;

    if(m_collection == NULL)
        return false;

    int ret = m_collection->getDevice(index, name, id);
    if (ret != 0)
        return false;

    deviceName.assign(name);
    deviceId.assign(id);

    return true;
}
```

#### Add the setCurDevice() Method

The `setCurDevice()` method sets the current device of the application to `deviceId`.

1. Ensure `m_deviceManager` is not `NULL` or return `false`.
2. Set the current device using `(*m_deviceManager)->setDevice()`.
3. Ensure the device is set by checking if `ret` equals `0` and return `true`. Otherwise, return `false`.

``` C++
bool AGCameraManager::setCurDevice(const char* deviceId)
{
    if (m_deviceManager == NULL || *m_deviceManager == NULL)
        return false;

    int ret = (*m_deviceManager)->setDevice(deviceId);

    return ret == 0 ? true : false;
}

```

### Create the AGEngine Class

The `AGEngine` class handles interaction with the Agora SDK.

- [Initialize the AGEngine Class](#initialize-the-agengine-class)
- [Create Engine Settings and Configuration Methods](#create-engine-settings-and-configuration-methods)
- [Create Log Methods](#create-log-methods)
- [Create Channel Methods](#create-channel-methods)
- [Create Video Methods](#create-video-methods)
- [Create Audio Methods](#create-audio-methods)
- [Add Testing Methods](#add-testing-methods)
- [Add the Message Methods](#add-the-message-methods)
- [Add the release() Method](#add-the-release-method)

#### Initialize the AGEngine Class

1. Create the engine object using `createAgoraRtcEngine()`.
2. Initialize a `RtcEngineContext` object and set the event handler using `ctx.eventHandler` and the App ID using `ctx.appId`.
3. Initialize the engine with `ctx` using `m_agoraEngine->initialize()`.
4. Initialize `m_parameters` with the `m_agoraEngine` using `new RtcEngineParameters()`.

``` C++
#include <stdio.h>
#include <cstring>

#include "IAgoraRtcEngine.h"
#include "AGEngine.h"

AGEngine::AGEngine(IRtcEngineEventHandler* handler, const char* appId)
{
    m_agoraEngine = createAgoraRtcEngine(); 
    RtcEngineContext ctx;
    ctx.eventHandler = handler;
    ctx.appId = appId;
    m_agoraEngine->initialize(ctx);

    m_parameters = new RtcEngineParameters(m_agoraEngine);
}
```
#### Create Engine Settings and Configuration Methods

The following methods retrieve the engine settings and update engine configurations.

- [Add the getSDKVersion() Method](#add-the-getsdkversion-method)
- [Add the getCallID() Method](#add-the-getcallid-method)
- [Add the enableWebSdkInteroperability() Method](#add-the-enablewebsdkinteroperability-method)
- [Add the setEncryptionSecret() Method](#add-the-setencryptionsecret-method)
- [Add the enableLocalRender() Method](#add-the-enablelocalrender-method)

##### Add the getSDKVersion() Method

The `getSDKVersion()` retrieves the version of the Agora SDK the application is using.

1. Ensure `m_agoraEngine` is not `NULL` or return `false`.
2. Initialize `buildNumber` to `0` and retrieve the SDK version number using `m_agoraEngine->getVersion()`.
3. Return the result `engineVer`.

``` C++
const char* AGEngine::getSDKVersion()
{
    if(m_agoraEngine == NULL)
        return false;

    int buildNumber = 0;
    const char *engineVer = m_agoraEngine->getVersion(&buildNumber);

    return engineVer;
}
```

##### Add the getCallID() Method

The `getCallID()` method retrieves the call ID from the Agora engine.

1. Initialize `uid`
2. Retrieve the call ID using `m_agoraEngine->getCallId()`.
3. Return the resulting value using `uid->c_str()`.

``` C++
const char* AGEngine::getCallID()
{
    agora::util::AString uid;

    m_agoraEngine->getCallId(uid);

    return uid->c_str();
}
```

##### Add the enableWebSdkInteroperability() Method

The `enableWebSdkInteroperability()` method enables or disables the ability to interact with the Agora Web SDK.

Create a new `RtcEngineParameters` object and initialize `ret` with a value of `1`.

Enable or disable the Web SDK by passing `enable` into the method `rep.enableWebSdkInteroperability()`.

If the enable or disable Web SDK is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::enableWebSdkInteroperability(bool enable)
{
    RtcEngineParameters rep(*m_agoraEngine);
    int ret = -1;
    ret = rep.enableWebSdkInteroperability(enable);

    return ret == 0 ? true : false;
}
```

##### Add the setEncryptionSecret() Method

The `setEncryptionSecret()` method sets the encryption type and encryption key used by the Agora engine.

Depending on `encryptType`, invoke `m_agoraEngine->setEncryptionMode()` with one of the following parameter values:

Encryption Type Value|Parameter Value|Description
---|---|---
`0`|`aes-128-xts`|AES 128 encryption 
`1`|`aes-256-xts`|AES 256 encryption 
default|`aes-128-xts`|AES 128 encryption 

Set the encryption key using `m_agoraEngine->setEncryptionSecret`.

If the encryption key and type are set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::setEncryptionSecret(const char* key, int encryptType)
{
    switch (encryptType)
    {
        case 0:
            m_agoraEngine->setEncryptionMode("aes-128-xts");
            break;
        case 1:
            m_agoraEngine->setEncryptionMode("aes-256-xts");
            break;
        default:
            m_agoraEngine->setEncryptionMode("aes-128-xts");
            break;
    }
    int ret = m_agoraEngine->setEncryptionSecret(key);

    return ret == 0 ? true : false;
}
```

##### Add the enableLocalRender() Method

The `enableLocalRender()` method enables local rendering. It is set to be enabled by default for the sample application, and therefore the return value is `true`.

``` C++
bool AGEngine::enableLocalRender(bool enable)
{
    int ret = 0;

    return ret == 0 ? true : false;
}
```

#### Create Log Methods

The log methods handle logging for the sample application.

##### Add the setLogFilePath() Method

The `setLogFilePath()` method sets the file path for saving logs.

1. Initialize a `RtcEngineParameters` object with `m_agoraEngine`.
2. Create a `fileName` string by appending `AgoraSdk.log` to `logPath`.
3. Set the log path for the engine using `rep.setLogFile()`.
4. Ensure the log path is set by checking if `ret` equals `0`, and return `true`. Otherwise, return `false`.

``` C++
bool AGEngine::setLogFilePath(const char* logPath)
{
    RtcEngineParameters rep(*m_agoraEngine);

    string fileName = string(logPath) + "AgoraSdk.log";
    int ret = rep.setLogFile(fileName.c_str());

    return ret == 0 ? true : false;
}
```

##### Add the setLogFilter() Method

The `setLogFilter()` sets the log filter `type` and file path `logPath`.

Create a new `RtcEngineParameters` object with `m_agoraEngine` and initialize `ret` with a value of `-1`.

Invoke `rep->setLogFilter()` with the `type` parameter to set the log filter type.

Invoke `rep->setLogFile()` with the `logPath` parameter to set the log file path.

If the log filter type and file path are set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::setLogFilter(int type, const char* logPath)
{
    int ret = -1;
    RtcEngineParameters rep(*m_agoraEngine);

    ret = rep.setLogFilter((agora::LOG_FILTER_TYPE)type);

    ret |= rep.setLogFile(logPath);

    return ret == 0 ? true : false;
}
```

#### Create Channel Methods

The channel methods manage the channel sessions for the sample application.

##### Add the joinChannel() Method

The `joinChannel()` method joins the use `uid` to the channel `channelId`.

Initialize a variable `ret` with a value of `1`.

If `m_agoraEngine` is valid:

1. Join the channel using `m_agoraEngine->joinChannel()`.
2. Ensure the channel is successfully joined by checking if `ret` equals `0` and initialize an `AParameter` object.
3. Set the local camera to `che.video.local.camera_index` at index `0` using `msp->setInt()`.
4. If the channel is successfully joined, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::joinChannel(const char* channelId, int uid)
{
    int ret = -1;
    if(m_agoraEngine) {
        ret = m_agoraEngine->joinChannel(NULL, channelId, NULL, uid);
        if(ret == 0) {
            AParameter msp(*m_agoraEngine);
            msp->setInt("che.video.local.camera_index", 0);
        }
    }

    return ret == 0 ? true : false;
}
```

##### Add the setChannelProfile() Method

The `setChannelProfile()` method set the profile for the channel.

Initialize `ret` with a value of `1`.

If the `profile` is valid:

1. Set the channel profile to `CHANNEL_PROFILE_LIVE_BROADCASTING` using `m_agoraEngine->setChannelProfile()`. This enables live broadcasting for the channel.
2. Ensure the channel `profile` is set by checking if `ret` equals `0`, and set the client role to `CLIENT_ROLE_BROADCASTER`using `m_agoraEngine->setClientRole()`. This enables the user to act as a broadcaster for the channel.

If the `profile` is not valid, set the channel profile to `CHANNEL_PROFILE_COMMUNICATION` using `m_agoraEngine->setChannelProfile()`. This enables general communication for the channel.

If the channel profile is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::setChannelProfile(int profile) {
    int ret = -1;
    if(profile){
        ret = m_agoraEngine->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
        if(ret == 0)
            ret = m_agoraEngine->setClientRole(CLIENT_ROLE_BROADCASTER);
    }
    else
        ret = m_agoraEngine->setChannelProfile(CHANNEL_PROFILE_COMMUNICATION);

    return ret == 0 ? true : false;
}

```

##### Add the leaveChannel() Method

The `leaveChannel()` method enables the ability to leave a channel.

Invoke the Agora SDK method `m_agoraEngine->leaveChannel()`.

If the channel is left successfully, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::leaveChannel()
{
    //    m_agoraEngine->stopPreview();
    int ret = m_agoraEngine->leaveChannel();

    return ret == 0 ? true : false;
}
```

#### Create Video Methods

The video methods manage video settings for the sample application.

##### Add the enableVideo() Method

The `enableVideo()` method enables or disables video.

Initialize `ret` with a value of `-1`.

* If enabling video, invoke `m_agoraEngine->enableVideo()`.
* If disabling video, invoke `m_agoraEngine->disableVideo()`.

If the enable or disable video is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::enableVideo(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->enableVideo();
    else
        ret = m_agoraEngine->disableVideo();

    return ret == 0 ? true : false;
}
```

##### Add the enableLocalVideo() Method

The `enableLocalVideo()` method enables or disables local video.

Create a new `RtcEngineParameters` object with `m_agoraEngine` and initialize `ret` with a value of `-1`.

Invoke `rep->enableLocalVideo()` with the `enable` parameter to enable or disable local video.

If the enable or disable local video is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::enableLocalVideo(bool enable)
{
    RtcEngineParameters rep(*m_agoraEngine);
    int ret = -1;
    ret = rep.enableLocalVideo(enable);

    return ret == 0 ? true : false;
}
```

##### Add the muteLocalVideo() Method

The `muteLocalVideo()` method turns the local video on or off.

Create a new `RtcEngineParameters` object with `m_agoraEngine` and initialize `ret` with a value of `-1`.

Invoke `rep->muteLocalVideoStream()` with the `mute` parameter to turn the local video on or off.

If turn local video on or off is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::muteLocalVideo(bool mute)
{
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteLocalVideoStream(mute);

    return ret == 0 ? true : false;
}
```

##### Add the muteRemoteVideo() Method

The `muteRemoteVideo()` method turns on or off the remote video for the user specified by `uid`.

Create a new `RtcEngineParameters` object for the engine `m_agoraEngine` and start or stop the remote video by passing `uid` and `mute` into the `rep.muteRemoteVideoStream()` method.

If starting or stopping the remote video is successful, return `true`. Otherwise, return `false`.

``` C++
bool AGEngine::muteRemoteVideo(int uid, bool mute){
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteRemoteVideoStream(uid, mute);

    return ret == 0 ? true : false;
}
```

##### Add the setVideoProfile() Method

The `setVideoProfile()` method sets the video profile for the engine by invoking `m_agoraEngine->setVideoProfile()` with the parameter `videoProfile`.

If setting the video profile is successful, return `true`. Otherwise, return `false`.

``` C++
bool AGEngine::setVideoProfile(int videoProfile)
{
    int ret = m_agoraEngine->setVideoProfile((VIDEO_PROFILE_TYPE)videoProfile, false);

    return ret == 0 ? true : false;
}
```


#### Create Audio Methods

The audio methods manage video settings for the sample application.

##### Add the enableAudio() Method

The `enableAudio()` method enables or disables audio.

Initialize `ret` with a value of `-1`.

* If enabling audio, invoke `m_agoraEngine->enableAudio()`.
* If disabling audio, invoke `m_agoraEngine->disableAudio()`.

If enable or disable audio is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::enableAudio(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->enableAudio();
    else
        ret = m_agoraEngine->disableAudio();

    return ret == 0 ? true : false;
}
```

##### Add the muteLocalAudio() Method

The `muteLocalAudio()` method mutes or unmutes audio.

Create a new `RtcEngineParameters` object with `m_agoraEngine` and initialize `ret` with a value of `-1`.

Invoke `rep->muteLocalAudioStream()` with the `mute` parameter to mute or unmute audio.

If mute or unmute audio is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::muteLocalAudio(bool mute)
{    
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteLocalAudioStream(mute);

    return ret == 0 ? true : false;
}
```

##### Add the muteRemoteAudio() Method

The `muteRemoteAudio()` method mutes or unmutes the remote audio for the user specified by `uid`.

Create a new `RtcEngineParameters` object for the engine `m_agoraEngine` and mute or unmute the remote audio by passing `uid` and `mute` into the `rep.muteRemoteAudioStream()` method.

If starting or stopping the remote audio is successful, return `true`. Otherwise, return `false`.


``` C++
bool AGEngine::muteRemoteAudio(int uid, bool mute){
    RtcEngineParameters rep(*m_agoraEngine);

    int ret = rep.muteRemoteAudioStream(uid, mute);

    return ret == 0 ? true : false;
}
```

##### Add the setAudioProfile() Method

The `setAudioProfile()` method sets the audio profile for the engine by invoking `m_agoraEngine-> setAudioProfile()` with the parameters `audioProfile` and `audioScenario`.

If setting the audio profile is successful, return `true`. Otherwise, return `false`.


``` C++
bool AGEngine::setAudioProfile(int audioProfile, int audioScenario)
{
    int ret = m_agoraEngine->setAudioProfile((AUDIO_PROFILE_TYPE)audioProfile, (AUDIO_SCENARIO_TYPE)audioScenario);

    return ret == 0 ? true : false;
}
```

#### Add Testing Methods

The testing methods enable and disable Agora SDK tests.

##### Add the enableNetworkTest() Method

The `enableNetworkTest()` method enables or disables the last mile network test.

Initialize `ret` with a value of `-1`.

* If enabling the last mile test, invoke `m_agoraEngine->enableLastmileTest()`.
* If disabling the last mile test, invoke `m_agoraEngine->disableLastmileTest()`.

If the enable or disable last mile test is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::enableNetworkTest(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->enableLastmileTest();
    else
        ret = m_agoraEngine->disableLastmileTest();

    return ret == 0 ? true : false;
}
```

##### Add the enableEchoTest() Method

The `enableEchoTest()` method enables or disables the echo test.

Initialize `ret` with a value of `-1`.

* If enabling the echo test, invoke `m_agoraEngine->startEchoTest()`.
* If disabling the echo test, invoke `m_agoraEngine->stopEchoTest()`.

If the enable or disable last mile test is set, return `true` by checking if `ret` equals `0`. Otherwise, return `false`.

``` C++
bool AGEngine::enableEchoTest(bool enable)
{
    int ret = -1;

    if (enable)
        ret = m_agoraEngine->startEchoTest();
    else
        ret = m_agoraEngine->stopEchoTest();

    return ret == 0 ? true : false;
}
```

#### Add the Message Methods

The message methods handle messages created and sent through the Agora SDK.

##### Add the createMessageStream() Method

The `createMessageStream()` method creates a message stream.

Initialize `dataStream` with a value of `0` and invoke the `m_agoraEngine->createDataStream()` method to create the data stream. 

Complete the method by returning the resulting `dataStream`.

``` C++
int AGEngine::createMessageStream()
{
    int dataStream = 0;
    m_agoraEngine->createDataStream(&dataStream, true, true);

    return dataStream;
}
```

##### Add the sendChatMessage() Method

The `sendChatMessage()` method sends a message specified by `chatMessage` to the stream specified by `streamId`.

Initialize `messageLen` with the size of `chatMessage` and invoke the `m_agoraEngine->sendStreamMessage()` with the parameters `streamId`, `chatMessage`, and `messageLen` method to send the message to the stream. 

If the message send is successful, return `true`. Otherwise, return `false`.

``` C++
bool AGEngine::sendChatMessage(int streamId, const char* chatMessage)
{
    int messageLen = strlen(chatMessage);

    int ret = m_agoraEngine->sendStreamMessage(streamId, chatMessage, messageLen);

    return ret == 0 ? true : false;
}
```

#### Add the release() Method

The `release()` method releases the Agora engine from memory for garbage collection.

If `m_agoraEngine` is not `NULL` invoke `m_agoraEngine->release()` and set its value to `NULL`.

If `m_parameters` exists, clear the variable using `delete` and set its value to `NULL`.

``` C++
bool AGEngine::release()
{
    if(m_agoraEngine != NULL) {
        m_agoraEngine->release();
        m_agoraEngine = NULL;
    }

    if(m_parameters) {
        delete m_parameters;
        m_parameters = NULL;
    }

    return true;
}
```

### Create the AGEngineEventHandler Class

The `AGEngineEventHandler` class manages the Agora engine event handlers and sends it to the receiver in the [`AGEngineModel` class](#create-the-agenginemodel-class)

When the `AGEngineEventHandler` class is initialized, set `m_receiver` to `NULL`.

``` C++
#include "AGEngineEventHandler.h"
#include "AGEventDef.h"
#include "IAGEventReceiver.h"
#include <cstring>
#include <iostream>

AGEngineEventHandler::AGEngineEventHandler()
{
    m_receiver = NULL;
}
```

- [Add the setEventReceiver() Method](#add-the-seteventreceiver-method)
- [Add Channel Callbacks](#add-channel-callbacks)
- [Add Warning and Error Callbacks](#add-warning-and-error-callbacks)
- [Add Audio and Video Callbacks](#add-audio-and-video-callbacks)
- [Add User Callbacks](#add-user-callbacks)
- [Add Connection Callbacks](#add-connection-callbacks)
- [Add Statistics and Status Callbacks](#add-statistics-and-status-callbacks)

#### Add the setEventReceiver() Method

The `setEventReceiver()` method sets `m_receiver` with the value `receiver `.

``` C++
void AGEngineEventHandler::setEventReceiver(IAGEventReceiver* receiver)
{
    m_receiver = receiver;
}
```

#### Add Channel Callbacks

The callbacks in this section trigger when changes to the channel occur.

##### Add the onJoinChannelSuccess() Callback

The `onJoinChannelSuccess()` callback triggers when a user of `uid` successfully joins a `channel`.

Ensure `m_receiver` is not `NULL` before submitting the join success data:

1. Declare a new `joinChannelSuccessData` object.
2. Set `data.channel` to the value `std::string(channel)`.
3. Set `data.elapsed` to the value `elapsed`.
4. Set `data.uid` to the value `uid`.
5. Trigger the `EID_JOINCHANNEL_SUCCESS` event for the receiver with the user, channel, and time elapsed data using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    if(m_receiver != NULL) {
        joinChannelSuccessData data;
        data.channel = std::string(channel);
        data.elapsed = elapsed;
        data.uid = uid;
        m_receiver->onEvent(EID_JOINCHANNEL_SUCCESS, &data);
    }
}
```

##### Add the onRejoinChannelSuccess() Callback

The `onRejoinChannelSuccess()` callback triggers when a user of `uid` successfully re-joins a `channel`.

Ensure `m_receiver` is not `NULL` before submitting the re-join success data:

1. Declare a new `rejoinChannelSuccessData ` object.
2. Set `data.channel` to the value `std::string(channel)`.
3. Set `data.elapsed` to the value `elapsed`.
4. Set `data.uid` to the value `uid`.
5. Trigger the `EID_REJOINCHANNEL_SUCCESS` event for the receiver with the user, channel, and time elapsed data using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
    if(m_receiver != NULL) {
        rejoinChannelSuccessData data;
        data.channel = std::string(channel);
        data.elapsed = elapsed;
        data.uid = uid;
        m_receiver->onEvent(EID_REJOINCHANNEL_SUCCESS, &data);
    }
}
```

##### Add the onLeaveChannel() Callback

The `onLeaveChannel()` callback triggers when the channel is left.

Ensure `m_receiver` is not `NULL` before submitting the leave channel data:

1. Declare a new `leaveChannelData` object.
2. Set `data.stat` to the value `stat`.
3. Trigger the `EID_LEAVE_CHANNEL` event for the receiver with the statistic information using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
    if(m_receiver != NULL) {
        leaveChannelData data;
        data.stat = stat;
        m_receiver->onEvent(EID_LEAVE_CHANNEL, &data);
    }
}
```

#### Add Warning and Error Callbacks

The callbacks in this section trigger when the warnings and errors occur.

##### Add the onWarning() Callback

The `onWarning()` callback triggers when a warning occurs from the Agora engine.

Ensure `m_receiver` is not `NULL` before submitting the warning data:

1. Declare a new `warningData` object.
2. Set `data.msg` to the value `std::string(msg)` if `msg` is not `NULL`. Otherwise, set `on warning` as the value.
3. Set `data.warn` to the value `warn`.
4. Trigger the `EID_WARNING` event for the receiver with the warning message and type using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onWarning(int warn, const char* msg)
{
    if(m_receiver != NULL) {
        warningData data;
        data.msg = (msg == NULL)? "on warning" : std::string(msg); 
        data.warn = warn;
        m_receiver->onEvent(EID_WARNING, &data);
    }
}
```

##### Add the onError() Callback

The `onError()` callback triggers when an error occurs from the Agora engine.

Ensure `m_receiver` is not `NULL` before submitting the error data:

1. Declare a new `errorData` object.
2. Set `data.err` to the value `err`.
3. Set `data.msg` to the value `std::string(msg)` if `msg` is not `NULL`. Otherwise, set `on error` as the value.
4. Trigger the `EID_ERROR ` event for the receiver with the error message and type using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onError(int err, const char* msg)
{
    if(m_receiver != NULL) {
        errorData data;
        data.err = err;
        data.msg = (msg == NULL)? "on error" : std::string(msg); 
        m_receiver->onEvent(EID_ERROR, &data);
    }
}
```

#### Add User Callbacks

The callback methods in this section are triggered by user actions.

- [Add the onUserJoined() Callback](#add-the-onuserjoined-callback)
- [Add the onUserOffline() Callback](#add-the-onuseroffline-callback)
- [Add the onUserMuteAudio() Callback](#add-the-onusermuteaudio-callback)
- [Add the onUserMuteVideo() Callback](#add-the-onusermutevideo-callback)
- [Add the onUserEnableVideo() Callback](#add-the-onuserenablevideo-callback)

##### Add the onUserJoined() Callback

The `onUserJoined()` callback triggers when a user of `uid` joins.

Ensure `m_receiver` is not `NULL` before submitting the join success data:

1. Declare a new `userJoinedData` object.
2. Set `data.uid` to the value `uid`.
3. Set `data.elapsed` to the value `elapsed`.
4. Trigger the `EID_USER_JOINED` event for the receiver with the user and time elapsed data using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
    if(m_receiver != NULL) {
        userJoinedData data;
        data.uid = uid;
        data.elapsed = elapsed;
        m_receiver->onEvent(EID_USER_JOINED, &data);
    }
}
```

##### Add the onUserOffline() Callback

The `onUserOffline()` callback triggers when a user of `uid` goes offline.

Ensure `m_receiver` is not `NULL` before submitting the offline user data:

1. Declare a new `userOfflineData` object.
2. Set `data.uid` to the value `uid`.
3. Set `data.reason` to the value `reason`.
4. Trigger the `EID_USER_OFFLINE` event for the receiver with the user and reason data using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
    if(m_receiver != NULL) {
        userOfflineData data;
        data.uid = uid;
        data.reason = reason;
        m_receiver->onEvent(EID_USER_OFFLINE, &data);
    }
}
```

##### Add the onUserMuteAudio() Callback

The `onUserMuteAudio()` callback triggers when a user of `uid` mutes or unmutes audio.

Ensure `m_receiver` is not `NULL` before submitting the mute or unmute data:

1. Declare a new `userMuteAudioData` object.
2. Set `data.uid` to the value `uid`.
3. Set `data.muted` to the value `muted`.
4. Trigger the `EID_USER_MUTE_AUDIO` event for the receiver with the user and mute or unmute data using `m_receiver->onEvent()`.


``` C++
void AGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{
    if(m_receiver != NULL) {
        userMuteAudioData data;
        data.uid = uid;
        data.muted = muted;
        m_receiver->onEvent(EID_USER_MUTE_AUDIO, &data);
    }
}
```

##### Add the onUserMuteVideo() Callback

The `onUserMuteVideo()` callback triggers when a user of `uid` turns the video on or off.

Ensure `m_receiver` is not `NULL` before submitting the video on or off data:

1. Declare a new `userMuteVideoData` object.
2. Set `data.uid` to the value `uid`.
3. Set `data.muted` to the value `muted`.
4. Trigger the `EID_USER_MUTE_VIDEO` event for the receiver with the user and mute or unmute data using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{
    if(m_receiver != NULL) {
        userMuteVideoData data;
        data.uid = uid;
        data.muted = muted;
        m_receiver->onEvent(EID_USER_MUTE_VIDEO, &data);
    }
}
```

##### Add the onUserEnableVideo() Callback

The `onUserEnableVideo()` callback triggers when a user of `uid` enables or disables video.

Ensure `m_receiver` is not `NULL` before submitting the enable or disable video data:

1. Declare a new `userEnableVideoData` object.
2. Set `data.uid` to the value `uid`.
3. Set `data.enabled` to the value `enabled`.
4. Trigger the `EID_USER_ENABLE_VIDEO` event for the receiver with the user and enable or disable data using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{
    if(m_receiver != NULL) {
        userEnableVideoData data;
        data.uid = uid;
        data.enabled = enabled;
        m_receiver->onEvent(EID_USER_ENABLE_VIDEO, &data);
    }
}
```


#### Add Connection Callbacks

The callbacks in this section trigger when changes to the connection occur.

##### Add the onConnectionLost() Callback

The `onConnectionLost()` callback triggers when the connection is lost.

Ensure `m_receiver` is not `NULL` before triggering the `EID_CONNECTION_LOST` event for the receiver using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onConnectionLost()
{
    if(m_receiver != NULL)
        m_receiver->onEvent(EID_CONNECTION_LOST, NULL);
}
```

##### Add the onConnectionInterrupted() Callback

The `onConnectionInterrupted()` callback triggers when the connection is interrupted.

Ensure `m_receiver` is not `NULL` before triggering the `EID_CONNECTION_INTERRUPTED` event for the receiver using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onConnectionInterrupted()
{
    if(m_receiver != NULL)
        m_receiver->onEvent(EID_CONNECTION_INTERRUPTED, NULL);
}
```

#### Add Statistics and Status Callbacks

The callbacks in this section trigger when statistics are received, or status changes in the application are detected.

##### Add the onRtcStats() Callback

The `onRtcStats()` callback triggers when statistics are received from the Agora engine.

Ensure `m_receiver` is not `NULL` before triggering the `EID_RTC_STAT` event using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onRtcStats(const RtcStats& stat)
{
    if(m_receiver != NULL) 
        m_receiver->onEvent(EID_RTC_STAT, NULL);
}
```

##### Add the onAudioQuality() Callback

The `onAudioQuality()` callback triggers when the audio `quality` changes for a user with `uid`.

Ensure `m_receiver` is not `NULL` before submitting the audio quality data:

1. Declare a new `audioQualityData` object.
2. Set `data.uid` to the value `uid`.
3. Set `data.quality` to the value `quality`.
4. Set `data.delay` to the value `delay`.
5. Set `data.lost` to the value `lost`.
6. Trigger the `EID_AUDIO_QUALITY` event for the receiver with the user, audio quality, delay, and lost information using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost)
{
    if(m_receiver != NULL) {
        audioQualityData data;
        data.uid = uid;
        data.quality = quality;
        data.delay = delay;
        data.lost = lost;
        m_receiver->onEvent(EID_AUDIO_QUALITY, &data); 
    }
} 
```

##### Add the onCameraReady() Callback

The `onCameraReady()` callback triggers when the camera device is ready.

Ensure `m_receiver` is not `NULL` before triggering the `EID_CAMERA_READY` event for the receiver using `m_receiver->onEvent()`.

``` C++
void AGEngineEventHandler::onCameraReady()
{
    if(m_receiver != NULL)
        m_receiver->onEvent(EID_CAMERA_READY, NULL);
}
```


### Create the AGEngineModel Class

The `AGEngineModel` class manages event listeners for the sample application.

The `Get()` method returns `m_model`, the declared `AGEngineModel` object.

``` C++
AGEngineModel AGEngineModel::m_model;

AGEngineModel* AGEngineModel::Get() {
    return &m_model;
}
```

- [Create Initialization Methods](#create-initialization-methods)
- [Create Channel Methods and Callbacks](#create-channel-methods-and-callbacks)
- [Create Video Callbacks](#create-video-callbacks)
- [Create Audio Callbacks](#create-audio-callbacks)
- [Create Configuration and Device Callbacks](#create-configuration-and-device-callbacks)
- [Create Camera Callbacks](#create-camera-callbacks)
- [Create Application Exit Callback](#create-application-exit-callback)

#### Create Initialization Methods

The methods in this section initialize the class settings and default variable values.

##### Add the AGEngineModel() Method

The `AGEngineModel()` method maps event listener constants to their associated callbacks using `registerHandler()` and sets the event receiver using `m_engineEventHandler.setEventReceiver()`.

Event|Callback|Description
---|---|---
`MSG_OPEN`|`onOpenMsg`|Sets the configuration for the Agora engine and joins the user to the channel.
`MSG_CLOSE`|`onCloseMsg`|Closes the camera manager and leaves the channel.
`MSG_CONFIGURE`|`onConfigureMsg`|Triggers when the engine configurations change.
`MSG_ENABLE_VIDEO`|`onEnableVideoMsg`|Triggers when the video is enabled.
`MSG_ENABLE_AUDIO`|`onEnableAudioMsg`|Triggers when the audio is enabled.
`MSG_ENABLE_LOCAL_VIDEO`|`onEnableLocalVideoMsg`|Triggers when the local video is enabled.
`MSG_MUTE_LOCAL_VIDEO`|`onMuteLocalVideoMsg`|Triggers when the local video is turned off.
`MSG_MUTE_LOCAL_AUDIO`|`onMuteLocalAudioMsg`|Triggers when the local audio is turned off.
`MSG_PRINT_DEVICE_INFO`|`onPrintDeviceInfoMsg`|Triggers when the device information needs to be displayed.
`MSG_SET_CUR_CAMERA`|`onSetCurCameraMsg`|Triggers when the current camera is set.
`MSG_EXIT`|`onExitMsg`|Triggers when the application is exited.
`MSG_MUTE_REMOTE`|`onMuteRemote`|Triggers when the remote video is turned off.

``` C++
AGEngineModel::AGEngineModel() {

    registerHandler(MSG_OPEN,(handler_ptr)&AGEngineModel::onOpenMsg);
    registerHandler(MSG_CLOSE, (handler_ptr)&AGEngineModel::onCloseMsg);
    registerHandler(MSG_CONFIGURE, (handler_ptr)&AGEngineModel::onConfigureMsg);
    registerHandler(MSG_ENABLE_VIDEO, (handler_ptr)&AGEngineModel::onEnableVideoMsg);
    registerHandler(MSG_ENABLE_AUDIO, (handler_ptr)&AGEngineModel::onEnableAudioMsg);
    registerHandler(MSG_ENABLE_LOCAL_VIDEO, (handler_ptr)&AGEngineModel::onEnableLocalVideoMsg);
    registerHandler(MSG_MUTE_LOCAL_VIDEO, (handler_ptr)&AGEngineModel::onMuteLocalVideoMsg);
    registerHandler(MSG_MUTE_LOCAL_AUDIO, (handler_ptr)&AGEngineModel::onMuteLocalAudioMsg);
    registerHandler(MSG_PRINT_DEVICE_INFO, (handler_ptr)&AGEngineModel::onPrintDeviceInfoMsg);
    registerHandler(MSG_SET_CUR_CAMERA, (handler_ptr)&AGEngineModel::onSetCurCameraMsg);
    registerHandler(MSG_EXIT, (handler_ptr)&AGEngineModel::onExitMsg);
    registerHandler(MSG_MUTE_REMOTE, (handler_ptr)&AGEngineModel::onMuteRemote);

    m_engineEventHandler.setEventReceiver(this);
}
```

##### Add the initialize() Method

The `initialize()` method initializes the Agora engine and the Agora camera manager.

If `m_engine` does not exist yet, create a new `AGEngine` object with the `m_engineEventHandler` event handler, and set `m_cameraMgr` with a new `AGCameraManager` object.

``` C++
void AGEngineModel::initialize() {
    if(!m_engine){
        m_engine = new AGEngine(&m_engineEventHandler, m_cfg.appId.c_str());
    }

    m_cameraMgr = new AGCameraManager();
}
```

#### Create Channel Methods and Callbacks

The methods in this section handle joining and leaving a channel and its callbacks.

##### Add the onOpenMsg() Method

The `onOpenMsg()` method sets the configuration for the Agora engine and joins the user to the channel.

Set the engine configuration settings:

- Set the channel profile with `m_cfg.channelProfile` using `m_engine->setChannelProfile()`
- Enable or disable video with `m_cfg.enableVideo` using `m_engine->enableVideo()`
- Enable or disable audio with `m_cfg.enableAudio ` using `m_engine->enableAudio()`
- Enable or disable local video with `m_cfg.enableLocalVideo ` using `m_engine->enableLocalVideo()`
- Turn on or off local video with `m_cfg.muteLocalVideo ` using `m_engine->muteLocalVideo()`
- Mute or unmute local audio with `m_cfg.muteLocalAudio ` using `m_engine->muteLocalAudio()`
- Set the video profile with `m_cfg.videoProfile ` using `m_engine->setVideoProfile()`
- Set the audio profile with `m_cfg.audioProfile` and `m_cfg.audioScenario` using `m_engine->setAudioProfile()`
- Enable or disable interactions with the Agora web SDK with `m_cfg.enableWebSdkInteroperability` using `m_engine->enableWebSdkInteroperability()`

Join the user `m_cfg.uid` to the channel `m_cfg.channelId.c_str()` using `m_engine->joinChannel()`. Return the result `ret`.

``` C++
bool AGEngineModel::onOpenMsg(void* msg) {
    cout << "AgoraRtcEngine:open" <<endl;

    m_engine->setChannelProfile(m_cfg.channelProfile);

    m_engine->enableVideo(m_cfg.enableVideo);

    m_engine->enableAudio(m_cfg.enableAudio);

    m_engine->enableLocalVideo(m_cfg.enableLocalVideo);

    m_engine->muteLocalVideo(m_cfg.muteLocalVideo);

    m_engine->muteLocalAudio(m_cfg.muteLocalAudio);

    m_engine->setVideoProfile(m_cfg.videoProfile);

    m_engine->setAudioProfile(m_cfg.audioProfile, m_cfg.audioScenario);

    m_engine->enableWebSdkInteroperability(m_cfg.enableWebSdkInteroperability);

    int ret = m_engine->joinChannel(m_cfg.channelId.c_str(), m_cfg.uid); 

    return ret;
}
```

##### Add the onCloseMsg() Method

The `onCloseMsg()` method closes the camera manager and leaves the channel.

Display the close message to the terminal using `cout <<`.

Close the camera manager using `m_cameraMgr->close()` and leave the channel using `m_engine->leaveChannel()`.

``` C++
bool AGEngineModel::onCloseMsg(void* msg) {
    cout << "AgoraRtcEngine:close" <<endl;

    m_cameraMgr->close();
    return m_engine->leaveChannel(); 
}
```

##### Add the onEvent() Method

The `onEvent()` method triggers when a user joins a channel.

If `id` matches `EID_JOINCHANNEL_SUCCESS`, invoke `m_cameraMgr->create()`.

Initialize a new `eventWrapper` object and set `wrapper.id` with `id` and `wrapper.data` with `pData`.

If `m_controller` exists, send a message using `m_controller->sendMsg()`.

``` C++
void AGEngineModel::onEvent(int id, void* pData) {
    switch(id) {
        case EID_JOINCHANNEL_SUCCESS: 
            m_cameraMgr->create(m_engine->getRtcEngine());
            break;
        default:
            break;
    }
    eventWrapper wrapper;
    wrapper.id = id;
    wrapper.data = pData;
    
    if(m_controller)
        m_controller->sendMsg(this, MSG_RTC_EVENT, (void*)&wrapper);
}

```

#### Create Video Callbacks

The methods in this section handle callbacks triggered by video changes.

##### Add the onEnableVideoMsg() Method

The `onEnableVideoMsg()` method triggers when the video is enabled.

Convert `msg` to a boolean object `enable`.

- If `enable` does not match the enable video configuration `m_cfg.enableVideo`, update the configuration and enable the video using `m_engine->enableVideo()` and display the `enable video` message to the terminal using `cout <<`.
- If `enable` does match the enable video configuration `m_cfg.enableVideo`, the video is already enabled. Display the `already enabled video` message to the terminal using `cout <<`.

``` C++
bool AGEngineModel::onEnableVideoMsg(void* msg) {
    bool enable = *(reinterpret_cast<bool*>(msg));

    if(enable != m_cfg.enableVideo) {
        m_cfg.enableVideo =  enable;
        m_engine->enableVideo(enable);
        cout << "AgoraRtcEngine: enable video: " << enable <<endl;
    } else {
        cout << "AgoraRtcEngine: already enabled video: " << enable <<endl;
    }

    return true;
}
```

##### Add the onEnableLocalVideoMsg() Method

The `onEnableLocalVideoMsg()` method triggers when the local video is enabled.

Convert `msg` to a boolean object `enable`.

- If `enable` does not match the enable local video configuration `m_cfg.enableLocalVideo`, update the configuration and enable the local video using `m_engine->enableLocalVideo()` and display the `enable local video` message to the terminal using `cout <<`.
- If `enable` does match the enable local video configuration `m_cfg.enableLocalVideo`, the local video is already enabled. Display the `already enabled local video` message to the terminal using `cout <<`.

``` C++
bool AGEngineModel::onEnableLocalVideoMsg(void* msg) {
    bool enable = *(reinterpret_cast<bool*>(msg));

    if(enable != m_cfg.enableLocalVideo) {
        m_cfg.enableLocalVideo =  enable;
        m_engine->enableLocalVideo(enable);
        cout << "AgoraRtcEngine: enable local video: " << enable <<endl;
    } else {
        cout << "AgoraRtcEngine: already enabled local video: " << enable <<endl;
    }

    return true;
}
```

##### Add the onMuteLocalVideoMsg() Method

The `onMuteLocalVideoMsg()` method triggers when the local video is turned off.

Convert `msg` to a boolean object `mute`.

- If `mute` does not match the turn on or off local video configuration `m_cfg.muteLocalVideo`, update the configuration and turn off the local video using `m_engine->muteLocalVideo()` and display the `mute local video` message to the terminal using `cout <<`.
- If `mute` does match the turn on or off local video configuration `m_cfg.muteLocalVideo`, the local video is already turned off. Display the `already muted local video` message to the terminal using `cout <<`.

``` C++
bool AGEngineModel::onMuteLocalVideoMsg(void* msg) {
    bool mute = *(reinterpret_cast<bool*>(msg));

    if(mute != m_cfg.muteLocalVideo) {
        m_cfg.muteLocalVideo =  mute;
        m_engine->muteLocalVideo(m_cfg.muteLocalVideo);
        cout << "AgoraRtcEngine: mute local video: " << mute <<endl;
    } else {
        cout << "AgoraRtcEngine: already muted local video: " << mute <<endl;
    }

    return true;
}
```

##### Add the onMuteRemote() Method

The `onMuteRemote()` method triggers when the remote video is turned off.

Convert `msg` to a `muteRemoteMsg` object.

Check if the message is video or audio using `muteMsg->isVideo`:

- If the `muteMsg` is video, set and return the remote video with the `muteMsg->uid` and `muteMsg->mute` using `m_engine->muteRemoteVideo()`.
- If the `muteMsg` is audio, set and return the remote audio with the `muteMsg->uid` and `muteMsg->mute` using `m_engine-> muteRemoteAudio()`.

``` C++
bool AGEngineModel::onMuteRemote(void* msg) {
    muteRemoteMsg* muteMsg = reinterpret_cast<muteRemoteMsg*>(msg);
    if(muteMsg->isVideo)
        return m_engine->muteRemoteVideo(muteMsg->uid, muteMsg->mute);
    else
        return m_engine->muteRemoteAudio(muteMsg->uid, muteMsg->mute);
}
```

#### Create Audio Callbacks

The methods in this section handle callbacks triggered by audio changes.

##### Add the onEnableAudioMsg() Method

The `onEnableAudioMsg()` method triggers when the audio is enabled.

Convert `msg` to a boolean object `enable`.

- If `enable` does not match the enable audio configuration `m_cfg.enableAudio `, update the configuration and enable the audio using `m_engine-> enableAudio()` and display the `enable audio` message to the terminal using `cout <<`.
- If `enable` does match the enable audio configuration `m_cfg.enableAudio `, the audio is already enabled. Display the `already enabled audio` message to the terminal using `cout <<`.

``` C++
bool AGEngineModel::onEnableAudioMsg(void* msg) {
    bool enable = *(reinterpret_cast<bool*>(msg));

    if(enable != m_cfg.enableAudio) {
        m_cfg.enableAudio =  enable;
        m_engine->enableAudio(enable);
        cout << "AgoraRtcEngine: enable audio: " << enable <<endl;
    } else {
        cout << "AgoraRtcEngine: already enabled audio: " << enable <<endl;
    }

    return true;
}
```

##### Add the onMuteLocalAudioMsg() Method

The `onMuteLocalAudioMsg()` method triggers when the local audio is turned off.

Convert `msg` to a boolean object `mute`.

- If `mute` does not match the turn on or off local audio configuration `m_cfg.muteLocalAudio`, update the configuration and turn off the local audio using `m_engine->muteLocalAudio()` and display the `mute local audio` message to the terminal using `cout <<`.
- If `mute` does match the turn on or off local audio configuration `m_cfg.muteLocalAudio`, the local audio is already turned off. Display the `already muted local audio` message to the terminal using `cout <<`.

``` C++
bool AGEngineModel::onMuteLocalAudioMsg(void* msg) {
    bool mute = *(reinterpret_cast<bool*>(msg));

    if(mute != m_cfg.muteLocalAudio) {
        m_cfg.muteLocalAudio =  mute;
        m_engine->muteLocalAudio(m_cfg.muteLocalAudio);
        cout << "AgoraRtcEngine: mute local audio: " << mute <<endl;
    } else {
        cout << "AgoraRtcEngine: already muted local audio: " << mute <<endl;
    }

    return true;
}
```


#### Create Configuration and Device Callbacks

The methods in this section handle callbacks triggered by configuration and device changes.

##### Add the onConfigureMsg() Method

The `onConfigureMsg()` method triggers when the engine configurations change.

Convert `msg` to an `AppConfig` object and set it as the value for `m_cfg`. Display the `model onConfigureMsg` message using `cout <<` and return `true`.

``` C++
bool AGEngineModel::onConfigureMsg(void* msg) {
    AppConfig* cfg = reinterpret_cast<AppConfig*>(msg);
    m_cfg = *cfg;

    cout << "model onConfigureMsg"<<endl;
    return true;
}
```

##### Add the onPrintDeviceInfoMsg() Method

The `onPrintDeviceInfoMsg()` method triggers when the device information needs to be displayed.

Display the following messages using `cout <<`.

Key message|Description
---|---
`model onPrintDeviceInfoMsg`|Text communicating device information is about to be displayed.
`cameraCnt`|Number of camera devices.
`camera device list:`|Text communicating a list of devices is about to be displayed.
`deviceName` and `deviceId`|Used when iterating through each device to display the device name and device ID.

Retrieve the current camera device ID using `m_cameraMgr->getCurDeviceId()` and print `curDeviceId` to the terminal using `cout <<`.

``` C++
bool AGEngineModel::onPrintDeviceInfoMsg(void* msg) {
    cout << "model onPrintDeviceInfoMsg"<<endl;

    int cameraCnt = m_cameraMgr->getDeviceCount();
    cout <<"camera device number is:" << cameraCnt<< endl;
    cout <<"camera device list:" << endl;
    for(int i = 0; i < cameraCnt; ++i) {
        string deviceId;
        string deviceName;
        if(m_cameraMgr->getDevice(i , deviceName, deviceId)) {
            cout <<"device index: "<< i<<",  device name: "<< deviceName <<",  device Id: " << deviceId << endl;
        }
    }
    string curDeviceId;
    if(m_cameraMgr->getCurDeviceId(curDeviceId)) 
        cout <<"current camera device id is:" << curDeviceId << endl;

    return true;
}
```

#### Create Camera Callbacks

##### Add the onSetCurCameraMsg() Method

The `onSetCurCameraMsg()` method triggers when the current camera is set.

Convert `msg` to a `string` object. If `m_cameraMgr` does not exist, return `false`.

Set the current device with `deviceId->c_str()` using `m_cameraMgr->setCurDevice()`.

``` C++
bool AGEngineModel::onSetCurCameraMsg(void* msg) {
    string* deviceId = reinterpret_cast<string*>(msg);
    if(!m_cameraMgr)
        return false;

    return m_cameraMgr->setCurDevice(deviceId->c_str());
}
```

##### Add the release() Method

The `release()` method releases the camera manager and engine from memory.

- If `m_cameraMgr` exists, remove it using `delete` and set it to `NULL`.
- If `m_engine` exists, remove it using `delete` and set it to `NULL`.

``` C++
void AGEngineModel::release() {
    if(m_cameraMgr) {
        delete m_cameraMgr;
        m_cameraMgr = NULL;
    }

    if(m_engine) {
        delete m_engine;
        m_engine = NULL;
    }
}

```

#### Create Application Exit Callback

##### Add the onExitMsg() Method

The `onExitMsg()` method triggers when the application is exited.

1. Close the channel using `m_cameraMgr->close()`.
2. Leave the channel using `m_engine->leaveChannel()`.
3. Release the engine from memory for garbage collection using `m_engine->release()`.
4. Release remaining application memory for garbage collection using `release()`.

``` C++
bool AGEngineModel::onExitMsg(void* msg) {
    m_cameraMgr->close();
    m_engine->leaveChannel();
    m_engine->release();

    release();

    return true;
}
```

## Resources
- Full API documentation in the [Document Center](https://docs.agora.io/en/)
- File bugs about this sample [here](https://github.com/AgoraIO/OpenVideoCall-Linux/issues)
- Detailed Agora Linux Recording guides [here](https://docs.agora.io/en/2.3.1/addons/Recording/Quickstart%20Guide/recording_cpp?platform=C%2B%2B)


## Learn More
Agora Video SDK samples are also available for the following platforms:
	- OpenVideoCall for [Android](https://github.com/AgoraIO/OpenVideoCall-Android)
	- OpenVideoCall for [iOS](https://github.com/AgoraIO/OpenVideoCall-iOS)
	- OpenVideoCall for [MacOS](https://github.com/AgoraIO/OpenVideoCall-macOS)
	- OpenVideoCall for [Windows](https://github.com/AgoraIO/OpenVideoCall-Windows)

## License
This software is licensed under the MIT License (MIT). [View the license](LICENSE.md).