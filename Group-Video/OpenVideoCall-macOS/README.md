# Open Video Call for MacOS (Swift)

*English | [中文](README.zh.md)*

This tutorial describes how to add video chat to your MacOS applications using Swift and the Agora Video SDK.

With this sample app, you can:

- Join/leave a channel
- Mute/unmute audio
- Enable/disable video
- Switch camera views
- Screen sharing
- Enable/disable image enhancement
- Play/stop music
- Set up the resolution, the frame rate display

## Prerequisites

- Xcode 10.0+

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App Id

To build and run the sample application, get an App Id:

1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App Id** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.

5. Open `OpenVideoCall.xcodeproj` and edit the `KeyCenter.swift` file. In the `agoraKit` declaration, update `<#Your App Id#>` with your App Id, and assign the token variable with the temp Access Token generated from dashboard.

    ``` Swift
    static let AppId: String = <#Your App Id#>
    // assign token to nil if you have not enabled app certificate
    static var Token: String? = <#Temp Access Token#>
    ```

### Integrate the Agora Video SDK

1. Download the [Agora Video SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the following files from the SDK `libs` folder into the sample application `OpenVideoCall` folder.

    - `AograRtcKit.framework`
    - `AgoraRtcCryptoLoader.framework`
    - `libcrypto.a`
  
2. Open Xcode and run this project.

## Contract Us

- For potential issues, you may take a look at our [FAQ](https://docs.agora.io/en/faq) first
- Dive into [Agora SDK Samples](https://github.com/AgoraIO) to see more tutorials
- Would like to see how Agora SDK is used in more complicated real use case? Take a look at [Agora Use Case](https://github.com/AgoraIO-usecase)
- Repositories managed by developer communities can be found at [Agora Community](https://github.com/AgoraIO-Community)
- You can find full API document at [Document Center](https://docs.agora.io/en/)
- If you encounter problems during integration, you can ask question in [Developer Forum](https://stackoverflow.com/questions/tagged/agora.io)
- You can file bugs about this sample at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
