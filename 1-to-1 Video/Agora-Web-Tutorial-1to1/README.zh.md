# Agora Tutorial for Web on PC - 1to1

*Read this in other languages: [English](README.md)*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现1对1视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话；
- 推流和停止推流

## 运行示例程序
首先在 [Agora.io 注册](https://dashboard.agora.io/cn/signup/) 注册账号，并创建自己的测试项目，获取到 AppID。

在 [Agora.io SDK](https://www.agora.io/cn/download/) 下载 **语音通话 + 直播 Web SDK**，解压后将其中的 **AgoraRTCSDK-x.x.x.js** 文件复制到本项目根目录下。

将项目部署到本地或远端 HTTP 服务器后使用浏览器访问 index.html。

在成功看到示例界面后，使用 AppID 输入框输入第一步获得的 AppID。

点击 Join 即可进入频道，如若已经有朋友加入了该频道，就可以开始音视频通话。

## 注意
默认该项目只可以运行于 PC 浏览器，如果要在移动端浏览器上运行，需要在 `createClient ` 传入对应的参数，具体参看对应文档的该 API 描述。

## 联系我们

- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 如果在集成中遇到问题, 你可以到 [开发者社区](https://dev.agora.io/cn/) 提问
- 如果有售前咨询问题, 可以拨打 400 632 6626，或加入官方Q群 12742516 提问
- 如果需要售后技术支持, 你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的bug, 欢迎提交 [issue](https://github.com/AgoraIO/Agora-Web-Tutorial-1to1/issues)

## 代码许可

The MIT License (MIT).
