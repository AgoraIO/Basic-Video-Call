package io.agora.propeller;

import android.view.SurfaceView;

public class UserStatusData {
    public static final int DEFAULT_STATUS = 0;
    public static final int VIDEO_MUTED = 1;
    public static final int AUDIO_MUTED = VIDEO_MUTED << 1;

    public static final int DEFAULT_VOLUME = 0;

    public UserStatusData(int uid, SurfaceView view, Integer status, int volume) {
        this(uid, view, status, volume, null);
    }

    public UserStatusData(int uid, SurfaceView view, Integer status, int volume, VideoInfoData i) {
        this.mUid = uid;
        this.mView = view;
        this.mStatus = status;
        this.mVolume = volume;
        this.mVideoInfo = i;
    }

    public int mUid;

    public SurfaceView mView;

    public Integer mStatus; // if status is null, do nothing

    public int mVolume;

    private VideoInfoData mVideoInfo;

    public void setVideoInfo(VideoInfoData video) {
        mVideoInfo = video;
    }

    public VideoInfoData getVideoInfoData() {
        return mVideoInfo;
    }

    @Override
    public String toString() {
        return "UserStatusData{" +
                "mUid=" + (mUid & 0XFFFFFFFFL) +
                ", mView=" + mView +
                ", mStatus=" + mStatus +
                ", mVolume=" + mVolume +
                '}';
    }
}
