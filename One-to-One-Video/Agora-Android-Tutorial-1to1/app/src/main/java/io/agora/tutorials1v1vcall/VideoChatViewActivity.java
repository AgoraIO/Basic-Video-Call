package io.agora.tutorials1v1vcall;

import android.Manifest;
import android.annotation.TargetApi;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Matrix;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.opengl.GLES20;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.util.concurrent.CountDownLatch;

import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.RtcEngine;
import io.agora.rtc.gl.GlTextureFrameBuffer;
import io.agora.rtc.gl.RendererCommon;
import io.agora.rtc.gl.TextureBufferImpl;
import io.agora.rtc.gl.VideoFrame;
import io.agora.rtc.gl.YuvConverter;
import io.agora.rtc.mediaio.SurfaceTextureHelper;
import io.agora.rtc.utils.YuvUtils;
import io.agora.rtc.video.AgoraVideoFrame;
import io.agora.rtc.video.VideoCanvas;
import io.agora.rtc.video.VideoEncoderConfiguration;

public class VideoChatViewActivity extends AppCompatActivity {

    private static final String LOG_TAG = VideoChatViewActivity.class.getSimpleName();

    private static final int PERMISSION_REQ_ID = 22;

    // permission WRITE_EXTERNAL_STORAGE is not mandatory for Agora RTC SDK, just incase if you wanna save logs to external sdcard
    private static final String[] REQUESTED_PERMISSIONS = {Manifest.permission.RECORD_AUDIO, Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE};

    private RtcEngine mRtcEngine;
    private final IRtcEngineEventHandler mRtcEventHandler = new IRtcEngineEventHandler() {
        @Override
        public void onFirstRemoteVideoDecoded(final int uid, int width, int height, int elapsed) {
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

    // Experimental: for texture copy and conversion
    final int mWidth = 640;
    final int mHeight = 480;
    private CameraTextureSource mCameraTextureSource;
    private YuvConverter mYuvConverter;
    // private int mFbo = 0;
    // private int m2DTextureID = 0;
    private GlTextureFrameBuffer mTextureFrameBuffer;
    private TextureRenderer mCameraToFbo;
    private static final boolean DEBUG_TEXTURE = false;
    private ViEEGLSurfaceRenderer mTextureLocalView;
    private ImageView mYuvImageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_chat_view);

        if (checkSelfPermission(REQUESTED_PERMISSIONS[0], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[1], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[2], PERMISSION_REQ_ID)) {
            initAgoraEngineAndJoinChannel();
        }
    }

    private void initAgoraEngineAndJoinChannel() {
        initializeAgoraEngine();
        setupVideoProfile();
        setupLocalVideo();
        if (DEBUG_TEXTURE) {
            setupLocalYUVPlayback();
        }
        setupVideoSource();
        joinChannel();
    }

    private boolean checkSelfPermission(String permission, int requestCode) {
        Log.i(LOG_TAG, "checkSelfPermission " + permission + " " + requestCode);
        if (ContextCompat.checkSelfPermission(this,
                permission)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this,
                    REQUESTED_PERMISSIONS,
                    requestCode);
            return false;
        }
        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[], @NonNull int[] grantResults) {
        Log.i(LOG_TAG, "onRequestPermissionsResult " + grantResults[0] + " " + requestCode);

        switch (requestCode) {
            case PERMISSION_REQ_ID: {
                if (grantResults[0] != PackageManager.PERMISSION_GRANTED || grantResults[1] != PackageManager.PERMISSION_GRANTED || grantResults[2] != PackageManager.PERMISSION_GRANTED) {
                    showLongToast("Need permissions " + Manifest.permission.RECORD_AUDIO + "/" + Manifest.permission.CAMERA + "/" + Manifest.permission.WRITE_EXTERNAL_STORAGE);
                    finish();
                    break;
                }

                initAgoraEngineAndJoinChannel();
                break;
            }
        }
    }

    private final void showLongToast(final String msg) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_LONG).show();
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mCameraTextureSource.stop();
        mCameraTextureSource.release();

        leaveChannel();
        // TODO: 2019-07-27: call below on the corresponding egl thread
        // if (mTextureFrameBuffer != null) {
        //    mTextureFrameBuffer.release();
        // }
        RtcEngine.destroy();
        mRtcEngine = null;
    }

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

    public void onSwitchCameraClicked(View view) {
        mRtcEngine.switchCamera();
    }

    public void onEncCallClicked(View view) {
        finish();
    }

    private void initializeAgoraEngine() {
        try {
            mRtcEngine = RtcEngine.create(getBaseContext(), getString(R.string.agora_app_id), mRtcEventHandler);
        } catch (Exception e) {
            Log.e(LOG_TAG, Log.getStackTraceString(e));

            throw new RuntimeException("NEED TO check rtc sdk init fatal error\n" + Log.getStackTraceString(e));
        }
    }

    private void setupVideoProfile() {
        mRtcEngine.enableVideo();

//      mRtcEngine.setVideoProfile(Constants.VIDEO_PROFILE_360P, false); // Earlier than 2.3.0
        mRtcEngine.setVideoEncoderConfiguration(new VideoEncoderConfiguration(VideoEncoderConfiguration.VD_640x360, VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_15,
                VideoEncoderConfiguration.STANDARD_BITRATE,
                VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT));
    }

    private void setupLocalVideo() {
        FrameLayout container = (FrameLayout) findViewById(R.id.local_video_view_container);
        if (DEBUG_TEXTURE) {
            mTextureLocalView = new ViEEGLSurfaceRenderer(this);
            mTextureLocalView.setZOrderMediaOverlay(true);
            container.addView(mTextureLocalView);
            return;
        }
        SurfaceView surfaceView = RtcEngine.CreateRendererView(getBaseContext());
        surfaceView.setZOrderMediaOverlay(true);
        container.addView(surfaceView);
        mRtcEngine.setupLocalVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_FIT, 0));
    }

    /**
     * Setup camera video source
     */
    private void setupVideoSource() {
        mRtcEngine.setExternalVideoSource(true, false, true);
        mCameraTextureSource = new CameraTextureSource(this, mWidth, mHeight, new SurfaceTextureHelper.OnTextureFrameAvailableListener() {
            @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR1)
            @Override
            public void onTextureFrameAvailable(int oesTextureId, float[] transformMatrix, long timestampNs) {
                int rotation = mCameraTextureSource.getFrameOrientation();
                copyTextureOesTo2D(oesTextureId, mWidth, mHeight);

                final AgoraVideoFrame videoFrame = textureToNV21Frame(mWidth, mHeight,
                        mTextureFrameBuffer.getTextureId(), VideoFrame.TextureBuffer.Type.RGB,
                        transformMatrix, timestampNs, rotation);

                if (DEBUG_TEXTURE) {
                    // local yuv playback
                    final CountDownLatch latch = new CountDownLatch(1);
                    runOnUiThread(new Runnable() {
                      @Override
                      public void run() {
                        YuvImage yuvImage = new YuvImage(videoFrame.buf, ImageFormat.NV21,
                                videoFrame.stride, videoFrame.height, null);
                        ByteArrayOutputStream baos = new ByteArrayOutputStream();
                        yuvImage.compressToJpeg(new Rect(0, 0, videoFrame.stride, videoFrame.height), 50, baos);
                        byte[] imageBytes = baos.toByteArray();
                        Bitmap bitmap = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.length);
                        mYuvImageView.setImageBitmap(bitmap);
                        latch.countDown();
                      }
                    });
                    try {
                        latch.await();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    mTextureLocalView.renderTextureFrame(mTextureFrameBuffer.getTextureId(),
                            AgoraVideoFrame.FORMAT_TEXTURE_2D, mWidth,
                            mHeight, rotation, timestampNs, transformMatrix);
                } else {
                    mRtcEngine.pushExternalVideoFrame(videoFrame);
                }
            }
        });
        mCameraTextureSource.initialize();
        mCameraTextureSource.start();
        if (DEBUG_TEXTURE) {
            mTextureLocalView.init(mCameraTextureSource.getEglContext());
        }
    }

    /**
     * This can only be called on a thread that has an active EGL context.
     */
    private AgoraVideoFrame textureToNV21Frame(int width, int height, int textureId,
                                               VideoFrame.TextureBuffer.Type type,
                                               float[] transformMatrix, long timestampNs,
                                               int rotation) {
      // TODO: 1. find the issue, and reuse mYuvConverter; 2. release the final mYuvConverter
        if (mYuvConverter != null) {
            mYuvConverter.release();
        }
        mYuvConverter = new YuvConverter();

        Matrix matrix = RendererCommon.convertMatrixToAndroidGraphicsMatrix(transformMatrix);
        VideoFrame.TextureBuffer textureBuffer = new TextureBufferImpl(width, height, type,
                textureId, matrix, null, null);

        VideoFrame.I420Buffer i420Buffer = mYuvConverter.convert(textureBuffer);

        byte[] buf = YuvUtils.yuv420toNV21(i420Buffer, width, height);
        AgoraVideoFrame frame = new AgoraVideoFrame();
        frame.format = AgoraVideoFrame.FORMAT_NV21;
        frame.stride = width;
        frame.height = height;
        frame.buf = buf;
        frame.rotation = rotation;
        frame.timeStamp = timestampNs / 1000 / 1000;
        return frame;
    }

    /**
     * This can only be called on a thread that has an active EGL context.
     */
    private void copyTextureOesTo2D(int oesTextureId, int width, int height) {
        if (mTextureFrameBuffer == null) {
//            int[] fbo = new int[1];
//            GLES20.glGenFramebuffers(1, fbo, 0);
//            mFbo = fbo[0];
//
//            int[] texture = new int[1];
//            GLES20.glGenTextures(1, texture, 0);
//            m2DTextureID = texture[0];
//
//            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
//            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, m2DTextureID);
//            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, width, mHeight, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
//            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
//            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
//            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
//            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
//            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
//
//            // Bind the framebuffer
//            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFbo);
//            // Specify texture as color attachment
//            GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, m2DTextureID, 0);
//
//            // Check for framebuffer complete
//            int status = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
//            if (status != GLES20.GL_FRAMEBUFFER_COMPLETE) {
//                Log.e(LOG_TAG, "Failed to create framebuffer!!!");
//            }
            mTextureFrameBuffer = new GlTextureFrameBuffer(GLES20.GL_RGBA);
        }

        if (mCameraToFbo == null) {
            mCameraToFbo = new TextureRenderer(true);
        }

        mTextureFrameBuffer.setSize(width, height);

        // 将背景清空成指定颜色
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        // 每次Draw之前都要绑定一次
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mTextureFrameBuffer.getFrameBufferId());

        // Render to FBO (TODO: resize/crop to target size)
        GLES20.glFinish();
        // 这里的宽高指的是要绘制到的View的宽高，比如SurfaceView
        GLES20.glViewport(0, 0, width, height);

        mCameraToFbo.draw(oesTextureId);

        // 解绑
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
    }

    private void joinChannel() {
        String token = getString(R.string.agora_access_token);
        if(token.isEmpty()) {
            token = null;
        }
        mRtcEngine.joinChannel(token, "demoChannel1", "Extra Optional Data", 0); // if you do not specify the uid, we will generate the uid for you
    }

    private void setupLocalYUVPlayback() {
        FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);
        mYuvImageView = new ImageView(this);
        container.addView(mYuvImageView, new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
    }

    private void setupRemoteVideo(int uid) {
        FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);

        if (container.getChildCount() >= 1) {
            return;
        }

        SurfaceView surfaceView = RtcEngine.CreateRendererView(getBaseContext());
        container.addView(surfaceView);
        mRtcEngine.setupRemoteVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_FIT, uid));

        surfaceView.setTag(uid); // for mark purpose
        View tipMsg = findViewById(R.id.quick_tips_when_use_agora_sdk); // optional UI
        tipMsg.setVisibility(View.GONE);
    }

    private void leaveChannel() {
        mRtcEngine.leaveChannel();
    }

    private void onRemoteUserLeft() {
        FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);
        container.removeAllViews();

        View tipMsg = findViewById(R.id.quick_tips_when_use_agora_sdk); // optional UI
        tipMsg.setVisibility(View.VISIBLE);
    }

    private void onRemoteUserVideoMuted(int uid, boolean muted) {
        FrameLayout container = (FrameLayout) findViewById(R.id.remote_video_view_container);

        SurfaceView surfaceView = (SurfaceView) container.getChildAt(0);

        Object tag = surfaceView.getTag();
        if (tag != null && (Integer) tag == uid) {
            surfaceView.setVisibility(muted ? View.GONE : View.VISIBLE);
        }
    }
}
