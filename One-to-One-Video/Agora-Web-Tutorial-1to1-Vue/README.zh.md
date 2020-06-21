*[English](README.md) | 中文*

本项目代码示例了如何快速集成 基础一对一/一对多 视频通话
功能包含：
1. 加入频道
2. 离开频道
其中，Appid, Channel 必填，Token 为可选
视频支持一对一，一对多 通话

如何获取appID ？
1. 在[agora.io](https://dashboard.agora.io/signin/)创建一个开发者账号
2. 前往后台页面，点击左部导航栏的 **项目 > 项目列表** 菜单
3. 复制后台的 **App ID** 并备注，稍后启动应用时会用到它
4. 在项目页面生成临时 **Access Token** (24小时内有效)并备注，注意生成的Token只能适用于对应的频道名。

开始运行项目
# basic-video-call

## Project setup
```
npm install
```

### Compiles and hot-reloads for development
```
npm run dev
```

### Compiles and minifies for production
```
npm run build
```

### Lints and fixes files
```
npm run lint
```

### Customize configuration
See [Configuration Reference](https://cli.vuejs.org/config/).
