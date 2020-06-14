``# Open Video Call Windows

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

* VS 2013(or higher)
* QT5.6(or higher)
* Windows 7(or higher)


## Running the App
First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID. define the APP_ID with your App ID.

* #define APP_ID _T("Your App ID")

Next, download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the **libs** to the project folder(the old one may be over written).

Finally, Open OpenVideoCall.pro with your Qt5.6(or higher) and build current project and run.

* If you don't fill appid in the code file,you can place a file named AgoraConfigOpenVideoCall.ini in the executable path.The file content:
    
    [BaseInfo]
    AppId=xxxx

**Note：**If you fill appid in the code file, appid in ini file will be omitted.

## Connect Us

- For potential issues, take a look at our [FAQ](https://docs.agora.io/cn/faq) first
- Dive into [Agora SDK Samples](https://github.com/AgoraIO) to see more tutorials
- Take a look at [Agora Use Case](https://github.com/AgoraIO-usecase) for more complicated real use case
- Repositories managed by developer communities can be found at [Agora Community](https://github.com/AgoraIO-Community)
- You can find full API documentation at [Document Center](https://docs.agora.io/en/)
- If you encounter problems during integration, you can ask question in [Stack Overflow](https://stackoverflow.com/questions/tagged/agora.io)
- You can file bugs about this sample at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
