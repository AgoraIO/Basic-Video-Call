# Agora Open Video Call for Web

这个实例代码展示了如何使用[Agora2.0+](https://docs.agora.io/en/2.2/product/Voice/API%20Reference/communication_web_audio#voice-call-api)接口快速实现视频电话应用

## 环境准备

- Agora开发者账号
- Node.js 6.9.1+
- 支持SSL(https)的服务器

## 运行示例程序

这个段落主要讲解了如果编译和运行实例程序

### 创建Agora账号并获取AppId

在编译和启动实例程序前，您需要首先获取一个可用的App ID:
1. 在[agora.io](https://dashboard.agora.io/signin/)创建一个开发者账号
2. 前往后台页面，点击左部导航栏的 **项目 > 项目列表** 菜单
3. 复制后台的 **App ID** 并备注，稍后启动应用时会用到它
4. 打开 **src/utils/Settings.js** 文件。在文件的底部，将`<#YOUR APP ID#>`替换为您自己的App ID
**注意：** App ID是字符串类型，需要双引号或者单引号
```javascript
export const APP_ID = <#YOUR APP ID#>;
```

### 更新并启动实例程序

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

- 完整的 API 文档见 [文档中心](https://docs.agora.io/en/)
- 如果发现了示例代码的 bug, 欢迎提交 [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## 代码许可

The MIT License (MIT)
