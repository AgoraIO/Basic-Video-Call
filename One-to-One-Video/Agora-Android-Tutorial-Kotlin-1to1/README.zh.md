# Android Tutorial for Kotlin - 1to1

*[English](README.md) | 中文*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现 1 对 1 视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话；
- 静音和解除静音；
- 关闭摄像头和开启摄像头；
- 切换前置摄像头和后置摄像头；
- 选择分辨率、码率和帧率；

## 环境准备

- Android Studio 3.3+
- 真实 Android 设备 (Nexus 5X 或者其它设备)
- 部分模拟器会存在功能缺失或者性能问题，所以推荐使用真机

## 运行示例程序

这个段落主要讲解了如何编译和运行实例程序。

### 创建Agora账号并获取AppId

在编译和启动实例程序前，您需要首先获取一个可用的App ID:
1. 在[agora.io](https://dashboard.agora.io/signin/)创建一个开发者账号
2. 前往后台页面，点击左部导航栏的 **项目 > 项目列表** 菜单
3. 复制后台的 **App ID** 并备注，稍后启动应用时会用到它
4. 在项目页面生成临时 **Access Token** (24小时内有效)并备注，注意生成的Token只能适用于对应的频道名。

5. 将 AppID 填写进 "app/src/main/res/values/strings.xml"
  ```
  <string name="private_app_id"><#YOUR APP ID#></string>
  <!-- 临时Token 可以在 https://dashboard.agora.io 获取 -->
  <!-- 在正式上线生产环境前，你必须部署你自己的Token服务器 -->
  <!-- 如果你的项目没有打开安全证书，下面的值可以直接留空 -->
  <string name="agora_access_token"><#YOUR TOKEN#></string>
  ```

> 为提高项目的安全性，Agora 使用 Token（动态密钥）对即将加入频道的用户进行鉴权。
>
> 临时 Token 仅作为演示和测试用途。在生产环境中，你需要自行部署服务器签发 Token，详见[生成 Token](https://docs.agora.io/cn/Interactive Broadcast/token_server)。

### 集成 Agora 视频 SDK

集成方式有以下两种：
  - 通过JCenter集成：
    - 在项目对应的模块的 `app/build.gradle` 文件的依赖属性中加入通过 JCenter 自动集成 Agora 视频 SDK 的地址：
      ```
      implementation 'io.agora.rtc:full-sdk:3.0.0'
      ```
  - 手动集成：
    - 在 [Agora.io SDK](https://www.agora.io/cn/download/) 下载 **视频通话 + 直播 SDK**并解压，按以下对应关系将 **libs** 目录的内容复制到项目内。
      
      SDK目录|项目目录
      ---|---
      .jar file|**/apps/libs** folder
      **arm64-v8a** folder|**/app/src/main/jniLibs** folder
      **x86** folder|**/app/src/main/jniLibs** folder
      **armeabi-v7a** folder|**/app/src/main/jniLibs** folder
    - 若需要需要使用C++头文件，可以在解压SDK后将其中的 **libs**/**include** 文件夹下的 ***.h** 复制到本项目的 **app**/**src**/**main**/**cpp**/**agora** 下。


### 启动应用程序

最后用 Android Studio 打开该项目，连上设备，编译并运行。

也可以使用 `Gradle` 直接编译运行。

#### 方法1：使用 Android Studio 运行（推荐）

1. 在 Android Studio 中打开实例程序。
2. 选择 **File** > **Sync Project with Gradle Files**。
3. 编译并运行实例项目。此时应该能够在你的设备上显示该应用。

**注意：** 如果你的应用没有在 Android Studio 中进行编译，检查 **Build Error** 窗口来解决问题：
- 你可能需要安装额外的 Android SDK 支持文件。
- 你可能需要更新在应用根目录的 build.gradle 文件中的 Kotlin 版本号 `ext.kotlin_version` 。

#### 方法2：使用 IntelliJ IDEA

将实例应用作为 Gradle 项目导入到 **IntelliJ IDEA** 中。

1. 在主菜单中选择 **File** > **Open** 。
2. 在打开的对话中，选择包含实例应用的目录，并且点击 **OK** 。
3. 编译并运行示例项目。此时应该能够在你的设备上显示该应用。

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
