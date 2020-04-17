package io.agora.openvcall.ui;

import android.Manifest;
import android.content.pm.PackageManager;
import android.content.res.TypedArray;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.view.*;
import android.view.inputmethod.InputMethodManager;
import android.widget.Toast;
import io.agora.openvcall.AGApplication;
import io.agora.openvcall.R;
import io.agora.openvcall.model.*;
import io.agora.propeller.Constant;
import io.agora.rtc.RtcEngine;
import io.agora.rtc.video.VideoCanvas;
import io.agora.rtc.video.VideoEncoderConfiguration;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

public abstract class BaseActivity extends AppCompatActivity {
    private final static Logger log = LoggerFactory.getLogger(BaseActivity.class);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final View layout = findViewById(Window.ID_ANDROID_CONTENT);
        ViewTreeObserver vto = layout.getViewTreeObserver();
        vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                    layout.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                } else {
                    layout.getViewTreeObserver().removeGlobalOnLayoutListener(this);
                }
                initUIandEvent();
            }
        });
    }

    protected abstract void initUIandEvent();

    protected abstract void deInitUIandEvent();

    protected void permissionGranted() {
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (isFinishing()) {
                    return;
                }

                boolean checkPermissionResult = checkSelfPermissions();

                if ((Build.VERSION.SDK_INT < Build.VERSION_CODES.M)) {
                    // so far we do not use OnRequestPermissionsResultCallback
                }
            }
        }, 500);
    }

    private boolean checkSelfPermissions() {
        return checkSelfPermission(Manifest.permission.RECORD_AUDIO, ConstantApp.PERMISSION_REQ_ID_RECORD_AUDIO) &&
                checkSelfPermission(Manifest.permission.CAMERA, ConstantApp.PERMISSION_REQ_ID_CAMERA) &&
                checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, ConstantApp.PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE);
    }

    @Override
    protected void onDestroy() {
        deInitUIandEvent();
        super.onDestroy();
    }

    public final void closeIME(View v) {
        InputMethodManager mgr = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(v.getWindowToken(), 0); // 0 force close IME
        v.clearFocus();
    }

    public boolean checkSelfPermission(String permission, int requestCode) {
        log.debug("checkSelfPermission " + permission + " " + requestCode);
        if (ContextCompat.checkSelfPermission(this,
                permission)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this,
                    new String[]{permission},
                    requestCode);
            return false;
        }

        if (Manifest.permission.CAMERA.equals(permission)) {
            permissionGranted();
        }
        return true;
    }

    protected AGApplication application() {
        return (AGApplication) getApplication();
    }

    protected RtcEngine rtcEngine() {
        return application().rtcEngine();
    }

    protected EngineConfig config() {
        return application().config();
    }

    protected void addEventHandler(AGEventHandler handler) {
        application().addEventHandler(handler);
    }

    protected void removeEventHandler(AGEventHandler handler) {
        application().remoteEventHandler(handler);
    }

    protected CurrentUserSettings vSettings() {
        return application().userSettings();
    }

    public final void showLongToast(final String msg) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_LONG).show();
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[], @NonNull int[] grantResults) {
        log.debug("onRequestPermissionsResult " + requestCode + " " + Arrays.toString(permissions) + " " + Arrays.toString(grantResults));
        switch (requestCode) {
            case ConstantApp.PERMISSION_REQ_ID_RECORD_AUDIO: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    checkSelfPermission(Manifest.permission.CAMERA, ConstantApp.PERMISSION_REQ_ID_CAMERA);
                } else {
                    finish();
                }
                break;
            }
            case ConstantApp.PERMISSION_REQ_ID_CAMERA: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, ConstantApp.PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE);
                    permissionGranted();
                } else {
                    finish();
                }
                break;
            }
            case ConstantApp.PERMISSION_REQ_ID_WRITE_EXTERNAL_STORAGE: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                } else {
                    finish();
                }
                break;
            }
        }
    }

    protected int virtualKeyHeight() {
        boolean hasPermanentMenuKey = ViewConfiguration.get(getApplication()).hasPermanentMenuKey();
        if (hasPermanentMenuKey) {
            return 0;
        }

        // Also can use getResources().getIdentifier("navigation_bar_height", "dimen", "android");
        DisplayMetrics metrics = new DisplayMetrics();
        Display display = getWindowManager().getDefaultDisplay();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            display.getRealMetrics(metrics);
        } else {
            display.getMetrics(metrics);
        }

        int fullHeight = metrics.heightPixels;
        int fullWidth = metrics.widthPixels;

        if (fullHeight < fullWidth) {
            fullHeight ^= fullWidth;
            fullWidth ^= fullHeight;
            fullHeight ^= fullWidth;
        }

        display.getMetrics(metrics);

        int newFullHeight = metrics.heightPixels;
        int newFullWidth = metrics.widthPixels;

        if (newFullHeight < newFullWidth) {
            newFullHeight ^= newFullWidth;
            newFullWidth ^= newFullHeight;
            newFullHeight ^= newFullWidth;
        }

        int virtualKeyHeight = fullHeight - newFullHeight;

        if (virtualKeyHeight > 0) {
            return virtualKeyHeight;
        }

        virtualKeyHeight = fullWidth - newFullWidth;

        return virtualKeyHeight;
    }

    protected final int getStatusBarHeight() {
        // status bar height
        int statusBarHeight = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            statusBarHeight = getResources().getDimensionPixelSize(resourceId);
        }

        if (statusBarHeight == 0) {
            log.error("Can not get height of status bar");
        }

        return statusBarHeight;
    }

    protected final int getActionBarHeight() {
        // action bar height
        int actionBarHeight = 0;
        final TypedArray styledAttributes = this.getTheme().obtainStyledAttributes(
                new int[]{android.R.attr.actionBarSize}
        );
        actionBarHeight = (int) styledAttributes.getDimension(0, 0);
        styledAttributes.recycle();

        if (actionBarHeight == 0) {
            log.error("Can not get height of action bar");
        }

        return actionBarHeight;
    }

    /**
     *
     * Starts/Stops the local video preview
     *
     * Before calling this method, you must:
     * Call the enableVideo method to enable the video.
     *
     * @param start Whether to start/stop the local preview
     * @param view The SurfaceView in which to render the preview
     * @param uid User ID.
     */
    protected void preview(boolean start, SurfaceView view, int uid) {
        if (start) {
            rtcEngine().setupLocalVideo(new VideoCanvas(view, VideoCanvas.RENDER_MODE_HIDDEN, uid));
            rtcEngine().startPreview();
        } else {
            rtcEngine().stopPreview();
        }
    }

    /**
     * Allows a user to join a channel.
     *
     * Users in the same channel can talk to each other, and multiple users in the same channel can start a group chat. Users with different App IDs cannot call each other.
     *
     * You must call the leaveChannel method to exit the current call before joining another channel.
     *
     * A successful joinChannel method call triggers the following callbacks:
     *
     * The local client: onJoinChannelSuccess.
     * The remote client: onUserJoined, if the user joining the channel is in the Communication profile, or is a BROADCASTER in the Live Broadcast profile.
     *
     * When the connection between the client and Agora's server is interrupted due to poor
     * network conditions, the SDK tries reconnecting to the server. When the local client
     * successfully rejoins the channel, the SDK triggers the onRejoinChannelSuccess callback
     * on the local client.
     *
     * @param channel The unique channel name for the AgoraRTC session in the string format.
     * @param uid User ID.
     */
    public final void joinChannel(final String channel, int uid) {
        String accessToken = getApplicationContext().getString(R.string.agora_access_token);
        if (TextUtils.equals(accessToken, "") || TextUtils.equals(accessToken, "<#YOUR ACCESS TOKEN#>")) {
            accessToken = null; // default, no token
        }

        rtcEngine().joinChannel(accessToken, channel, "OpenVCall", uid);
        config().mChannel = channel;
        enablePreProcessor();
        log.debug("joinChannel " + channel + " " + uid);
    }

    /**
     * Allows a user to leave a channel.
     *
     * After joining a channel, the user must call the leaveChannel method to end the call before
     * joining another channel. This method returns 0 if the user leaves the channel and releases
     * all resources related to the call. This method call is asynchronous, and the user has not
     * exited the channel when the method call returns. Once the user leaves the channel,
     * the SDK triggers the onLeaveChannel callback.
     *
     * A successful leaveChannel method call triggers the following callbacks:
     *
     * The local client: onLeaveChannel.
     * The remote client: onUserOffline, if the user leaving the channel is in the
     * Communication channel, or is a BROADCASTER in the Live Broadcast profile.
     *
     * @param channel Channel Name
     */
    public final void leaveChannel(String channel) {
        log.debug("leaveChannel " + channel);
        config().mChannel = null;
        disablePreProcessor();
        rtcEngine().leaveChannel();
        config().reset();
    }

    /**
     * Enables image enhancement and sets the options.
     */
    protected void enablePreProcessor() {
        if (Constant.BEAUTY_EFFECT_ENABLED) {
            rtcEngine().setBeautyEffectOptions(true, Constant.BEAUTY_OPTIONS);
        }
    }

    public final void setBeautyEffectParameters(float lightness, float smoothness, float redness) {
        Constant.BEAUTY_OPTIONS.lighteningLevel = lightness;
        Constant.BEAUTY_OPTIONS.smoothnessLevel = smoothness;
        Constant.BEAUTY_OPTIONS.rednessLevel = redness;
    }


    /**
     * Disables image enhancement.
     */
    protected void disablePreProcessor() {
        // do not support null when setBeautyEffectOptions to false
        rtcEngine().setBeautyEffectOptions(false, Constant.BEAUTY_OPTIONS);
    }

    protected void configEngine(VideoEncoderConfiguration.VideoDimensions videoDimension, VideoEncoderConfiguration.FRAME_RATE fps, String encryptionKey, String encryptionMode) {
        if (!TextUtils.isEmpty(encryptionKey)) {
            rtcEngine().setEncryptionMode(encryptionMode);
            rtcEngine().setEncryptionSecret(encryptionKey);
        }

        log.debug("configEngine " + videoDimension + " " + fps + " " + encryptionMode);
        // Set the Resolution, FPS. Bitrate and Orientation of the video
        rtcEngine().setVideoEncoderConfiguration(new VideoEncoderConfiguration(videoDimension,
                fps,
                VideoEncoderConfiguration.STANDARD_BITRATE,
                VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT));
    }
}
