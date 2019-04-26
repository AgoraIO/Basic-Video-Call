# Agora 1-to-1 Tutorial for Windows

*Read this in other languages: [中文](README.zh.md)*

This tutorial enables you to quickly get started developing a Windows app with video chat. With this sample app you can:

* Join and leave a channel
* Mute and unmute audio
* Enable or disable video

## Prerequisites

* Visual Studio 2013 or higher
* Windows 7 or higher

## Quick Start
This section shows you how to prepare, build, and run the sample application.

### Create an Account and Obtain an App ID
To build and run the sample application you must obtain an App ID: 

1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/).
2. Upon logging in, use the dashboard tree on the left to navigate to  **Projects** > **Project List**.
3. Locate the file **AgoraTutorial/AgoraObject.h** and replace `YOUR-APP-ID` with the App ID in the dashboard.

```
#define APP_ID _T("YOUR-APP-ID") // put your app id here
```

### Integrate the Agora Video SDK into the sample project

1. Clone this repository.
2. Download the Agora Video SDK from [Agora.io SDK](https://www.agora.io/en/download/).
3. Unzip the downloaded SDK package.
4. Copy the `sdk` folder from the downloaded SDK package into the `AgoraTutorial` folder of the sample application.

### Obtain and Build the Sample Application 

1. Open the `AgoraTutorial.sln` file in Visual Studio.
2. Build and run the sample project.

## Resources

* A detailed code walkthrough for this sample is available in [Steps to Create this Sample](./guide.md).
* You can find full API documentation at the [Document Center](https://docs.agora.io/en/).
* You can file bugs about this sample [here](https://github.com/AgoraIO/Basic-Video-Call/issues).

## Learn More

- 1 to 1 Video Tutorial for [Android](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-Android-Tutorial-1to1)
- 1 to 1 Video Tutorial for [iOS/Swift](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-iOS-Tutorial-Swift-1to1)
- 1 to 1 Video Tutorial for [MacOS](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-macOS-Tutorial-Swift-1to1)

## License

This software is under the MIT License (MIT). [View the license](LICENSE.md).
