# Web Tutorial For React & Basic Video Call

*English | [中文](README.zh.md)*

This tutorial shows you how to quickly create an basic video communication using the Agora sample app.

## Prerequisites

- nodejs LTS
- A web browser

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [agora.io](https://dashboard.agora.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.


### Install dependencies and integrate the Agora Video SDK

1. Using the Terminal app, enter the `install` command in your project directory. This command installs libraries that are required to run the sample application.
    ``` 
    npm install
    ```
2. Start the application by entering the `start` command.
    ``` 
    npm start
    ```
    Run app <br/> in development mode
    Go to [http://localhost:3000](http://localhost:3000) and view it in a browser.

    If you edit, the page will reload. < br / >
    You will also see any lint errors in the console.

    The `run build` command is for production purposes and minifies code.
    ``` 
    npm run build
    ```
    
## License

The MIT License (MIT)