package io.agora.openvcall.model;

import io.agora.rtc.video.VideoEncoderConfiguration;

public class EngineConfig {
    VideoEncoderConfiguration.VideoDimensions mVideoDimension;

    public int mUid;

    public String mChannel;

    public void reset() {
        mChannel = null;
    }

    EngineConfig() {
    }
}
