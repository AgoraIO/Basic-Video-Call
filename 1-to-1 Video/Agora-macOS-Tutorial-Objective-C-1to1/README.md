# Agora macOS Tutorial for Objective-C - 1to1

*其他语言版本： [简体中文](README.zh.md)*

The Agora macOS Tutorial for Objective-C 1to1 Sample App is an open-source demo that will help you get video chat integrated directly into your macOS applications using the Agora Video SDK.

With this sample app, you can:

- Join / leave channel
- Mute / unmute audio
- Mute / unmute video
- Device selection
- Screen share

This demo is written in **Objective-C**, you can find **Swift** version here: [Agora-macOS-Tutorial-Swift-1to1](https://github.com/AgoraIO/Agora-macOS-Tutorial-Swift-1to1)

A full-fledged demo can be found here: [OpenVideoCall-macOS](https://github.com/AgoraIO/OpenVideoCall-macOS)

Agora Video SDK supports iOS / Android / Windows / macOS etc. You can find demos of these platform here:

- [Agora-iOS-Tutorial-Objective-C-1to1](https://github.com/AgoraIO/Agora-iOS-Tutorial-Objective-C-1to1)
- [Agora-Android-Tutorial-1to1](https://github.com/AgoraIO/Agora-Android-Tutorial-1to1)
- [Agora-Windows-Tutorial-1to1](https://github.com/AgoraIO/Agora-Windows-Tutorial-1to1)

## Running the App
First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID. Update "AppID.m" with your App ID.

```
NSString *const appID = @"Your App ID"; 
```

Next, download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the "libs" folder to the project folder.

Finally, Open Agora Mac Tutorial Objective-C.xcodeproj, setup your development signing and run.

## Developer Environment Requirements
* XCode 8.0 +

## Connect Us

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Agora-macOS-Tutorial-Objective-C-1to1/issues)

## License

The MIT License (MIT).
