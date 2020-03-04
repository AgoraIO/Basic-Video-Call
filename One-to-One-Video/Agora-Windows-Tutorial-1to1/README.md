# 1-to-1 Tutorial for Windows

*English | [中文](README.zh.md)*

This tutorial enables you to quickly get started developing a Windows app with video chat. With this sample app you can:

* Join and leave a channel
* Mute and unmute audio
* Enable or disable video

## Prerequisites

- Visual Studio 2013 or higher
- Windows 7 or higher

## Running the sample program

First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID. define the APP_ID with your App ID.

    #define APP_ID _T("Your App ID")

If you don't want to modify the code part, you can create an AppId.ini file under Debug/Release. Modify the appId value to the App ID you just applied.

    [AppID]
    AppID=xxxxxxxxxxxxxxxxxxx

Next, download the **Agora Video SDK** from [Agora.io SDK](https://www.agora.io/en/download/). Unzip the downloaded SDK package and copy the **sdk** to the project folder(the old one may be over written).

Finally, Open AgoraTutorial.sln with your VC++ 2013(or higher) and build all solution and run.
    

**Note：**

  1. After the program is compiled, if the program "xxx\xxx\xxx\Debug\Language\English.dll" cannot be started when running the program, 
      please select the OpenLive project in the Solution Explorer and right click. In the pop-up menu bar, select "Set as startup project" to solve. Then run the program again.
  2. You may encounter crash when running this demo under debug mode. Please run this demo under release mode.



## Resources

- For potential issues, you may take a look at our [FAQ](https://docs.agora.io/en/faq) first
- Dive into [Agora SDK Samples](https://github.com/AgoraIO) to see more tutorials
- Would like to see how Agora SDK is used in more complicated real use case? Take a look at [Agora Use Case](https://github.com/AgoraIO-usecase)
- Repositories managed by developer communities can be found at [Agora Community](https://github.com/AgoraIO-Community)
- You can find full API document at [Document Center](https://docs.agora.io/en/)
- If you encounter problems during integration, you can ask question in [Developer Forum](https://stackoverflow.com/questions/tagged/agora.io)
- You can file bugs about this sample at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
