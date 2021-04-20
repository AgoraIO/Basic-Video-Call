package io.agora.openvcall.model;

import io.agora.rtc.Constants;
import io.agora.rtc.video.VideoEncoderConfiguration;

public class ConstantApp {
    public static final String APP_BUILD_DATE = "today";

    public static final int BASE_VALUE_PERMISSION = 0X0001;
    public static final int PERMISSION_REQ_ID_RECORD_AUDIO = BASE_VALUE_PERMISSION + 1;
    public static final int PERMISSION_REQ_ID_CAMERA = BASE_VALUE_PERMISSION + 2;

    public static final int MAX_PEER_COUNT = 4;

    // please check details string_array_resolutions/string_array_frame_rate/string_array_bit_rate at strings_config.xml
    public static VideoEncoderConfiguration.VideoDimensions[] VIDEO_DIMENSIONS = new VideoEncoderConfiguration.VideoDimensions[] {
            VideoEncoderConfiguration.VD_160x120,
            VideoEncoderConfiguration.VD_320x180,
            VideoEncoderConfiguration.VD_320x240,
            VideoEncoderConfiguration.VD_640x360,
            VideoEncoderConfiguration.VD_640x480,
            VideoEncoderConfiguration.VD_1280x720
    };
    public static VideoEncoderConfiguration.FRAME_RATE[] VIDEO_FPS = new VideoEncoderConfiguration.FRAME_RATE[] {
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_1,
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_7,
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_10,
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_15,
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_24,
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_30,
    };

    public static final int DEFAULT_VIDEO_ENC_RESOLUTION_IDX = 2; // default use 240P
    public static final int DEFAULT_VIDEO_ENC_FPS_IDX = 3; // default use 15fps

    public static class PrefManager {
        public static final String PREF_PROPERTY_VIDEO_ENC_RESOLUTION = "pref_profile_index";
        public static final String PREF_PROPERTY_VIDEO_ENC_FPS = "pref_ENC_fps";
        public static final String PREF_PROPERTY_UID = "pOCXx_uid";
    }

    public static final String ACTION_KEY_CHANNEL_NAME = "ecHANEL";
    public static final String ACTION_KEY_ENCRYPTION_KEY = "xdL_encr_key_";
    public static final String ACTION_KEY_ENCRYPTION_MODE = "tOK_edsx_Mode";

    public static class AppError {
        public static final int NO_CONNECTION_ERROR = 3;
    }

    public static String getNetworkQualityDescription(int quality) {
        String inString;
        switch (quality) {
            case Constants.QUALITY_EXCELLENT:
                inString = "Excellent(" + quality + ")";
                break;

            case Constants.QUALITY_GOOD:
                inString = "Good(" + quality + ")";
                break;

            case Constants.QUALITY_POOR:
                inString = "Poor(" + quality + ")";
                break;

            case Constants.QUALITY_BAD:
                inString = "Bad(" + quality + ")";
                break;

            case Constants.QUALITY_VBAD:
                inString = "Very Bad(" + quality + ")";
                break;

            default:
                inString = "Unknown(" + quality + ")";
                break;
        }
        return inString;
    }
}
