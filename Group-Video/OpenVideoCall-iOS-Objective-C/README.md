# Open Video Call iOS for Objective-C

*English | [中文](README.zh.md)*

The Open Video Call iOS for Objective-C Sample App is an open-source demo that will help you get video chat integrated directly into your iOS applications using the Agora Video SDK.

With this sample app, you can:
- Join / leave channel
- Mute / unmute audio
- Enable / disable video
- Switch camera
- Setup resolution, frame rate and bit rate

## Prerequisites

- Xcode 10.0+
- Physical iOS device (iPhone or iPad)
- iOS simulator is NOT supported

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Copy the **App ID** from the Dashboard.

4. Open `OpenVideoCall.xcodeproj` and edit the `KeyCenter.m` file. Update `<#Your App Id#>` with your app ID.

    ```
    + (NSString *)AppId {
        return @"Your App ID";
    }
    ```

### Integrate the Agora Video SDK

1. Download the [Agora Video SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the following files from the SDK `libs` folder into the sample application `` folder.
    - `AograRtcEngineKit.framework`
    - `AgoraRtcCryptoLoader.framework`
    - `libcrypto.a`
  
2. Connect your iPhone or iPad device and run the project. Ensure a valid provisioning profile is applied or your project will not run.


## Resources

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
