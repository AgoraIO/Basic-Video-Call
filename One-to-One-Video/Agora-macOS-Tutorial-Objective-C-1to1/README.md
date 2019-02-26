# Agora macOS Tutorial for Objective-C - 1to1

*Read this in other languages: [Chinese](README.zh.md)*

The Agora macOS Tutorial for Objective-C 1to1 Sample App is an open-source demo that will help you get video chat integrated directly into your macOS applications using the Agora Video SDK.

With this sample app, you can:

- Join / leave channel
- Mute / unmute audio
- Mute / unmute video
- Device selection
- Screen share

## Running the App
First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID. Update "AppID.m" with your App ID.

```
NSString *const appID = @"Your App ID"; 
```

Next, download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the "libs" folder to the project folder.

Finally, Open Agora Mac Tutorial Objective-C.xcodeproj, setup your development signing and run.

## Developer Environment Requirements
* XCode 10.0 +

## Connect Us

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can file bugs about this demo at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT).
