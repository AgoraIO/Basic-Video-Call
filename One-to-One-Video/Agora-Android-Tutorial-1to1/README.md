# Agora Android 1-to-1 Tutorial

*Read this in other languages: [中文](README.zh.md)*

This tutorial enables you to quickly get started in your development efforts to create an Android app with real-time video calls, voice calls, and interactive broadcasting. With this sample app you can:

* Join and leave a channel.
* Mute and unmute audio.
* Enable or disable video.
* Choose between the front or rear camera.

A more complete demonstration can be found [here](https://github.com/AgoraIO/Basic-Video-Call/tree/master/Group-Video/OpenVideoCall-Android).

## Prerequisites

* Android Studio 3.3 or above.
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
The SDK must be integrated into the sample project before it can opened and built. There are two methods for integrating the Agora Video SDK into the sample project. The first method uses JCenter to automatically integrate the SDK files. The second method requires you to manually copy the SDK files to the project.

#### Method 1 - Integrate the SDK Automatically Using JCenter (Recommended)

1. Clone this repository.
2. Open **app/build.gradle** and add the following line to the `dependencies` list:

```
...
dependencies {
    ...
    implementation 'io.agora.rtc:full-sdk:2.4.0'
}
```

#### Method 2 - Manually copy the SDK files

1. Clone this repository.
2. Download the Agora Video SDK from [Agora.io SDK](https://www.agora.io/en/download/).
3. Unzip the downloaded SDK package.
4. Copy the .jar file from the **libs** folder of the downloaded SDK package to the **/apps/libs** folder of the sample application.
5. Copy the .so files from the **armeabi-v7a** folder of the downloaded SDK package to the **/app/src/main/jniLibs/armeabi-v7a** folder of the sample application.
6. *(Optional)* Copy the .so files from the **arm64-v8a** folder of the downloaded SDK package to the **/app/src/main/jniLibs/arm64-v8a** folder of the sample application.
7. *(Optional)* Copy the .so files from the **x86** folder of the downloaded SDK package to the **/app/src/main/jniLibs/x86** folder of the sample application.


### Obtain and Build the Sample Application 

1. Open the sample application in Android Studio.
2. Build and run the sample project. This should display the application on your device.

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
