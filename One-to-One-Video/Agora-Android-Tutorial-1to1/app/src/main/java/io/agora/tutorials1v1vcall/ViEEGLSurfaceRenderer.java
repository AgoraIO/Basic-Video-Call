/*
 * Copyright (c) 2019  www.agora.io
 *
 * All Rights Reserved.
 */

package io.agora.tutorials1v1vcall;


import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.nio.ByteBuffer;

import io.agora.rtc.gl.EglBase;
import io.agora.rtc.gl.RendererCommon;
import io.agora.rtc.internal.Logging;
import io.agora.rtc.utils.ThreadUtils;

public class ViEEGLSurfaceRenderer
        extends SurfaceView implements SurfaceHolder.Callback {
    private final static String TAG = ViEEGLSurfaceRenderer.class.getSimpleName();
    private ViEEGLRenderer mRender;

    private EglBase.Context mEglContext;
    private int[] mConfigAttributes;
    private RendererCommon.GlDrawer mDrawer;
    private long nativeObject = 0;
    private final Object nativeFunctionLock = new Object();


    public static boolean UseEglSurface(Object renderWindow) {
        return ViEEGLSurfaceRenderer.class.isInstance(renderWindow);
    }

    // Return true if this device support Open GL ES 2.0 rendering.
    public static boolean IsSupported(Context context) {
        ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return info.reqGlEsVersion >= 0x20000;
    }

    /**
     * Standard View constructor. In order to render something, you must first call init().
     */
    public ViEEGLSurfaceRenderer(Context context) {
        super(context);
        mRender = new ViEEGLRenderer(TAG);
        //getHolder().addCallback(this);

        mRender.setRenderView(this, this);
    }

    /**
     * Standard View constructor. In order to render something, you must first call init().
     */
    public ViEEGLSurfaceRenderer(Context context, AttributeSet attrs) {
        super(context, attrs);
        mRender = new ViEEGLRenderer(TAG);
        //getHolder().addCallback(this);

        mRender.setRenderView(this, this);
    }

    public void init(EglBase.Context sharedContext) {
        //mRender.init(sharedContext);
        mEglContext = sharedContext;
        onInitialize(null);
        onStart();
    }

    public void init(EglBase.Context sharedContext, SurfaceTextureHelper helper) {
        //mRender.init(sharedContext);
        mEglContext = sharedContext;
        onInitialize(helper);
        onStart();
    }

    public void init(final EglBase.Context sharedContext, final int[] configAttributes, RendererCommon.GlDrawer drawer) {
        //mRender.init(sharedContext, configAttributes, drawer);

        mEglContext = sharedContext;
        mConfigAttributes = configAttributes;
        mDrawer = drawer;

        onInitialize(null);
        onStart();
    }

    public void deinit() {
        onStop();
        onDispose();
    }

    public void RegisterNativeObject(long nativeObject) {
        // TODO(Haonong Yu): 2019-07-17 thread safety
        synchronized (nativeFunctionLock) {
            this.nativeObject = nativeObject;
        }
    }

    public void DeRegisterNativeObject() {
        // TODO(Haonong Yu): 2019-07-17 thread safety
        synchronized (nativeFunctionLock) {
            this.nativeObject = 0;
        }
    }

    public void setMirror(final boolean mirror) {
        mRender.getEglRender().setMirror(mirror);
    }

    private boolean onInitialize(SurfaceTextureHelper helper) {
        if (mConfigAttributes != null && mDrawer != null) {
            mRender.init(mEglContext, mConfigAttributes, mDrawer, helper);
        } else {
            mRender.init(mEglContext, helper);
        }
        return true;
    }

    private boolean onStart() {
        return mRender.start();
    }

    private void onStop() {
        mRender.stop();
    }

    private void onDispose() {
        mRender.release();
    }

    public void rendByteBufferFrame(ByteBuffer buffer, int format, int width, int height, int rotation, long ts) {
        mRender.consume(buffer, format, width, height, rotation, ts);
    }

    public void rendByteArrayFrame(byte[] data, int pixelFormat, int width, int height, int rotation, long ts) {
        mRender.consume(data, pixelFormat, width, height, rotation, ts);
    }

    public void renderTextureFrame(int texId, int pixelFormat, int width, int height, int rotation, long ts, float[] matrix) {
        //Log.i(TAG, "renderTextureFrame:" + texId + ";" + pixelFormat + ";" + width + ";" + mHeight + ";" + rotation + ";" + matrix);
        mRender.consume(texId, pixelFormat, width, height, rotation, ts, matrix, new Runnable() {
            @Override
            public void run() {
                Logging.d(TAG, "DONE, return texture back");
                synchronized (nativeFunctionLock) {
                    if (nativeObject != 0) {
                        returnTexture(nativeObject);
                    }
                }
            }
        });
    }

    // SurfaceHolder.Callback interface.
    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed");
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.i(TAG, "surfaceChanged: format: " + format + " size: " + width + "x" + height);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        ThreadUtils.checkIsOnMainThread();
        mRender.getEglRender().setLayoutAspectRatio((right - left) / (float) (bottom - top));
    }

    private native void returnTexture(long nativeObject);
}