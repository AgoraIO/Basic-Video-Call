# Web Tutorial - 1to1

*English | [中文](README.zh.md)*

This tutorial shows you how to quickly create an basic video communication using the Agora sample app.

## Prerequisites

- A web browser

## Quick Start

This section shows you how to prepare, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.


### Integrate Agora Video SDK

Download SDK into **assets** path and rename file name as **AgoraRTCSDK-2.8.0.js**

### Run Web-Server

1. Install live-server globally via npm
    ```
    npm i live-server -g
    ```
2. Launch server via following commands in working folder,
    ```
    live-server .
    ```
3. Enter Valid APPID, Token, Channel, UID and click **Join Room**

## Resources

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
