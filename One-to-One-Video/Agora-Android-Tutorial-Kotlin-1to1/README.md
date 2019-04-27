
# Agora Android 1-to-1 Tutorial for Kotlin

*Read this in other languages: [中文](README.zh.md)*

This tutorial enables you to quickly get started in your development efforts to create an Android app with real-time video chat using the Agora Video SDK. 

With this sample app you can:

* [Join](#join-a-channel) and [leave](#hang-up-and-end-the-call) a channel.
* [Mute and unmute audio](#mute-audio-and-video).
* [Enable or disable video](#mute-audio-and-video).
* [Choose between the front or rear camera](#toggle-cameras).

**Note:** This sample is written in [Kotlin](https://kotlinlang.org), a language officially supported by Android.

## Prerequisites

* Android Studio 3.1+ or IntelliJ IDEA 2018.3+.
* Android device (e.g. Nexus 5X). A real device is recommended because some simulators have missing functionality or lack the performance necessary to run the sample.

## Quick Start
This section shows you how to prepare, build, and run the sample application.

### Create an Account and Obtain an App ID
In order to build and run the sample application you must obtain an App ID: 

1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Locate the file **app/src/main/res/values/strings.xml** and replace <#YOUR APP ID#> with the App ID in the dashboard.

```xml
<string name="agora_app_id"><#YOUR APP ID#></string>
```

### Integrate the Agora Video SDK into the sample project

The SDK must be integrated into the sample project before it can opened and built.

#### Manually copy the SDK files

1. Download the Agora Video SDK from [Agora.io SDK](https://www.agora.io/en/download/).
2. Unzip the downloaded SDK package.
3. Copy the following files from from the **libs** folder of the downloaded SDK package:

Copy from SDK|Copy to Project Folder
---|---
.jar file|**/apps/libs** folder
**arm64-v8a** folder|**/app/src/main/jniLibs** folder
**x86** folder|**/app/src/main/jniLibs** folder
**armeabi-v7a** folder|**/app/src/main/jniLibs** folder


### Obtain and Build the Sample Application 

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

## Resources
* A detailed code walkthrough for this sample is available in [Steps to Create this Sample](./guide.md).
* You can find full API documentation at the [Document Center](https://docs.agora.io/en/).
* You can file bugs about this sample [here](https://github.com/AgoraIO/Basic-Video-Call/issues).


## Learn More
- 1 to 1 Video Tutorial for [iOS/Swift](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-iOS-Tutorial-Swift-1to1)
- 1 to 1 Video Tutorial for [iOS/Objective-C](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-macOS-Tutorial-Objective-C-1to1)
- 1 to 1 Video Tutorial for [macOS](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-macOS-Tutorial-Swift-1to1)
- 1 to 1 Video Tutorial for [Windows](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-Windows-Tutorial-1to1)
- 1 to 1 Video Tutorial for [Web](https://github.com/AgoraIO/Basic-Video-Call/tree/master/One-to-One-Video/Agora-Web-Tutorial-1to1)


## License
This software is under the MIT License (MIT). [View the license](LICENSE.md).