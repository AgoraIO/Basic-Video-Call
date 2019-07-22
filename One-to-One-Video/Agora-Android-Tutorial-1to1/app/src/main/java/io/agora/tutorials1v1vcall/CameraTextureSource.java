package io.agora.tutorials1v1vcall;

import android.content.Context;
import android.hardware.Camera;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.Surface;
import android.view.WindowManager;

import java.io.IOException;
import java.util.List;

import io.agora.rtc.gl.EglBase;
import io.agora.rtc.gl.RendererCommon;
import io.agora.rtc.mediaio.SurfaceTextureHelper;

public class CameraTextureSource implements SurfaceTextureHelper.OnTextureFrameAvailableListener {
  private static final String TAG = CameraTextureSource.class.getSimpleName();

  private Context context;
  private Camera camera;
  private Camera.CameraInfo info;
  private SurfaceTextureHelper surfaceTextureHelper;
  private int width;
  private int height;
  private SurfaceTextureHelper.OnTextureFrameAvailableListener listener;

  public CameraTextureSource(Context context, int width, int height,
                             @NonNull SurfaceTextureHelper.OnTextureFrameAvailableListener listener) {
    this.context = context;
    this.width = width;
    this.height = height;
    this.listener = listener;
  }

  @Override
  public void onTextureFrameAvailable(int oesTextureId, float[] transformMatrix, long timestampNs) {
    if (info.facing == android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT) {
      // Undo the mirror that the OS "helps" us with.
      // http://developer.android.com/reference/android/hardware/Camera.html#setDisplayOrientation(int)
      transformMatrix = RendererCommon.multiplyMatrices(transformMatrix, RendererCommon.horizontalFlipMatrix());
    }
    listener.onTextureFrameAvailable(oesTextureId, transformMatrix, timestampNs);
    surfaceTextureHelper.returnTextureFrame();
  }

  public boolean initialize() {
    surfaceTextureHelper = SurfaceTextureHelper.create("TexCamThread", null);
    surfaceTextureHelper.getSurfaceTexture().setDefaultBufferSize(width, height);
    surfaceTextureHelper.startListening(this);

    try {
      openCamera();
      camera.setPreviewTexture(surfaceTextureHelper.getSurfaceTexture());
    } catch (IOException ex) {
      Log.e(TAG, "initialize: failed to initialize camera device");
      return false;
    }
    return true;
  }

  public void start() {
    camera.startPreview();
  }

  public void stop() {
    camera.stopPreview();
  }

  public void release() {
    if (camera != null) {
      try {
        camera.setPreviewTexture(null);
      } catch (Exception e) {
        Log.e(TAG, "failed to set Preview Texture");
      }
      camera.release();
      camera = null;
      Log.d(TAG, "release camera -- done");
    }

    surfaceTextureHelper.stopListening();
    surfaceTextureHelper.dispose();
    surfaceTextureHelper = null;
  }

  private void openCamera() {
    if (camera != null) {
      throw new RuntimeException("camera already initialized");
    }

    info = new Camera.CameraInfo();
    // Try to find a front-facing camera (e.g. for videoconferencing).
    int numCameras = Camera.getNumberOfCameras();
    for (int i = 0; i < numCameras; i++) {
      Camera.getCameraInfo(i, info);
      if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
        camera = Camera.open(i);
        break;
      }
    }
    if (camera == null) {
      Log.d(TAG, "No front-facing camera found; opening default");
      camera = Camera.open();    // opens first back-facing camera
    }
    if (camera == null) {
      throw new RuntimeException("Unable to open camera");
    }

    Camera.Parameters parms = camera.getParameters();
    List<int[]> frameRates = parms.getSupportedPreviewFpsRange();
    int minFps = (frameRates.get(frameRates.size() - 1))[Camera.Parameters.PREVIEW_FPS_MIN_INDEX];
    int maxFps = (frameRates.get(frameRates.size() - 1))[Camera.Parameters.PREVIEW_FPS_MAX_INDEX ];
    parms.setPreviewFpsRange(minFps, maxFps);
    parms.setPreviewSize(width, height);
    parms.setRecordingHint(true);
    camera.setParameters(parms);

    Camera.Size cameraPreviewSize = parms.getPreviewSize();
    String previewFacts = cameraPreviewSize.width + "x" + cameraPreviewSize.height;
    Log.i(TAG, "Camera config: " + previewFacts);
  }

  public SurfaceTextureHelper getSurfaceTextureHelper() {
    return surfaceTextureHelper;
  }

  public EglBase.Context getEglContext() { return surfaceTextureHelper.getEglContext(); }

  private int getDeviceOrientation() {
    int orientation = 0;

    WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
    switch (wm.getDefaultDisplay().getRotation()) {
      case Surface.ROTATION_90:
        orientation = 90;
        break;
      case Surface.ROTATION_180:
        orientation = 180;
        break;
      case Surface.ROTATION_270:
        orientation = 270;
        break;
      case Surface.ROTATION_0:
      default:
        orientation = 0;
        break;
    }
    return orientation;
  }

  public int getFrameOrientation() {
    int rotation = getDeviceOrientation();
    if (info.facing == android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK) {
      rotation = 360 - rotation;
    }
    return (info.orientation + rotation) % 360;
  }
}