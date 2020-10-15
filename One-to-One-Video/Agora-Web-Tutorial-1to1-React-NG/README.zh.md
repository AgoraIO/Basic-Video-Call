# Web Tutorial For React & Basic Video Call

*[English](README.md) | 中文*

这个开源示例项目演示了如何快速集成 Agora 视频 SDK，实现简单的一对一视频通话。

在这个示例项目中包含了以下功能：

- 加入通话和离开通话:

## 环境准备

- nodejs LTS
- 浏览器

### 创建Agora账号并获取AppId

在编译和启动实例程序前，您需要首先获取一个可用的App ID:
1. 在[agora.io](https://dashboard.agora.io/signin/)创建一个开发者账号
2. 前往后台页面，点击左部导航栏的 **项目 > 项目列表** 菜单
3. 复制后台的 **App ID** 并备注，稍后启动应用时会用到它
4. 在项目页面生成临时 **Access Token** (24小时内有效)并备注，注意生成的Token只能适用于对应的频道名。


### 集成 Agora 视频 SDK

1. 在Terminal中，在您的项目根目录输入`install`命令以安装项目依赖
    ```
    npm install
    ```
2. 输入`start`命令以启动Web程序
    ```
    npm start
    ```
    在开发模式下运行 app <br/>
    打开[http://localhost:3000](http://localhost:3000)在浏览器中查看它。

    如果进行编辑，页面将重新加载。<br/>
    您还将在控制台中看到任何 lint 错误。

    输入`npm run build`会压缩静态资源文件，可作为生产环境打包发布。
    ``` 
    npm run build
    ```


## 代码许可

The MIT License (MIT)
