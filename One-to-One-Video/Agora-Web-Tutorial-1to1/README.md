# Web Tutorial - 1to1

*English | [中文](README.zh.md)*

This tutorial shows you how to quickly create an basic video communication using the Agora sample app.

With this sample app, you can:

- Join/Leave a realtime video call
- Select Camera/Microphone to use

## Prerequisites

- Node.js 6.9.1+
- A web server that supports SSL (https)

## Quick Start

This section shows you how to prepare, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.


### Integrate Agora Video SDK

Download SDK into **assets** path and rename file name as **AgoraRTCSDK.js**

### Run Web-Server

1. Launch server via following commands in working folder,
    ```
    npx serve .
    ```
2. Enter Valid APPID, Token, Channel and click **JOIN**

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
