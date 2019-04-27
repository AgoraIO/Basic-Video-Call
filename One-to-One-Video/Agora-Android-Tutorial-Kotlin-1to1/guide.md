## Steps to Create the Sample 

* [Set Permissions](#set-permissions)
* [Create Visual Assets](#create-visual-assets)
* [Design the User Interface](#design-the-user-interface)
* [Configure Resources](#configure-resources)

### Set Permissions

In the `AndroidManifest.xml` file, `uses-permissions` settings were added for the Internet, audio recording, audio settings, network state, and camera to allow the app to access these features:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="io.agora.tutorials1v1vcall">

    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.RECORD_AUDIO" />
    <uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.CAMERA" />

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
Add the following icon assets for the user interface to the */res/drawable-xxxhdpi* folder:

Asset|Image|Description                                       
---|---|---
`btn_end_call.png`|![app/src/main/res/drawable-xxxhdpi/](app/src/main/res/drawable-xxxhdpi/btn_end_call.png)|An image of a red telephone for a *hang up* button.
`btn_mute.png`|![app/src/main/res/drawable-xxxhdpi/btn_mute.png](app/src/main/res/drawable-xxxhdpi/btn_mute.png)|An image of a microphone to mute audio.
`btn_switch_camera.png`|![app/src/main/res/drawable-xxxhdpi/btn_switch_camera.png](app/src/main/res/drawable-xxxhdpi/btn_switch_camera.png)|An image of a camera and rotational arrows to switch between the two cameras.
`btn_video.png`|![app/src/main/res/drawable-xxxhdpi/btn_video.png](app/src/main/res/drawable-xxxhdpi/btn_video.png)|An image of a camera to start video.
`btn_voice.png`|![app/src/main/res/drawable-xxxhdpi/btn_voice.png](app/src/main/res/drawable-xxxhdpi/btn_voice.png)|An image of an arrow indicating that audio chat is enabled.
`ic_launcher.png`|![app/src/main/res/drawable-xxxhdpi/ic_launcher.png](app/src/main/res/drawable-xxxhdpi/ic_launcher.png)|A desktop icon for users to invoke the sample application.


### Design the User Interface

The sample contains a single activity called *VideoChatViewActivity* and its layout is defined in */layout/activity_video_chat_view.xml*.

The main aspects of this layout are shown here:
![ActivityVideoChatView.jpg](images/ActivityVideoChatView.jpg)

Component|Description
---|---
`activity_video_chat_view`|A view that handles the main video feed. This view contains other views.
`remote_video_view_container`|A view displaying the remote, incoming video feed (for example, the video the user will see).
`local_video_view_container`|A smaller view at the top right corner showing the local video feed.
`quick_tips_when_use_agora_sdk`|Displays quick tip information.
`LinearLayout (unamed)`|A layout that encapsulates four buttons: **Pause Video**, **Audio Mute**, **Switch Camera**, and **Hang Up**. Each button uses the assets described above.

### Configure Resources

To configure 1-to-1 communication resources:

* [Add Import Statements and Initialize Global Variables](#add-import-statements-and-initialize-global-variables)
* [Check Permissions](#check-permissions)
* [Create an Agora Instance](#create-an-agora-instance)
* [Configure the Video Mode](#configure-the-video-mode)
* [Set up Local Video](#set-up-local-video)
* [Join a Channel](#join-a-channel)
* [Set up Video Chat View Activity](#set-up-video-chat-view-activity)
* [Manage Communication Features](#manage-communication-features)
 
#### Add Import Statements and Initialize Global Variables

The code samples in this section are in [*VideoChatViewActivity.kt*](src/main/java/io/agora/tutorials1v1vcall/VideoChatViewActivity.kt).

The following imports define the interface of the Agora API that provides communication functionality: 

 - `io.agora.rtc.IRtcEngineEventHandler`
 - `io.agora.rtc.RtcEngine`
 - `io.agora.rtc.video.VideoCanvas`
 - `io.agora.rtc.video.VideoEncoderConfiguration`

```
package io.agora.tutorials1v1vcall

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.PorterDuff
import android.os.Bundle
import android.support.v4.app.ActivityCompat
import android.support.v4.content.ContextCompat
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.SurfaceView
import android.view.View
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast

import io.agora.rtc.IRtcEngineEventHandler
import io.agora.rtc.RtcEngine
import io.agora.rtc.video.VideoCanvas
import io.agora.rtc.video.VideoEncoderConfiguration
```

The `VideoChatViewActivity` class extends `AppCompatActivity` and initializes two global variables `mRtcEngine` and `mRtcEventHandler`.

When `mRtcEventHandler` is initialized, three event listeners are added to invoke their respective methods:

Event Listener|Method|Description
---|---|---
`onFirstRemoteVideoDecoded`|`setupRemoteVideo()`|Triggers when the first remote video is decoded.
`onUserOffline`|`onRemoteUserLeft()`|Triggers when a user goes offline.
`onUserMuteVideo`|`onRemoteUserVideoMuted()`|Triggers when a user stops their video.

```
class VideoChatViewActivity : AppCompatActivity() {

    private var mRtcEngine: RtcEngine? = null
    private val mRtcEventHandler = object : IRtcEngineEventHandler() {
        override fun onFirstRemoteVideoDecoded(uid: Int, width: Int, height: Int, elapsed: Int) {
            runOnUiThread { setupRemoteVideo(uid) }
        }

        override fun onUserOffline(uid: Int, reason: Int) {
            runOnUiThread { onRemoteUserLeft() }
        }

        override fun onUserMuteVideo(uid: Int, muted: Boolean) {
            runOnUiThread { onRemoteUserVideoMuted(uid, muted) }
        }
    }
    
    ...
    
}    
```

The `mRtcEventHandler` variable is an object that contains a log tag, audio recording permissions value, and a camera permissions value.

```
    companion object {

        private val LOG_TAG = VideoChatViewActivity::class.java.simpleName

        private val PERMISSION_REQ_ID_RECORD_AUDIO = 22
        private val PERMISSION_REQ_ID_CAMERA = PERMISSION_REQ_ID_RECORD_AUDIO + 1
    }    
```

#### Check Permissions

The `checkSelfPermission()` method is invoked in the activity's `onCreate()` method prior to starting the Agora engine.

The permissions are verified using `ContextCompat.checkSelfPermission()` and `ActivityCompat.requestPermissions()`.

```
    fun checkSelfPermission(permission: String, requestCode: Int): Boolean {
        Log.i(LOG_TAG, "checkSelfPermission $permission $requestCode")
        if (ContextCompat.checkSelfPermission(this,
                permission) != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this,
                    arrayOf(permission),
                    requestCode)
            return false
        }
        return true
    }
```

The `onRequestPermissionsResult` method triggers when the user grants device permissions, and processes the permission results.

The permissions for audio recording `PERMISSION_REQ_ID_RECORD_AUDIO` is verifed and checks for camera permissions if audio recording permissions are granted.

The camera `PERMISSION_REQ_ID_CAMERA` is verified and initializes the Agora engine using `initAgoraEngineAndJoinChannel()`.

If any permissions fail, a log is created using `showLongToast()`.

```
    override fun onRequestPermissionsResult(requestCode: Int,
                                            permissions: Array<String>, grantResults: IntArray) {
        Log.i(LOG_TAG, "onRequestPermissionsResult " + grantResults[0] + " " + requestCode)

        when (requestCode) {
            PERMISSION_REQ_ID_RECORD_AUDIO -> {
                if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    checkSelfPermission(Manifest.permission.CAMERA, PERMISSION_REQ_ID_CAMERA)
                } else {
                    showLongToast("No permission for " + Manifest.permission.RECORD_AUDIO)
                    finish()
                }
            }
            PERMISSION_REQ_ID_CAMERA -> {
                if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    initAgoraEngineAndJoinChannel()
                } else {
                    showLongToast("No permission for " + Manifest.permission.CAMERA)
                    finish()
                }
            }
        }
    }
```

The `showLongToast()` method is used to log debugging text and messages for the application using `Toast.makeText()`.

```
    fun showLongToast(msg: String) {
        this.runOnUiThread { Toast.makeText(applicationContext, msg, Toast.LENGTH_LONG).show() }
    }
```

#### Create an Agora Instance

The `onCreate()` method initializes the activity view using `setContentView()` and initializes the Agora engine.

Check for audio recording and camera permissions using `checkSelfPermission()` prior to initializing the Agora engine using `initAgoraEngineAndJoinChannel()`.

```java
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video_chat_view)

        if (checkSelfPermission(Manifest.permission.RECORD_AUDIO, PERMISSION_REQ_ID_RECORD_AUDIO) && checkSelfPermission(Manifest.permission.CAMERA, PERMISSION_REQ_ID_CAMERA)) {
            initAgoraEngineAndJoinChannel()
        }
    }
```

The `initAgoraEngineAndJoinChannel()` method is invoked in the activity's `onCreate()` method after it checks for the necessary permissions.

1. Initialize the Agora engine using `initializeAgoraEngine()`.
2. Set up the video profile and local video using `setupVideoProfile()` and `setupLocalVideo()`.
3. Join the channel using `joinChannel()`.

```    
    private fun initAgoraEngineAndJoinChannel() {
        initializeAgoraEngine()
        setupVideoProfile()
        setupLocalVideo()
        joinChannel()
    }
```

The `initializeAgoraEngine()` method creates a singleton by invoking `RtcEngine.create()`, passing the App ID stored in *strings.xml* and a reference to the activity's event handler `mRtcEventHandler`. The Agora API uses events to inform the application about Agora engine runtime events, such as joining or leaving a channel and adding new participants. 

```java
    private fun initializeAgoraEngine() {
        try {
            mRtcEngine = RtcEngine.create(baseContext, getString(R.string.agora_app_id), mRtcEventHandler)
        } catch (e: Exception) {
            Log.e(LOG_TAG, Log.getStackTraceString(e))

            throw RuntimeException("NEED TO check rtc sdk init fatal error\n" + Log.getStackTraceString(e))
        }
    }
```


#### Configure the Video Mode

The next step is to enable video mode, configure the video encoding profile, and specify if the width and height can change when switching from portrait to landscape.

The helper method `setupVideoProfile()`, contains this logic and is invoked by `initAgoraEngineAndJoinChannel()` during the activity's creation.


Start by enabling video with `mRtcEngine!!.enableVideo()`. 

Set the video encoder configuration using `mRtcEngine!!.setVideoEncoderConfiguration()`:

Configuration Value|Description
---|---
`VideoEncoderConfiguration.VD_640x360`|640 x 360 size.
`VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_15`|15 fps frame rate.
`VideoEncoderConfiguration.STANDARD_BITRATE`|Standard bitrate.
`VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT`|Portait orientation mode.

```
    private fun setupVideoProfile() {
        mRtcEngine!!.enableVideo()
//      mRtcEngine!!.setVideoProfile(Constants.VIDEO_PROFILE_360P, false) // Earlier than 2.3.0
        mRtcEngine!!.setVideoEncoderConfiguration(VideoEncoderConfiguration(VideoEncoderConfiguration.VD_640x360,
                VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_15,
                VideoEncoderConfiguration.STANDARD_BITRATE,
                VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT))
    }
```

#### Set up Local Video

The logic for the local video feed is contained within a helper method called `setupLocalVideo()` that is invoked by `initAgoraEngineAndJoinChannel()`.

The `setupLocalVideo()` method creates a `View` object for the video stream and adds it to the main layout.

1. Retrieve the `local_video_view_container` layout.
2. Create a view for the video using `RtcEngine.CreateRendererView()`.
3. Set the Z order media overlay to `true`, to overlay the view on top of the parent view.
4. Add the view to the `local_video_view_container` layout using `container.addView()`.
5. Notify the Agora engine of the video canvas created for local video using `mRtcEngine!!.setupLocalVideo()`, binding the video window (view) of local video streams and configures the video display settings.

```
    private fun setupLocalVideo() {
        val container = findViewById(R.id.local_video_view_container) as FrameLayout
        val surfaceView = RtcEngine.CreateRendererView(baseContext)
        surfaceView.setZOrderMediaOverlay(true)
        container.addView(surfaceView)
        mRtcEngine!!.setupLocalVideo(VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_FIT, 0))
    }
```

#### Join a Channel

A helper method `joinChannel()` invokes `mRtcEngine!!.joinChannel()` and enables a user to join a the specified channel and is invoked by `initAgoraEngineAndJoinChannel()`.

The `channelName` parameter uses `demoChannel1` as the name of the channel.

**Note**: Users in the same channel can talk to each other, but users with different App IDs cannot call each other even if they join the same channel.

```
    private fun joinChannel() {
        mRtcEngine!!.joinChannel(null, "demoChannel1", "Extra Optional Data", 0) // if you do not specify the uid, we will generate the uid for you
    }
```

#### Set up Video Chat View Activity

The `onFirstRemoteVideoDecoded()` event listener invokes a helper method called `setupRemoteVideo()`.

The `setupRemoteVideo()` method sets up the remote video for the specified user `uid`:

1. Retrieve the remote video view layout `R.id.remote_video_view_container` and ensure it contains at least one child view.
2. Create a `View` object using `RtcEngine.CreateRendererView()` and add it to the layout using `container.addView()`.
3. Create a `VideoCanvas` and associate the view with it for the Agora engine using `mRtcEngine!!.setupRemoteVideo()`.
4. Tag the view with the user ID `uid`.
5. Retrieve the quick tips `R.id.quick_tips_when_use_agora_sdk` and hide it by setting `tipMsg.visibility` to `View.GONE`.

```java
    private fun setupRemoteVideo(uid: Int) {
        val container = findViewById(R.id.remote_video_view_container) as FrameLayout

        if (container.childCount >= 1) {
            return
        }

        val surfaceView = RtcEngine.CreateRendererView(baseContext)
        container.addView(surfaceView)
        mRtcEngine!!.setupRemoteVideo(VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_FIT, uid))

        surfaceView.tag = uid // for mark purpose
        val tipMsg = findViewById<TextView>(R.id.quick_tips_when_use_agora_sdk) // optional UI
        tipMsg.visibility = View.GONE
    }
```

The `onUserOffline()` method invokes a helper method called `onRemoteUserLeft()`.


The `onRemoteUserLeft()` method updates the layout and removes all video views:

1. Retrieve to the remote video view in the layout `R.id.remote_video_view_container` and removes all associated views using `container.removeAllViews()`.
2. Retrieve the quick tips `R.id.quick_tips_when_use_agora_sdk` and show it by setting `tipMsg.visibility` to `View.VISIBLE`.

```
    private fun onRemoteUserLeft() {
        val container = findViewById(R.id.remote_video_view_container) as FrameLayout
        container.removeAllViews()

        val tipMsg = findViewById<TextView>(R.id.quick_tips_when_use_agora_sdk) // optional UI
        tipMsg.visibility = View.VISIBLE
    }
```

The `onUserMuteVideo()` method invokes a helper method called: `onRemoteUserVideoMuted()`.

The `onRemoteUserVideoMuted` method shows/hides the video of the specified remote user `uid`.

1. Retreive the remote video view in the layout `R.id.remote_video_view_container` and the associated `SurfaceView`.
2. Ensure the `surfaceView.tag` is valid and matches the specified `uid` before toggling the visibility of the remote video view using `surfaceView.visibility`.

```
    private fun onRemoteUserVideoMuted(uid: Int, muted: Boolean) {
        val container = findViewById(R.id.remote_video_view_container) as FrameLayout

        val surfaceView = container.getChildAt(0) as SurfaceView

        val tag = surfaceView.tag
        if (tag != null && tag as Int == uid) {
            surfaceView.visibility = if (muted) View.GONE else View.VISIBLE
        }
    }
```

#### Manage Communication Features

Implement the following communication features:

* [Hang Up and End the Call](#hang-up-and-end-the-call)
* [Toggle Cameras](#toggle-cameras)
* [Mute Audio and Video](#mute-audio-and-video)

##### Hang Up and End the Call

*Video Chat View Activity* contains an `onDestroy()` method to close the activity view and application.

1. Leave the channel using the helper method `leaveChannel()`.
2. Destroy the Agora engine using `RtcEngine.destroy()` and setting `mRtcEngine` to `null`.

```
    override fun onDestroy() {
        super.onDestroy()

        leaveChannel()
        RtcEngine.destroy()
        mRtcEngine = null
    }
```

The helper method `leaveChannel()` contains the logic to leave the current video call / channel. This is invoked in `onDestroy()` when the application is shut down.

The `leaveChannel()` method invokes `mRtcEngine!!.leaveChannel()` to leave the channel.

```
    private fun leaveChannel() {
        mRtcEngine!!.leaveChannel()
    }
```

The class also contains a helper method called `onEncCallClicked()` which invokes `finish()` to cause the `onDestroy()` event to occur.

```
    fun onEncCallClicked(view: View) {
        finish()
    }
```

The `btn_end_call` button configured in *activity_video_chat_view.xml* invokes `onEncCallClicked()`, in response to the `onClick()` event.
 
```xml
            <ImageView
                android:layout_width="0dp"
                android:layout_height="match_parent"
                android:layout_weight="20"
                android:onClick="onEncCallClicked"
                android:scaleType="centerInside"
                android:src="@drawable/btn_end_call" />
```

##### Toggle Cameras

To enable the user to choose between the front and rear cameras, the activity defines a method called `onSwitchCameraClicked()`.

The `onSwitchCameraClicked()` method invokes `mRtcEngine!!.switchCamera()` to toggle between the device's front and rear cameras.

```
    fun onSwitchCameraClicked(view: View) {
        mRtcEngine!!.switchCamera()
    }
```

The `btn_switch_camera` button configured in *activity_video_chat_view.xml* invokes `onSwitchCameraClicked()` in response to the `onClick()` event:

```xml
            <ImageView
                android:layout_width="0dp"
                android:layout_height="match_parent"
                android:layout_weight="20"
                android:onClick="onSwitchCameraClicked"
                android:scaleType="centerInside"
                android:src="@drawable/btn_switch_camera" />
```

##### Mute Audio and Video

To allow the user to mute audio, the activity defines a method called `onLocalAudioMuteClicked()`.

1. Retrieve the `view` containing the mute audio button.
2. Determine if the state of the button is *selected* or not using `iv.isSelected`.
3. Toggle the *selected* state using `iv.isSelected`.
4. Clears the button color when deselecting it using `iv.clearColorFilter()` and set the button color when selecting it using `iv.setColorFilter`.
* Invokes `mRtcEngine!!.muteLocalAudioStream()` to toggle audio based on the *selected* state.

```
    fun onLocalAudioMuteClicked(view: View) {
        val iv = view as ImageView
        if (iv.isSelected) {
            iv.isSelected = false
            iv.clearColorFilter()
        } else {
            iv.isSelected = true
            iv.setColorFilter(resources.getColor(R.color.colorPrimary), PorterDuff.Mode.MULTIPLY)
        }

        mRtcEngine!!.muteLocalAudioStream(iv.isSelected)
    }
```

The `btn_mute` button configured in *activity_video_chat_view.xml* invokes `onLocalAudioMuteClicked()` in response to the `onClick()` event:

```xml
            <ImageView
                android:layout_width="0dp"
                android:layout_height="match_parent"
                android:layout_weight="20"
                android:onClick="onLocalAudioMuteClicked"
                android:scaleType="centerInside"
                android:src="@drawable/btn_mute" />
```

To allow the user to mute local video (for example, to prevent video of the current user from being broadcast to other users), the activity defines a method called `onLocalVideoMuteClicked()`.

1. Retrieve the `view` containing the mute video button.
2. Determine if the state of the button is *selected* or not using `iv.isSelected`.
3. Toggle the *selected* state using `iv.isSelected`.
4. Clear the button color when deselecting it using `iv.clearColorFilter()` and set the button color when selecting it using `iv.setColorFilter()`.
5. Invoke `mRtcEngine!!.muteLocalVideoStream()` to toggle video based on the *selected* state.
6. Retrieve the layout for the local video view `R.id.local_video_view_container` and associated `SurfaceView` object.
7. Toggle the Z order using `surfaceView.setZOrderMediaOverlay()` and visibility using `surfaceView.visibility` based on the *selected* state.

```
    fun onLocalVideoMuteClicked(view: View) {
        val iv = view as ImageView
        if (iv.isSelected) {
            iv.isSelected = false
            iv.clearColorFilter()
        } else {
            iv.isSelected = true
            iv.setColorFilter(resources.getColor(R.color.colorPrimary), PorterDuff.Mode.MULTIPLY)
        }

        mRtcEngine!!.muteLocalVideoStream(iv.isSelected)

        val container = findViewById(R.id.local_video_view_container) as FrameLayout
        val surfaceView = container.getChildAt(0) as SurfaceView
        surfaceView.setZOrderMediaOverlay(!iv.isSelected)
        surfaceView.visibility = if (iv.isSelected) View.GONE else View.VISIBLE
    }
```

The `btn_voice` button configured in *activity_video_chat_view.xml* invokes `onLocalVideoMuteClicked()` in response to the `onClick()` event:

```xml
            <ImageView
                android:layout_width="0dp"
                android:layout_height="match_parent"
                android:layout_weight="20"
                android:onClick="onLocalVideoMuteClicked"
                android:scaleType="centerInside"
                android:src="@drawable/btn_voice" />

```