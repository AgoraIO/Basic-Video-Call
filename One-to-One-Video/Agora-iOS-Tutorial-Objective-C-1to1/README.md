# iOS Tutorial for Objective-C - 1to1

*English | [中文](README.zh.md)*

The Agora iOS Tutorial for Objective-C 1to1 Sample App is an open-source demo that will help you get video chat integrated directly into your iOS applications using the Agora Video SDK.

With this sample app, you can:

- Join / leave channel
- Mute / unmute audio
- Enable / disable video
- Switch camera

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
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.

5. Open `Agora iOS Tutorial Objective-C.xcodeproj` and edit the `AppID.m` file. Update `<#Your App Id#>` with your app ID, and assign the token variable with the temp Access Token generated from dashboard.

    ```
    NSString *const appID = @"<#Your App ID#>";
    // assign token to nil if you have not enabled app certificate
    NSString *const token = @"<#Temp Token#>";
    ```

### Integrate the Agora Video SDK

1. Download the [Agora Video SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the following files from the SDK `libs` folder into the sample application `Agora iOS Tutorial Objective-C` folder.
    - `AograRtcEngineKit.framework`
    - `AgoraRtcCryptoLoader.framework`
    - `libcrypto.a`
  
2. Connect your iPhone or iPad device and run the project. Ensure a valid provisioning profile is applied or your project will not run.

## Contact Us

- For potential issues, take a look at our [FAQ](https://docs.agora.io/en/faq) first
- Dive into [Agora SDK Samples](https://github.com/AgoraIO) to see more tutorials
- Take a look at [Agora Use Case](https://github.com/AgoraIO-usecase) for more complicated real use case
- Repositories managed by developer communities can be found at [Agora Community](https://github.com/AgoraIO-Community)
- You can find full API documentation at [Document Center](https://docs.agora.io/en/)
- If you encounter problems during integration, you can ask question in [Stack Overflow](https://stackoverflow.com/questions/tagged/agora.io)
- You can file bugs about this sample at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
