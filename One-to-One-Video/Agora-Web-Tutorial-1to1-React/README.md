# Web Tutorial For React & TS - 1to1

*English | [中文](README.zh.md)*

This tutorial shows you how to quickly create an basic video communication using the Agora sample app.

## Prerequisites

- nodejs LTS
- A web browser

## Quick Start

This section shows you how to prepare, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.

2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.

3. Save the **App ID** from the Dashboard for later use.

4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.

   > To ensure communication security, Agora uses tokens (dynamic keys) to authenticate users joining a channel.
   >
   > Temporary tokens are for demonstration and testing purposes only and remain valid for 24 hours. In a production environment, you need to deploy your own server for generating tokens. See [Generate a Token](https://docs.agora.io/en/Interactive Broadcast/token_server)for details.


### Install dependencies and integrate the Agora Video SDK

1. Using the Terminal app, enter the `install` command in your project directory. This command installs libraries that are required to run the sample application.
    ``` bash
    # install dependencies
    npm install
    ```
2. Start the application by entering the `run dev` or `run build` command.
    The `run dev` command is for development purposes.
    ``` bash
    # serve with hot reload at localhost:8080
    npm run dev
    ```
    The `run build` command is for production purposes and minifies code.
    ``` bash
    # build for production with minification
    npm run build
    ```
3. Your default browser should open and display the sample application, as shown here.
    **Note:** In some cases, you may need to open a browser and enter `http://localhost:8080` as the URL.

## Resources

- For potential issues, take a look at our [FAQ](https://docs.agora.io/en/faq) first
- Dive into [Agora SDK Samples](https://github.com/AgoraIO) to see more tutorials
- Take a look at [Agora Use Case](https://github.com/AgoraIO-usecase) for more complicated real use case
- Repositories managed by developer communities can be found at [Agora Community](https://github.com/AgoraIO-Community)
- You can find full API documentation at [Document Center](https://docs.agora.io/en/)
- If you encounter problems during integration, you can ask question in [Stack Overflow](https://stackoverflow.com/questions/tagged/agora.io)
- You can file bugs about this sample at [issue](https://github.com/AgoraIO/Basic-Video-Call/issues)

## License

The MIT License (MIT)
