# Open Video Call Windows

*English | [中文](README.zh.md)*

The Open Video Call Windows Sample App is an open-source demo that will help you get video chat integrated directly into your Windows applications using the Agora Video SDK.

With this sample app, you can:

- Join / leave channel
- Mute / unmute audio
- Enable / disable video
- Change camera
- Send message to channel
- Setup resolution, frame rate and bit rate
- Enable encryption
- Enable / disable black and white filter

This demo is written in **C++**

A tutorial demo can be found here: [Agora-Windows-Tutorial-1to1](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-Windows-Tutorial-1to1)

Agora Video SDK supports iOS / Android / Windows / macOS etc. You can find demos of these platform here:

- [OpenVideoCall-Android](https://github.com/AgoraIO/Basic-Video-Call/tree/master/Group-Video/OpenVideoCall-Android)
- [OpenVideoCall-iOS](https://github.com/AgoraIO/Basic-Video-Call/tree/master/Group-Video/OpenVideoCall-iOS)
- [OpenVideoCall-macOS](https://github.com/AgoraIO/Basic-Video-Call/tree/master/Group-Video/OpenVideoCall-macOS)

## Prerequisites

- Visual Studio 2013 or higher
- Windows 7 or higher

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.

4. Define the APP_ID with your App ID.

    ```
    #define APP_ID _T("Your App ID")
    ```

### Integrate the Agora Video SDK

1. Download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/blog/download/). Unzip the downloaded SDK package and copy the **sdk** to the `OpenVideoCall` folder in project（the old one may be over written.
2. Open OpenVideoCall.sln, build the solution and run.


### Note

1. After the program is compiled, if the program "xxx\xxx\xxx\Debug\Language\English.dll" cannot be started when running the program, 

please select the OpenVideoCall project in the Solution Explorer and right click. In the pop-up menu bar, select "Set as active project" to solve. Then run the program again.

2. The dll library under the sdk/dll file needs to be placed in the corresponding execution path.

Tips: The relevant dll library has been configured for you in this case tutorial. If you want to use the interface provided by agora for related development, you need to put the dll library into the corresponding execution path as prompted above.

## Resources

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
