# Web Tutorial - 1to1

*[English](README.md) | 中文*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现多人视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话:
- 选择摄像头和麦克风设备:

## 环境准备

- Node.js 6.9.1+
- 支持SSL(https)的服务器

## 运行示例程序

这个段落主要讲解了如何编译和运行示例程序。

### 创建Agora账号并获取AppId

在编译和启动实例程序前，您需要首先获取一个可用的App ID:
1. 在[agora.io](https://dashboard.agora.io/signin/)创建一个开发者账号
2. 前往后台页面，点击左部导航栏的 **项目 > 项目列表** 菜单
3. 复制后台的 **App ID** 并备注，稍后启动应用时会用到它
4. 在项目页面生成临时 **Access Token** (24小时内有效)并备注，注意生成的Token只能适用于对应的频道名。


### 集成 Agora 视频 SDK

下载SDK到 **assets**目录，并改名为**AgoraRTCSDK.js**

### 启动Web-Server

1. 在工作目录通过以下命令启动live-server服务器,
    ```
    npx serve .
    ```
2. 在对应的输入框里正确输入 APPID, Token, Channel。点击**JOIN**即可。

## 联系我们

- 如果你遇到了困难，可以先参阅 [常见问题](https://docs.agora.io/cn/faq)
- 如果你想了解更多官方示例，可以参考 [官方SDK示例](https://github.com/AgoraIO)
- 如果你想了解声网SDK在复杂场景下的应用，可以参考 [官方场景案例](https://github.com/AgoraIO-usecase)
- 如果你想了解声网的一些社区开发者维护的项目，可以查看 [社区](https://github.com/AgoraIO-Community)
- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 若遇到问题需要开发者帮助，你可以到 [开发者社区](https://rtcdeveloper.com/) 提问
- 如果需要售后技术支持, 你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的 bug，欢迎提交 [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## 代码许可

The MIT License (MIT)
