/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package io.agora.tutorials1v1vcall;

import android.annotation.TargetApi;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

import java.util.concurrent.Callable;

import io.agora.rtc.gl.EglBase;
import io.agora.rtc.gl.GlUtil;
import io.agora.rtc.gl.VideoFrame;
import io.agora.rtc.gl.YuvConverter;
import io.agora.rtc.utils.ThreadUtils;

/**
 * Helper class to create and synchronize access to a SurfaceTexture. The caller will get notified
 * of new frames in {@link TextureSource#onTextureFrameAvailable(int, float[], long) onTextureFrameAvailable}, and should call {@link returnTextureFrame() returnTextureFrame} when done with the frame. Only one texture frame can be in flight at once, so {@link returnTextureFrame() returnTextureFrame} must be called in order to receive a new frame. Call {@link stopListening() stopListening} to stop receiveing new frames. Call {@link dispose() dispose} to release all resources once the texture frame is returned.
 * Note that there is a C++ counter part of this class that optionally can be used. It is used for
 * wrapping texture frames into webrtc::VideoFrames and also handles calling {@link returnTextureFrame() returnTextureFrame}
 * when the webrtc::VideoFrame is no longer used.
 */
public class SurfaceTextureHelper implements ITextureFrameHelper {
  private static final String TAG = "SurfaceTextureHelper2";
  /**
   * Callback interface for being notified that a new texture frame is available. The calls will be
   * made on the SurfaceTextureHelper handler thread, with a bound EGLContext. The callee is not
   * allowed to make another EGLContext current on the calling thread.
   */
  public interface OnTextureFrameAvailableListener {
    abstract void onTextureFrameAvailable(
            int oesTextureId, float[] transformMatrix, long timestampNs);
  }

  /**
   * Construct a new SurfaceTextureHelper sharing OpenGL resources with |sharedContext|. A dedicated
   * thread and handler is created for handling the SurfaceTexture. May return null if EGL fails to
   * initialize a pixel buffer surface and make it current.
   */
  public static SurfaceTextureHelper create(
          final String threadName, final EglBase.Context sharedContext) {
    final HandlerThread thread = new HandlerThread(threadName);
    thread.start();
    final Handler handler = new Handler(thread.getLooper());

    // The onFrameAvailable() callback will be executed on the SurfaceTexture ctor thread. See:
    // http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android/5.1.1_r1/android/graphics/SurfaceTexture.java#195.
    // Therefore, in order to control the callback thread on API lvl < 21, the SurfaceTextureHelper
    // is constructed on the |handler| thread.
    return ThreadUtils.invokeAtFrontUninterruptibly(handler, new Callable<SurfaceTextureHelper>() {
      @Override
      public SurfaceTextureHelper call() {
        try {
          return new SurfaceTextureHelper(sharedContext, handler);
        } catch (RuntimeException e) {
          Log.e(TAG, threadName + " create failure", e);
          return null;
        }
      }
    });
  }

  private final Handler handler;
  private final EglBase eglBase;
  private final SurfaceTexture surfaceTexture;
  private final int oesTextureId;
  private YuvConverter yuvConverter;

  // These variables are only accessed from the |handler| thread.
  private OnTextureFrameAvailableListener listener;
  // The possible states of this class.
  private boolean hasPendingTexture = false;
  private volatile boolean isTextureInUse = false;
  private boolean isQuitting = false;
  // |pendingListener| is set in setListener() and the runnable is posted to the handler thread.
  // setListener() is not allowed to be called again before {@link stopListening() stopListening}, so this is thread safe.
  private OnTextureFrameAvailableListener pendingListener;
  final Runnable setListenerRunnable = new Runnable() {
    @Override
    public void run() {
      Log.d(TAG, "Setting listener to " + pendingListener);
      listener = pendingListener;
      pendingListener = null;
      // May have a pending frame from the previous capture session - drop it.
      if (hasPendingTexture) {
        // Calling updateTexImage() is neccessary in order to receive new frames.
        updateTexImage();
        hasPendingTexture = false;
      }
    }
  };

  private SurfaceTextureHelper(EglBase.Context sharedContext, Handler handler) {
    if (handler.getLooper().getThread() != Thread.currentThread()) {
      throw new IllegalStateException("SurfaceTextureHelper must be created on the handler thread");
    }
    this.handler = handler;

    eglBase = EglBase.create(sharedContext, EglBase.CONFIG_PIXEL_BUFFER);
    try {
      // Both these statements have been observed to fail on rare occasions, see BUG=webrtc:5682.
      eglBase.createDummyPbufferSurface();
      eglBase.makeCurrent();
    } catch (RuntimeException e) {
      // Clean up before rethrowing the exception.
      Log.e(TAG, "SurfaceTextureHelper: failed to create pbufferSurface!!");
      eglBase.release();
      handler.getLooper().quit();
      throw e;
    }

    oesTextureId = GlUtil.generateTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES);
    surfaceTexture = new SurfaceTexture(oesTextureId);
    setOnFrameAvailableListener(surfaceTexture, new SurfaceTexture.OnFrameAvailableListener() {
      @Override
      public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        hasPendingTexture = true;
        tryDeliverTextureFrame();
      }
    }, handler);
  }

  @TargetApi(21)
  private static void setOnFrameAvailableListener(SurfaceTexture surfaceTexture,
                                                  SurfaceTexture.OnFrameAvailableListener listener, Handler handler) {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
      surfaceTexture.setOnFrameAvailableListener(listener, handler);
    } else {
      // The documentation states that the listener will be called on an arbitrary thread, but in
      // pratice, it is always the thread on which the SurfaceTexture was constructed. There are
      // assertions in place in case this ever changes. For API >= 21, we use the new API to
      // explicitly specify the handler.
      surfaceTexture.setOnFrameAvailableListener(listener);
    }
  }


  public EglBase.Context getEglContext() {
    return eglBase.getEglBaseContext();
  }

  /**
   * Start to stream textures to the given |listener|. If you need to change listener, you need to
   * call {@link stopListening() stopListening} first.
   */
  public void startListening(final OnTextureFrameAvailableListener listener) {
    if (this.listener != null || this.pendingListener != null) {
      throw new IllegalStateException("SurfaceTextureHelper listener has already been set.");
    }
    this.pendingListener = listener;
    handler.post(setListenerRunnable);
  }

  /**
   * Stop listening. The listener set in startListening() is guaranteded to not receive any more
   * onTextureFrameAvailable() callbacks after this function returns.
   */
  public void stopListening() {
    Log.d(TAG, "stopListening()");
    handler.removeCallbacks(setListenerRunnable);
    ThreadUtils.invokeAtFrontUninterruptibly(handler, new Runnable() {
      @Override
      public void run() {
        listener = null;
        pendingListener = null;
      }
    });
  }

  /**
   * Retrieve the underlying SurfaceTexture. The SurfaceTexture should be passed in to a video
   * producer such as a camera or decoder.
   */
  public SurfaceTexture getSurfaceTexture() {
    return surfaceTexture;
  }

  /**
   * Retrieve the handler that calls onTextureFrameAvailable(). This handler is valid until
   * {@link dispose() dispose} is called.
   */
  public Handler getHandler() {
    return handler;
  }

  /**
   * Call this function to signal that you are done with the frame received in
   * onTextureFrameAvailable(). Only one texture frame can be in flight at once, so you must call
   * this function in order to receive a new frame.
   */
  public void returnTextureFrame() {
    handler.post(new Runnable() {
      @Override
      public void run() {
        isTextureInUse = false;
        if (isQuitting) {
          release();
        } else {
          tryDeliverTextureFrame();
        }
      }
    });
  }

  public void returnTexture() {
    returnTextureFrame();
  }

  public boolean isTextureInUse() {
    return isTextureInUse;
  }

  /**
   * Call disconnect() to stop receiving frames. OpenGL resources are released and the handler is
   * stopped when the texture frame has been returned by a call to {@link returnTextureFrame() returnTextureFrame}. You are
   * guaranteed to not receive any more onTextureFrameAvailable() after this function returns.
   */
  public void dispose() {
    Log.d(TAG, "dispose()");
    ThreadUtils.invokeAtFrontUninterruptibly(handler, new Runnable() {
      @Override
      public void run() {
        isQuitting = true;
        if (!isTextureInUse) {
          release();
        }
      }
    });
  }

  /**
   * Posts to the correct thread to convert |textureBuffer| to I420. Must only be called with
   * textures generated by this SurfaceTextureHelper.
   */
  public VideoFrame.I420Buffer textureToYuv(final VideoFrame.TextureBuffer textureBuffer) {
    if (textureBuffer.getTextureId() != oesTextureId) {
      throw new IllegalStateException("textureToByteBuffer called with unexpected textureId");
    }

    final VideoFrame.I420Buffer[] result = new VideoFrame.I420Buffer[1];
    ThreadUtils.invokeAtFrontUninterruptibly(handler, new Runnable() {
      @Override
      public void run() {
      if (yuvConverter == null) {
        yuvConverter = new YuvConverter();
      }
      result[0] = yuvConverter.convert(textureBuffer);
      }
    });
    return result[0];
  }

  private void updateTexImage() {
    // SurfaceTexture.updateTexImage apparently can compete and deadlock with eglSwapBuffers,
    // as observed on Nexus 5. Therefore, synchronize it with the EGL functions.
    // See https://bugs.chromium.org/p/webrtc/issues/detail?id=5702 for more info.
    try {
      synchronized (EglBase.lock) {
        surfaceTexture.updateTexImage();
      }
    } catch (IllegalStateException ex) {
      Log.e(TAG, "SurfaceTextureHelper: failed to updateTexImage!!");
    }
  }

  private void tryDeliverTextureFrame() {
    if (handler.getLooper().getThread() != Thread.currentThread()) {
      throw new IllegalStateException("Wrong thread.");
    }
    if (isQuitting || !hasPendingTexture || isTextureInUse || listener == null) {
      return;
    }
    isTextureInUse = true;
    hasPendingTexture = false;

    updateTexImage();

    final float[] transformMatrix = new float[16];
    surfaceTexture.getTransformMatrix(transformMatrix);
    final long timestampNs = surfaceTexture.getTimestamp();
    listener.onTextureFrameAvailable(oesTextureId, transformMatrix, timestampNs);
  }

  private void release() {
    if (handler.getLooper().getThread() != Thread.currentThread()) {
      throw new IllegalStateException("Wrong thread.");
    }
    if (isTextureInUse || !isQuitting) {
      throw new IllegalStateException("Unexpected release.");
    }
    if (yuvConverter != null) {
      yuvConverter.release();
    }
    GLES20.glDeleteTextures(1, new int[] {oesTextureId}, 0);
    surfaceTexture.release();
    eglBase.release();
    handler.getLooper().quit();
  }
}
