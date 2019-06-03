# Android 1-to-1 Tutorial

*English | [中文](README.zh.md)*

This tutorial enables you to quickly get started in your development efforts to create an Android app with real-time video calls, voice calls, and interactive broadcasting.

With this sample app, you can:
- Join and leave a channel.
- Mute and unmute audio.
- Enable or disable video.
- Choose between the front or rear camera.

A more complete demo can be found [here](https://github.com/AgoraIO/Basic-Video-Call/tree/master/Group-Video/OpenVideoCall-Android).

## Prerequisites

- Android Studio 3.3 or above
- Android device (e.g. Nexus 5X). A real device is recommended because some simulators have missing functionality or lack the performance necessary to run the sample.

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Copy the **App ID** from the Dashboard.

4. Locate the file **app/src/main/res/values/strings.xml** and replace <#YOUR APP ID#> with the App ID in the dashboard.

```xml
<string name="agora_app_id"><#YOUR APP ID#></string>
```

### Integrate the Agora Video SDK

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

    

### Run the Application

Open project with Android Studio, connect your Android device, build and run.
      
Or use `Gradle` to build and run.


## Resources

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
