# 1-to-1 Tutorial for Windows

*English | [中文](README.zh.md)*

This tutorial enables you to quickly get started developing a Windows app with video chat. With this sample app you can:

* Join and leave a channel
* Mute and unmute audio
* Enable or disable video

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

1. Download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/blog/download/). Unzip the downloaded SDK package and copy the **sdk** to the `AgoraTutorial` folder in project（the old one may be over written.
2. Open AgoraTutorial.sln, build the solution and run.


## Resources

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
