package io.agora.openvcall.model;

import io.agora.rtc.IRtcEngineEventHandler;

public interface LastMileEventHandler {
    void onLastmileQuality(int quality);

    void onLastmileProbeResult(IRtcEngineEventHandler.LastmileProbeResult result);
}
