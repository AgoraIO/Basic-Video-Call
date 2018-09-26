# Open Video Call Linux

*Read this in other languages: [English](README.en.md)*

这个开源示例项目演示了如何快速集成Agora视频SDK，实现多人视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话；
- 静音和解除静音；
- 关闭摄像头和打开摄像头；
- 切换摄像头；
- 选择分辨率、码率和帧率；

Agora视频SDK支持 iOS / Android / Windows / macOS/ Linux 等多个平台，你可以查看对应各平台的示例项目：

- [OpenVideoCall-Android](https://github.com/AgoraIO/OpenVideoCall-Android)
- [OpenVideoCall-Windows](https://github.com/AgoraIO/OpenVideoCall-Windows)
- [OpenVideoCall-macOS](https://github.com/AgoraIO/OpenVideoCall-macOS)
- [OpenVideoCall-iOS](https://github.com/AgoraIO/OpenVideoCall-iOS)

## 运行示例程序
首先在 [Agora.io 注册](https://dashboard.agora.io/cn/signup/) 注册账号，并创建自己的测试项目，获取到 AppID。将 AppID 填写进 run.sh

```
--appId "your app ID"
```

然后在 [Agora.io SDK](https://www.agora.io/cn/blog/download/) 下载 **视频通话 + 直播 SDK**，解压后将其中**libs**文件夹下的 

- libagora_rtc_sdk.so

两个文件复制到本项目的 “OpenVideoCall/libs” 文件夹下。
在sample/OpenVideoCall下 输入命令 make， 便可编译生成openVideoCall demo。在run.sh中配置执行demo所需要的参数。
openVideoCall 为命令行程序。使用方法如下：
command:
- open
  开启通话。
- close
  关闭通话。
- enable_video 
  打开/关闭 全局视频功能，enable_video 0 为关闭视频功能，enable_video 1为打开视频功能
- enable_audio
  打开/关闭 全局音频功能
- enable_local_video
  打开/关闭 本地视频功能，enable_local_video 0 为关闭视频功能，enable_local_video 1为打开视频功能
- mute_local_video
  打开/关闭 本地视频流发送，mute_local_video 0 为打开本地视频流发送，mute_local_video 1为关闭本地视频流发送
- mute_local_audio
  打开/关闭 本地音频流发送，mute_local_video 0 为打开本地音频流发送，mute_local_video 1为关闭本地音频流发送
- print_device_info
  打印设备信息
- set_cur_camera
  设置当前工作的摄像头，set_cur_camera ‘device id ’ ，device id可以通过print_device_info 得到
- exit
  退出程序
  
## 系统运行环境
* OS:
  - Ubuntu 12.04 x64 或更高版本
  - CentOS 7 以上版本

* CPU:
  - x86_64
* Compiler:
  - gcc 4.8 and above

## 音视频依赖的预安装库
*




## 联系我们

- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 如果在集成中遇到问题，你可以到 [开发者社区](https://dev.agora.io/cn/) 提问
- 如果有售前咨询问题，可以拨打 400 632 6626，或加入官方Q群 12742516 提问
- 如果需要售后技术支持，你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的bug，欢迎提交 [issue](https://github.com/AgoraIO/OpenVideoCall-iOS/issues)

## 代码许可

The MIT License (MIT).
