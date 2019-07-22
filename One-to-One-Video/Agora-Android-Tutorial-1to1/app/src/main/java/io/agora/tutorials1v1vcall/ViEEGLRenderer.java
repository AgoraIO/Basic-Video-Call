/*
 * Copyright (c) 2019  www.agora.io
 *
 * All Rights Reserved.
 */


package io.agora.tutorials1v1vcall;

import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;

import java.nio.ByteBuffer;
import java.util.concurrent.CountDownLatch;

import io.agora.rtc.gl.EglBase;
import io.agora.rtc.gl.GlRectDrawer;
import io.agora.rtc.gl.JavaI420Buffer;
import io.agora.rtc.gl.RendererCommon;
import io.agora.rtc.gl.RgbaBuffer;
import io.agora.rtc.gl.TextureBufferImpl;
import io.agora.rtc.gl.VideoFrame;
import io.agora.rtc.utils.ThreadUtils;
import io.agora.rtc.video.AgoraVideoFrame;

public class ViEEGLRenderer implements
        SurfaceHolder.Callback,
        TextureView.SurfaceTextureListener {
    private final static String TAG = ViEEGLRenderer.class.getSimpleName();

    private static final String ERROR_EGL = "Only one egl surface allowed";
    private final EglRenderer eglRenderer;

    private SurfaceView mSurfaceView;
    private TextureView mTextureView;
    private Surface mSurface;
    private SurfaceTexture mSurfaceTexture;

    private SurfaceHolder.Callback mSurfaceViewListener;
    private TextureView.SurfaceTextureListener mSurfaceTextureListener;

    private boolean mHasEglSurface = false;
    private boolean mStarted = false;

    public ViEEGLRenderer(String name) {
        eglRenderer = new EglRenderer(name);
    }

    public EglRenderer getEglRender() {
        return eglRenderer;
    }

    /**
     * Initialize this class, sharing resources with |sharedContext|. It is allowed to call init() to
     * reinitialize the renderer after a previous init()/release() cycle.
     */
    public void init(EglBase.Context sharedContext, ITextureFrameHelper helper) {
        init(sharedContext, EglBase.CONFIG_PLAIN, new GlRectDrawer(), helper);
    }

    /**
     * Initialize this class, sharing resources with |sharedContext|. The custom |drawer| will be used
     * for drawing frames on the EGLSurface. This class is responsible for calling release() on
     * |drawer|. It is allowed to call init() to reinitialize the renderer after a previous
     * init()/release() cycle.
     */
    public void init(final EglBase.Context sharedContext, final int[] configAttributes,
                     RendererCommon.GlDrawer drawer, ITextureFrameHelper helper) {
        //ThreadUtils.checkIsOnMainThread();
        eglRenderer.init(sharedContext, configAttributes, drawer, helper);
    }

    public void setRenderView(SurfaceView view, SurfaceHolder.Callback listener) {
        ThreadUtils.checkIsOnMainThread();
        if (mHasEglSurface) {
            throw new IllegalStateException(ERROR_EGL);
        }

        mSurfaceView = view;
        mSurfaceViewListener = listener;
        mSurfaceView.getHolder().addCallback(this);
    }
    public void setRenderView(TextureView view, TextureView.SurfaceTextureListener listener) {
        ThreadUtils.checkIsOnMainThread();
        if (mHasEglSurface) {
            throw new IllegalStateException(ERROR_EGL);
        }

        mTextureView = view;
        mSurfaceTextureListener = listener;
        mTextureView.setSurfaceTextureListener(this);
    }

    public void setRenderSurface(Surface surface) {
        ThreadUtils.checkIsOnMainThread();
        if (mHasEglSurface) {
            throw new IllegalStateException(ERROR_EGL);
        }

        mSurface = surface;
        eglRenderer.createEglSurface(surface);
        mHasEglSurface = true;
    }

    public void setRenderSurface(SurfaceTexture st) {
        ThreadUtils.checkIsOnMainThread();
        if (mHasEglSurface) {
            throw new IllegalStateException(ERROR_EGL);
        }

        mSurfaceTexture = st;
        eglRenderer.createEglSurface(st);
        mHasEglSurface = true;
    }

    /**
     * Block until any pending frame is returned and all GL resources released, even if an interrupt
     * occurs. If an interrupt occurs during release(), the interrupt flag will be set. This function
     * should be called before the Activity is destroyed and the EGLContext is still valid. If you
     * don't call this function, the GL resources might leak.
     */
    public void release() {
        eglRenderer.release();
    }

    public boolean start() {
        return mStarted = true;
    }

    public void stop() {
        mStarted = false;
    }

    // TODO(Haonong Yu): 2019-07-17 releaseCallback is guaranteed to be called
    public void consume(int texId, int format, int width, int height, int rotation, long ts,
                        float[] matrix, Runnable releaseCallback) {
        VideoFrame.TextureBuffer.Type type = null;
        if (format == AgoraVideoFrame.FORMAT_TEXTURE_OES) {
            type = VideoFrame.TextureBuffer.Type.OES;
        } else if (format == AgoraVideoFrame.FORMAT_TEXTURE_2D) {
            type = VideoFrame.TextureBuffer.Type.RGB;
        }
        if (!mStarted || type == null) {
            if (releaseCallback != null) {
                // TODO(Haonong Yu): 2019-07-17 thread safety and logging.
                releaseCallback.run();
            }
        } else {
            rendTextureFrame(texId, type, width, height, rotation, ts, matrix, releaseCallback);
        }
    }

    public void consume(ByteBuffer buffer, int format, int width, int height, int rotation, long ts) {
        if (!mStarted)
            return;

        if (format == AgoraVideoFrame.FORMAT_I420) {
            rendI420Frame(buffer, format, width, height, rotation, ts);
        } else if (format == AgoraVideoFrame.FORMAT_RGBA) {
            rendRGBAFrame(buffer, format, width, height, rotation, ts);
        }
    }

    public void consume(byte[] data, int format, int width, int height, int rotation, long ts) {
        if (!mStarted)
            return;

        if (format == AgoraVideoFrame.FORMAT_I420) {
            rendI420Frame(data, format, width, height, rotation, ts);
        } else if (format == AgoraVideoFrame.FORMAT_RGBA) {
            rendRGBAFrame(data,format, width, height, rotation, ts);
        }
    }

    private void rendTextureFrame(int texId, VideoFrame.TextureBuffer.Type type, int width,
                                  int height, int rotation, long ts, float[] matrix,
                                  Runnable releaseCallback) {
        Matrix m = RendererCommon.convertMatrixToAndroidGraphicsMatrix(matrix);
        final VideoFrame.TextureBuffer buffer = new TextureBufferImpl(width, height,
                type, texId, m, null, releaseCallback);

        VideoFrame frame = new VideoFrame(buffer, rotation, ts);
        eglRenderer.renderFrame(frame);
        frame.release();
    }

    private void rendI420Frame(ByteBuffer data, int format, int width, int height, int rotation, long ts) {
        if (data == null) return;

        byte[] tmp = new byte[data.remaining()];
        data.get(tmp, 0, tmp.length);
        JavaI420Buffer buffer = JavaI420Buffer.createYUV(tmp, width, height);
        if (buffer == null) return;

        final VideoFrame frame = new VideoFrame(buffer, rotation, ts);
        eglRenderer.renderFrame(frame);
        frame.release();
    }

    private void rendI420Frame(byte[] data, int format, int width, int height, int rotation, long ts) {
        if (data == null || data.length == 0) return;

        JavaI420Buffer buffer = JavaI420Buffer.createYUV(data, width, height);
        if (buffer == null) return;

        final VideoFrame frame = new VideoFrame(buffer, rotation, ts);
        eglRenderer.renderFrame(frame);
        frame.release();
    }

    private void rendRGBAFrame(final ByteBuffer data, int format, int width, int height, int rotation, long ts) {
        if (data == null) return;

        RgbaBuffer buffer = new RgbaBuffer(data, width, height, new Runnable() {
            @Override
            public void run() {
                releaseBuffer(data);
            }
        });
        final VideoFrame frame = new VideoFrame(buffer, rotation, ts);
        eglRenderer.renderFrame(frame);
        frame.release();
    }

    private void rendRGBAFrame(byte[] data, int format, int width, int height, int rotation, long ts) {
        if (data == null || data.length == 0) return;

        final ByteBuffer buffer = ByteBuffer.wrap(data);
        // buffer.put(data);
        final RgbaBuffer rgbaBuffer = new RgbaBuffer(buffer, width, height, new Runnable() {
            @Override
            public void run() {
                releaseBuffer(buffer);
            }
        });
        final VideoFrame frame = new VideoFrame(rgbaBuffer, rotation, ts);
        eglRenderer.renderFrame(frame);
        frame.release();
    }

    /************************** SurfaceView callback ***************************/
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated..");
        ThreadUtils.checkIsOnMainThread();
        eglRenderer.createEglSurface(holder.getSurface());

        mHasEglSurface = true;
        if (mSurfaceViewListener != null) {
            mSurfaceViewListener.surfaceCreated(holder);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        ThreadUtils.checkIsOnMainThread();
        Log.e(TAG, "surfaceChanged: format: " + format + " size: " + width + "x" + height);
        eglRenderer.surfaceSizeChanged(width,height);
        if (mSurfaceViewListener != null) {
            mSurfaceViewListener.surfaceChanged(holder, format, width, height);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed..");
        ThreadUtils.checkIsOnMainThread();
        final CountDownLatch completionLatch = new CountDownLatch(1);
        eglRenderer.releaseEglSurface(new Runnable() {
            @Override
            public void run() {
                completionLatch.countDown();
            }
        });
        ThreadUtils.awaitUninterruptibly(completionLatch);

        if (mSurfaceViewListener != null) {
            mSurfaceViewListener.surfaceDestroyed(holder);
        }
    }

    /************************** SurfaceView callback end ***************************/

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        Log.i(TAG, "onSurfaceTextureAvailable..");
        ThreadUtils.checkIsOnMainThread();
        eglRenderer.createEglSurface(surface);

        mHasEglSurface = true;

        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureAvailable(surface, width, height);
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        Log.e(TAG, "onSurfaceTextureSizeChanged: width- " + width + ", mHeight: " + height);

        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureSizeChanged(surface, width, height);
        }
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.i(TAG, "onSurfaceTextureDestroyed..");
        ThreadUtils.checkIsOnMainThread();
        final CountDownLatch completionLatch = new CountDownLatch(1);
        eglRenderer.releaseEglSurface(new Runnable() {
            @Override
            public void run() {
                completionLatch.countDown();
            }
        });
        ThreadUtils.awaitUninterruptibly(completionLatch);

        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureDestroyed(surface);
        }

        return true;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        if (mSurfaceTextureListener != null) {
            mSurfaceTextureListener.onSurfaceTextureUpdated(surface);
        }
    }

    private void releaseBuffer(ByteBuffer buffer) {
        buffer = null;
    }
}
