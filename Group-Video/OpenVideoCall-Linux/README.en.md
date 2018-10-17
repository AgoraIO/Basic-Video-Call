# Open Video Call Linux

*其他语言版本： [简体中文](README.md)*

The Open Video Call Linux Sample App is an open-source demo that will help you get video chat integrated directly into your Linux applications using the Agora Video SDK.

With this sample app, you can:

- Join / leave channel
- Mute / unmute audio
- Enable / disable video
- Change camera
- Setup resolution, frame rate and bit rate

This demo is written in **C++**

Agora Video SDK supports iOS / Android / Windows / macOS etc. You can find demos of these platform here:

- [OpenVideoCall-Android](https://github.com/AgoraIO/OpenVideoCall-Android)
- [OpenVideoCall-iOS](https://github.com/AgoraIO/OpenVideoCall-iOS)
- [OpenVideoCall-macOS](https://github.com/AgoraIO/OpenVideoCall-macOS)
- [OpenVideoCall-Windows](https://github.com/AgoraIO/OpenVideoCall-Windows)

## Running the App
First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID. set appID in run.sh

```
--appId "Your app ID"
```

Next, download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/blog/download/).
Unzip the downloaded SDK package and copy
- libagora_rtc_sdk.so

to the "OpenVideoCall/libs" folder in project（the old one may be over written）.

configue parameters in run.sh.
Using 'make' command to build the demo. Using ./run.sh to run demo
openVideoCall is a commandline demo. Commands as follows:
- open
  open video call
- close
  close video call
- enable_video
  enable/disable video, 'enable_video 0' means disable video, 'enable_video 1' means enable video
- enable_audio
  enable/disable audio, 'enable_audio 0' means disable audio, 'enable_audio 1' means enable audio
- enable_local_video
  enable/disable local video, 'enable_local_video 0' means disable local video(local camera), 'enable_local_video 1' means enable local video
- mute_local_video
  mute/unmute local video, 'mute_local_video 0' means unmute local video(local camera), 'mute_local_video 1' means mute local video
- mute_local_audio
  mute/unmute local audio, 'mute_local_audio 0' means unmute local audio(local audio device), 'mute_local_audio 1' means mute local audio
- print_device_info
  print video and audio device info
- set_cur_camera
  set current carmera,'set_cur_camera deviceId' you can get deviceID by print_device_info
- exit
  exit app

## Developer Environment Requirements
* VC2013 or higher
* WIN7 or higher

## Connect Us

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can fire bugs about this demo at [issue](https://github.com/AgoraIO/OpenVideoCall-Windows/issues)

## License

The MIT License (MIT).
