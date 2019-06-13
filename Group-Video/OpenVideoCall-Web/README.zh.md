# Open Video Call for Web

*[English](README.md) | 中文*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现多人视频通话。

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

5. 打开 **src/utils/Settings.js** 文件。在文件的底部，将`<#YOUR APP ID#>`替换为您自己的App ID，然后将`<#YOUR TEMP TOKEN#>`替换为dashboard计算的Token。

    **注意：** App ID是字符串类型，需要双引号或者单引号
    ```javascript
    export const APP_ID = <#YOUR APP ID#>;

    // 如果你没有打开Token功能，token可以直接给null
    export const Token = "<#YOUR TEMP TOKEN HERE#>";
    ```

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


## 联系我们

- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 如果在集成中遇到问题, 你可以到 [开发者社区](https://dev.agora.io/cn/) 提问
- 如果有售前咨询问题, 可以拨打 400 632 6626，或加入官方Q群 12742516 提问
- 如果需要售后技术支持, 你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的 bug, 欢迎提交 [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## 代码许可

The MIT License (MIT)
