# Web Tutorial For React & TS - 1to1

*[English](README.md) | 中文*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现多人视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话:
- 选择摄像头和麦克风设备:

## 环境准备

- nodejs LTS
- 浏览器

## 运行示例程序

This section shows you how to prepare, and run the sample application.

### 创建Agora账号并获取AppId

在编译和启动实例程序前，您需要首先获取一个可用的App ID:
1. 在[agora.io](https://dashboard.agora.io/signin/)创建一个开发者账号
2. 前往后台页面，点击左部导航栏的 **项目 > 项目列表** 菜单
3. 复制后台的 **App ID** 并备注，稍后启动应用时会用到它
4. 在项目页面生成临时 **Access Token** (24小时内有效)并备注，注意生成的Token只能适用于对应的频道名。


### 集成 Agora 视频 SDK

1. 在Terminal中，在您的项目根目录输入`install`命令以安装项目依赖
    ```shell
    # install dependencies
    npm install
    ```
2. 输入`run dev`命令以启动Web程序
    ```shell
    # serve with hot reload at localhost:8080
    npm run dev
    ```
    输入`npm run build`会压缩静态资源文件，可作为生产环境打包发布。
    ``` bash
    # build for production with minification
    npm run build
    ```
3. 你的浏览器默认会打开示例应用程序。
    **注意** 如果没有自动打开，请在浏览器里手动输入URL `http://localhost:8080`。

## 联系我们

- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 如果在集成中遇到问题, 你可以到 [开发者社区](https://dev.agora.io/cn/) 提问
- 如果有售前咨询问题, 可以拨打 400 632 6626，或加入官方Q群 12742516 提问
- 如果需要售后技术支持, 你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的 bug, 欢迎提交 [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## 代码许可

The MIT License (MIT)
