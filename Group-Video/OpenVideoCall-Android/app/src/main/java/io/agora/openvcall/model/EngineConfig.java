package io.agora.openvcall.model;

public class EngineConfig {
    public int mUid;

    public String mChannel;

    public void reset() {
        mChannel = null;
    }

    EngineConfig() {
    }
}
