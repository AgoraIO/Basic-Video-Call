package io.agora.openvcall.model;

public interface DuringCallEventHandler extends AGEventHandler {

    void onUserJoined(int uid);

    void onFirstRemoteVideoDecoded(int uid, int width, int height, int elapsed);

    void onJoinChannelSuccess(String channel, int uid, int elapsed);

    void onUserOffline(int uid, int reason);

    void onExtraCallback(int type, Object... data);
}
