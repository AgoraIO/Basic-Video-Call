# Android 1-to-1 Tutorial for Kotlin

*English | [中文](README.zh.md)*

This tutorial enables you to quickly get started in your development efforts to create an Android app with real-time video chat using the Agora Video SDK. 

With this sample app you can:

- Join / leave channel
- Mute / unmute audio
- Enable / disable video
- Switch camera
- Setup resolution, frame rate and bit rate

**Note:** This sample is written in [Kotlin](https://kotlinlang.org), a language officially supported by Android.

## Prerequisites

- Android Studio 3.1+ or IntelliJ IDEA 2018.3+.
- Android device (e.g. Nexus 5X). A real device is recommended because some simulators have missing functionality or lack the performance necessary to run the sample.

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.

4. Locate the file **app/src/main/res/values/strings.xml** and replace <#YOUR APP ID#> with the App ID in the dashboard.

```xml
<string name="agora_app_id"><#YOUR APP ID#></string>
<!-- Obtain a temp Access Token at https://dashboard.agora.io -->
<!-- You will need to deploy your own token server for production release -->
<!-- Leave this value empty if Security keys/Token is not enabled for your project -->
<string name="agora_access_token"><#YOUR TOKEN#></string>
```

> To ensure communication security, Agora uses tokens (dynamic keys) to authenticate users joining a channel.
>
> Temporary tokens are for demonstration and testing purposes only and remain valid for 24 hours. In a production environment, you need to deploy your own server for generating tokens. See [Generate a Token](https://docs.agora.io/en/Interactive Broadcast/token_server)for details.

### Integrate the Agora Video SDK

The SDK must be integrated into the sample project before it can opened and built. There are two methods for integrating the Agora Video SDK into the sample project. The first method uses JCenter to automatically integrate the SDK files. The second method requires you to manually copy the SDK files to the project.

#### Method 1 - Integrate the SDK Automatically Using JCenter (Recommended)

1. Clone this repository.
2. Open **app/build.gradle** and add the following line to the `dependencies` list:

  ```
  ...
  dependencies {
      ...
      implementation 'io.agora.rtc:full-sdk:2.4.1'
  }
  ```

#### Method 2 - Manually copy the SDK files

1. Download the Agora Video SDK from [Agora.io SDK](https://www.agora.io/en/download/).
2. Unzip the downloaded SDK package.
3. Copy the following files from from the **libs** folder of the downloaded SDK package:

Copy from SDK|Copy to Project Folder
---|---
.jar file|**/apps/libs** folder
**arm64-v8a** folder|**/app/src/main/jniLibs** folder
**x86** folder|**/app/src/main/jniLibs** folder
**armeabi-v7a** folder|**/app/src/main/jniLibs** folder

​    

### Run the Application

Ensure USB debugging is enabled on your device under **Settings > Developer options** and connect the device to your computer.

#### Method 1: Using Android Studio (Recommended)

1. Open the sample application in Android Studio.
2. Select **File** > **Sync Project with Gradle Files**.
3. Build and run the sample project. This should display the application on your device.

**Note:** If your application does not compile in Android Studio,  check the **Build Error** window to troubleshoot: 
- You may need to install additional Android SDK support files.
- You may need to update the Kotlin version number `ext.kotlin_version` in the build.gradle file located in the root folder of the application.

#### Method 2: Using IntelliJ IDEA

Import the sample application into **IntelliJ IDEA** as a gradle project.

1. Select **File** > **Open** from the main menu.
2. In the dialog that opens, select the directory that contains the sample application and click **OK**.
3. Build and run the sample project. This should display the application on your device.


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
