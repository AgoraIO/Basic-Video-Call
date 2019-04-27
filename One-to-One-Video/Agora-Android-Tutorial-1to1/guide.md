## Steps to Create the Sample 

* [Set Permission](#set-permissions)
* [Create Visual Assets](#create-visual-assets)
* [Design the User Interface](#design-the-user-interface)

For details about the APIs used to develop this sample, see the [Agora.io Documentation version 2.2](https://docs.agora.io/en/2.2).


## Set Permissions
In the `AndroidManifest.xml` file, `uses-permissions` settings were added for the Internet, audio recording, audio settings, network state, camera, and Bluetooth to allow the app to access these features:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="io.agora.tutorials1v1vcall">

    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.RECORD_AUDIO" />
    <uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.CAMERA" />
    <uses-permission android:name="android.permission.BLUETOOTH" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:supportsRtl="true"
        android:theme="@style/AppTheme">
        <activity
            android:name=".VideoChatViewActivity"
            android:screenOrientation="sensorPortrait"
            android:theme="@style/FullScreenVideoTheme">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

</manifest>
```

### Create Visual Assets
Add the following icon assets for the user interface to the */res/drawable* folder:

|Asset                   |Description                                                                                        |
|------------------------|---------------------------------------------------------------------------------------------------|
|`btn_end_call.png`|An image of a red telephone for a *hang up* button.|
|`btn_mute.png`|An image of a microphone to mute audio.|
|`btn_switch_camera.png`|An image of a camera and rotational arrows to switch between the two cameras.|
|`btn_video.png`|An image of a camera to start video.|
|`btn_voice.png`|An image of an arrow indicating that audio chat is enabled.|
|`ic_launcher.png`|A desktop icon for users to invoke the sample application.|


### Design the User Interface
The sample contains a single activity called *VideoChatViewActivity* and its layout is defined in */layout/activity_video_chat_view.xml*. 

The main aspects of this layout are shown here:
![ActivityViewChat.png](/images/ActivityViewChat.png)

|Component                        |Description                                                                                                                                 |
|---------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------|
|`activity_video_chat_view`       |A view that handles the main video feed. This view contains other views.|
|`remote_video_view_container`    |A view displaying the remote, incoming video feed (for example, the video the user will see).|
|`local_video_view_container`     |A smaller view at the top right corner showing the local video feed.|
|`quick_tips_when_use_agora_sdk`  |Displays quick tip information.|
|`LinearLayout (unamed)`          |A layout that encapsulates four buttons: **Pause Video**, **Audio Mute**, **Switch Camera**, and **Hang Up**. Each button uses the assets described above.|

## Configure Resources
To configure 1-to-1 communication resources:
 * [Create an Agora Instance](#create-an-agora-instance)
 * [Configure the Video Mode](#configure-video-mode)
 * [Set up Local Video](#set-up-local-video)
 * [Join a Channel](#join-a-channel)
 * [Set up Video Chat View Activity](#set-up-video-chat-view-activity)
 
### Create an Agora Instance
The code samples in this section are in *ViewChatViewActivity.java*.

The following imports define the interface of the Agora API that provides communication functionality: 
 - `io.agora.rtc.Constants`
 - `io.agora.rtc.IRtcEngineEventHandler`
 - `io.agora.rtc.RtcEngine`
 - `io.agora.rtc.video.VideoCanvas`

Create a singleton by invoking [RtcEngine.create()](https://docs.agora.io/en/2.2/product/Interactive%20Gaming/API%20Reference/game_android?platform=Android) during initialization, passing the application ID stored in *strings.xml* and a reference to the activity's event handler. The Agora API uses events to inform the application about Agora engine runtime events, such as joining or leaving a channel and adding new participants. 

```java
import io.agora.rtc.Constants;
import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.RtcEngine;
import io.agora.rtc.video.VideoCanvas;

...

private void initializeAgoraEngine() {
    try {
        mRtcEngine = RtcEngine.create(getBaseContext(), getString(R.string.agora_app_id), mRtcEventHandler);
    } catch (Exception e) {
        Log.e(LOG_TAG, Log.getStackTraceString(e));

        throw new RuntimeException("NEED TO check rtc sdk init fatal error\n" + Log.getStackTraceString(e));
    }
}
```

In the sample project, a helper method called `initializeAgoraEngine()` creates the singleton and is invoked by another helper method called `initAgoraEngineAndJoinChannel()`:

```java
private void initAgoraEngineAndJoinChannel() {
    initializeAgoraEngine();     
    ...
}
```

 `initAgoraEngineAndJoinChannel()` is invoked in the activity's `onCreate()` method after it checks for the necessary permissions:

```java
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_video_chat_view);

    if (checkSelfPermission(Manifest.permission.RECORD_AUDIO, PERMISSION_REQ_ID_RECORD_AUDIO) && checkSelfPermission(Manifest.permission.CAMERA, PERMISSION_REQ_ID_CAMERA)) {
        initAgoraEngineAndJoinChannel();
    }
}
```

### Configure Video Mode
The next step is to enable video mode, configure the video encoding profile, and specify if the width and height can change when switching from portrait to landscape:

```Java
private void setupVideoProfile() {
    mRtcEngine.enableVideo();
    mRtcEngine.setVideoProfile(Constants.VIDEO_PROFILE_360P, false);
}

private void initAgoraEngineAndJoinChannel() {
    initializeAgoraEngine();     
    setupVideoProfile();        
    ...
}
```

In the sample, a helper method called `setupVideoProfile()` contains this logic and is invoked by `initAgoraEngineAndJoinChannel()` during the activity's creation:

- It starts by enabling video with [enableVideo](https://docs.agora.io/en/2.2/product/Video/API%20Reference/communication_android_video?platform=Android). 
- The video encoding profile is then set to 360p and the `swapWidthAndHeight` parameter is set to `false` via [setVideoProfile](https://docs.agora.io/en/2.2/product/Video/API%20Reference/communication_android_video?platform=Android). Each profile includes a set of parameters such as resolution, frame rate, bitrate, etc. 

Note: If a device's camera does not support the specified resolution, the SDK automatically chooses a suitable camera resolution. However, the encoder resolution still uses the profile specified by `setVideoProfile()`. 

Since this configuration takes place before entering a channel, the end user will start in video mode rather than audio mode. If video mode is enabled during a call, the app will switch from audio to video mode. 

### Set up Local Video
The logic for the local video feed is contained within a helper method called `setupLocalVideo()` that is invoked by `initAgoraEngineAndJoinChannel()`:

```java
private void setupLocalVideo() {
    FrameLayout container = (FrameLayout) findViewById(R.id.local_video_view_container);
    SurfaceView surfaceView = RtcEngine.CreateRendererView(getBaseContext());
    surfaceView.setZOrderMediaOverlay(true);
    container.addView(surfaceView);
    mRtcEngine.setupLocalVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_ADAPTIVE, 0));
}

private void initAgoraEngineAndJoinChannel() {
    initializeAgoraEngine();     
    setupVideoProfile();         
    setupLocalVideo();           
    ...
}
```

`setupLocalVideo()` creates a `View` object for the video stream and initializes the following:
* The Z order media overlay is set to `true` to overlay the view on top of the parent view.
* The View is added to the `local_video_view_container` layout.

The call to [setupLocalVideo](https://docs.agora.io/en/2.2/product/Video/API%20Reference/communication_android_video?platform=Android) then passes a new `VideoCanvas` object to the engine that binds the video window (view) of local video streams and configures the video display settings. 

### Join a Channel
A helper method called `joinChannel()` invokes `RtcEngine.joinChannel()` enables a user to join a specific channel:

```java
private void joinChannel() {
    mRtcEngine.joinChannel(null, "demoChannel1", "Extra Optional Data", 0); 
}
```

The `channelName` parameter receives the name of the channel to join (`demoChannel1`). The call to [RtcEngine.joinChannel()](https://docs.agora.io/en/2.2/product/Video/API%20Reference/communication_android_video?platform=Android) enables the speakerphone when using Agora.

**Note**: Users in the same channel can talk to each other, but users with different App IDs cannot call each other even if they join the same channel.

In this code sample, the helper method `joinChannel()` is invoked by `initAgoraEngineAndJoinChannel()`:

```java
private void initAgoraEngineAndJoinChannel() {
    initializeAgoraEngine();     
    setupVideoProfile();         
    setupLocalVideo();           
    joinChannel();               
}
```

### Set up Video Chat View Activity
The `VideoChatViewActivity` class contains an `IRtcEngineEventHandler` to handle various video session events:

```java
private final IRtcEngineEventHandler mRtcEventHandler = new IRtcEngineEventHandler() { 
    @Override
    public void onFirstRemoteVideoDecoded(final int uid, int width, int height, int elapsed) { 5
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                setupRemoteVideo(uid);
            }
        });
    }

    @Override
    public void onUserOffline(int uid, int reason) { 
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                onRemoteUserLeft();
            }
        });
    }

    @Override
    public void onUserMuteVideo(final int uid, final boolean muted) { 
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                onRemoteUserVideoMuted(uid, muted);
            }
        });
    }
};
```
The `onFirstRemoteVideoDecoded()` method is invoked once another user is connected and the first remote video frame is received and decoded. This method invokes a helper method called `setupRemoteVideo()`:

```java
private void setupRemoteVideo(int uid) {
    FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);

    if (container.getChildCount() >= 1) {
        return;
    }

    SurfaceView surfaceView = RtcEngine.CreateRendererView(getBaseContext());
    container.addView(surfaceView);
    mRtcEngine.setupRemoteVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_ADAPTIVE, uid));

    surfaceView.setTag(uid); // for mark purpose
    View tipMsg = findViewById(R.id.quick_tips_when_use_agora_sdk); // optional UI
    tipMsg.setVisibility(View.GONE);
}
```


`setupRemoteVideo()` performs the following:
* Gets a reference to the remote video view in the layout.
* Creates and adds a `View` object to the layout.
* Creates a `VideoCanvas` and associates the view with it.
* Tags the View with the channel ID.
* Hides the quick tips.

The `onUserOffline()` method is invoked when another user leaves the channel. This method invokes a helper method called `onRemoteUserLeft()`:

```java
private void onRemoteUserLeft() {
    FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);
    container.removeAllViews();

    View tipMsg = findViewById(R.id.quick_tips_when_use_agora_sdk); // optional UI
    tipMsg.setVisibility(View.VISIBLE);
}
```

`onRemoteUserLeft` performs the following:
* Obtains a reference to the remote video view in the layout and removes it.
* Shows the quick tips.

The `onUserMuteVideo()` method s invoked when a remote user pauses their stream. This method invokes a helper method called: `onRemoteUserVideoMuted()`:

```java
private void onRemoteUserVideoMuted(int uid, boolean muted) {
    FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);

    SurfaceView surfaceView = (SurfaceView) container.getChildAt(0);

    Object tag = surfaceView.getTag();
    if (tag != null && (Integer) tag == uid) {
        surfaceView.setVisibility(muted ? View.GONE : View.VISIBLE);
    }
}
```

`onRemoteUserVideoMuted` performs the following:
* Gets a reference to the remote video view in the layout and the associated View.
* Checks that the channel ID associated with View's tag matches the channel ID passed in from Agora.
* Toggles the visibility of the remote video view.

## Manage Communication Features
Implement the following communication features:
* [Hang Up and End the Call](#hang-up-and-end-the-call)
* [Toggle Cameras](#toggle-cameras)
* [Mute Audio and Video](#mute-audio-and-video)

### Hang Up and End the Call
*Video Chat View Activity* contains a helper method called `leaveChannel()` with the logic to leave the current video call (channel). This is invoked in `onDestroy()` when the application is shut down:

```java
private void leaveChannel() {
    mRtcEngine.leaveChannel();
}

@Override
protected void onDestroy() {
    super.onDestroy();

    leaveChannel();
    ...
}
```
`leaveChannel()` invokes `RtcEngine.leaveChannel()` to leave the channel.

The class also contains a helper method called `onEncCallClicked()` which invokes `finish()` to cause the `onDestroy()` event to occur. The `btn_end_call` button that ends a call, has been configured in *activity_video_chat_view.xml* to invoke `onEncCallClicked()` in response to the `onClick()` event:

```xml
<ImageView
    android:layout_width="0dp"
    android:layout_height="match_parent"
    android:layout_weight="20"
    android:onClick="onEncCallClicked"
    android:scaleType="centerInside"
    android:src="@drawable/btn_end_call" />
```

### Toggle Cameras
To enable the user to choose between the front and rear cameras, the activity defines a method called `onSwitchCameraClicked()`:

```java
public void onSwitchCameraClicked(View view) {
        mRtcEngine.switchCamera();
    }
```

`onSwitchCameraClicked()` invokes `RtcEngine.switchCamera()` to toggle between the device's front and rear cameras. The `btn_switch_camera` button that switches the cameras, has been configured in *activity_video_chat_view.xml* to invoke `onSwitchCameraClicked()` in response to the `onClick()` event:

```xml
<ImageView
    android:layout_width="0dp"
    android:layout_height="match_parent"
    android:layout_weight="20"
    android:onClick="onSwitchCameraClicked"
    android:scaleType="centerInside"
    android:src="@drawable/btn_switch_camera" />
```

## Mute Audio and Video
To allow the user to mute audio, the activity defines a method called `onLocalAudioMuteClicked()`:

```java
public void onLocalAudioMuteClicked(View view) {
    ImageView iv = (ImageView) view;
    if (iv.isSelected()) {
        iv.setSelected(false);
        iv.clearColorFilter();
    } else {
        iv.setSelected(true);
        iv.setColorFilter(getResources().getColor(R.color.colorPrimary), PorterDuff.Mode.MULTIPLY);
    }

    mRtcEngine.muteLocalAudioStream(iv.isSelected());
}
```

The `btn_mute` button that mutes audio has been configured in *activity_video_chat_view.xml* to invoke `onLocalAudioMuteClicked()` in response to the `onClick()` event:

```xml
<ImageView
    android:layout_width="0dp"
    android:layout_height="match_parent"
    android:layout_weight="20"
    android:onClick="onLocalAudioMuteClicked"
    android:scaleType="centerInside"
    android:src="@drawable/btn_mute" />
```


`onLocalAudioMuteClicked()` performs the following:
* Gets a reference to the View containing the mute audio button.
* Determines if the state of the button is *selected* or not.
* Toggles the View's *selected* state.
* Clears the View's button color when deselecting it and sets the button color to red when selecting it.
* Invokes `RtcEngine.muteLocalAudioStream()` to toggle audio based on the *selected* state.

To allow the user to mute local video (for example, to prevent video of the current user from being broadcast to other users), the activity defines a method called `onLocalVideoMuteClicked()`:

```java
public void onLocalVideoMuteClicked(View view) {
    ImageView iv = (ImageView) view;
    if (iv.isSelected()) {
        iv.setSelected(false);
        iv.clearColorFilter();
    } else {
        iv.setSelected(true);
        iv.setColorFilter(getResources().getColor(R.color.colorPrimary), PorterDuff.Mode.MULTIPLY);
    }

    mRtcEngine.muteLocalVideoStream(iv.isSelected());

    FrameLayout container = (FrameLayout) findViewById(R.id.local_video_view_container);
    SurfaceView surfaceView = (SurfaceView) container.getChildAt(0);
    surfaceView.setZOrderMediaOverlay(!iv.isSelected());
    surfaceView.setVisibility(iv.isSelected() ? View.GONE : View.VISIBLE);
}
```

`onLocalVideoMuteClicked()` performs the following:
* Obtains a reference to the View containing the mute video button.
* Determines if the state of the button is *selected* or not.
* Toggles the View's *selected* state.
* Clears the View's button color when deselecting it and sets the button color to red when selecting it.
* Invokes `RtcEngine.muteLocalVideoStream()` to toggle video based on the *selected* state.
* Obtains a reference to the layout for the Local Video View.
* Toggles the Z order and visibility based on the *Selected* state.

The `btn_voice` button that mutes video has been configured in *activity_video_chat_view.xml* to invoke `onLocalVideoMuteClicked()` in response to the `onClick()` event:

```xml
<ImageView
    android:layout_width="0dp"
    android:layout_height="match_parent"
    android:layout_weight="20"
    android:onClick="onLocalVideoMuteClicked"
    android:scaleType="centerInside"
    android:src="@drawable/btn_voice" />
```