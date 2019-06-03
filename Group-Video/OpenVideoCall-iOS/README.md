# Open Video Call iOS for Swift

*English | [中文](README.zh.md)*

This tutorial describes how to add video chat to your iOS applications using Swift and the Agora Video SDK.

With this sample app, you can:
- [Join/leave a channel](#create-the-dojoinpressed-ibaction-method)
- [Mute/unmute audio](#video-and-audio-methods)
- [Enable/disable video](#video-and-audio-methods)
- [Switch camera views](#camera-speaker-filter-and-close-methods)
- [Send a message to a channel](#create-the-send-method)
- [Set up the resolution, the frame rate, and the bit rate display](#create-the-remotevideostats-event-listener)
- [Enable encryption](#create-the-loadagorakit-method)
- [Enable/disable the black-and-white filter](#video-and-audio-methods)

## Prerequisites

- Xcode 10.0+
- Physical iOS device (iPhone or iPad)

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/).
2. In the Dashboard that opens, click **Projects** > **Project List** in the left navigation.
3. Copy the **App ID** from the Dashboard.

4. Open `OpenVideoCall.xcodeproj` and edit the `KeyCenter.swift` file. In the `agoraKit` declaration, update `<#Your App Id#>` with your app ID.

    ``` Swift
    static let AppId: String = <#Your App Id#>
    ```

### Integrate the Agora Video SDK


1. Download the [Agora Video SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the following files from the SDK `libs` folder into the sample application `OpenVideoCall` folder.
    - `AograRtcEngineKit.framework`
    - `AgoraRtcCryptoLoader.framework`
    - `libcrypto.a`
  
2. Connect your iPhone or iPad device and run the project. Ensure a valid provisioning profile is applied or your project will not run.


## Resources

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
