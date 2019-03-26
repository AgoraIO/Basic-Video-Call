# Agora Windows Tutorial - 1to1

*Read this in other languages: [English](README.md)*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现 1 对 1 视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话；
- 静音和解除静音；
- 关闭摄像头和开启摄像头；

本开源项目使用 **C++** 语言

你也可以在这里查看进阶版的示例项目：[OpenVideoCall-Windows](https://github.com/AgoraIO/OpenVideoCall-Windows)

Agora 视频 SDK 支持 iOS / Android / Windows / macOS 等多个平台，你可以查看对应各平台的示例项目：

- [Agora-Android-Tutorial-1to1](https://github.com/AgoraIO/Agora-Android-Tutorial-1to1)
- [Agora-iOS-Tutorial-Swift-1to1](https://github.com/AgoraIO/Agora-iOS-Tutorial-Swift-1to1)
- [Agora-macOS-Tutorial-Swift-1to1](https://github.com/AgoraIO/Agora-macOS-Tutorial-Swift-1to1)

## 运行示例程序
首先在 [Agora.io 注册](https://dashboard.agora.io/cn/signup/) 注册账号，并创建自己的测试项目，获取到 AppID。将代码中 APP_ID 宏内容定义为 AppID

```
#define APP_ID _T("")
```

然后在 [Agora.io SDK](https://www.agora.io/cn/download/) 下载 **视频通话 + 直播 SDK**，解压后将其中的 **sdk** 文件夹复制到本项目目录下（并覆盖原有旧目录）。

最后使用 Visual Studio 2013 打开 AgoraTutorial.sln，编译整个解决方案即可运行。

## 注意

1. sdk/dll 文件下的 dll 库需要放在相应的执行路径下。

温馨提示：此案例教程中已经为您配置好相关的 dll 库，若用户想自己使用 agora 提供的接口进行相关开发，则需按上述提示将 dll 库放入相应的执行路径下。

## 运行环境
* Visual Studio 2013 或更高版本
* 两台 Windows PC

## 联系我们

- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 如果在集成中遇到问题, 你可以到 [开发者社区](https://dev.agora.io/cn/) 提问
- 如果有售前咨询问题, 可以拨打 400 632 6626，或加入官方 Q 群 12742516 提问
- 如果需要售后技术支持, 你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的 bug , 欢迎提交 [issue](https://github.com/AgoraIO/Agora-Windows-Tutorial-1to1/issues)

## 代码许可

The MIT License (MIT).
