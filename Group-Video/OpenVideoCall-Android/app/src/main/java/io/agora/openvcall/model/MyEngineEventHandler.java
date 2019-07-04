package io.agora.openvcall.model;

import android.content.Context;

import io.agora.rtc.Constants;
import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.RtcEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;

public class MyEngineEventHandler {

    public MyEngineEventHandler(Context ctx, EngineConfig config) {
        this.mContext = ctx;
        this.mConfig = config;
    }

    private final EngineConfig mConfig;

    private final Context mContext;

    private final ConcurrentHashMap<AGEventHandler, Integer> mEventHandlerList = new ConcurrentHashMap<>();

    public void addEventHandler(AGEventHandler handler) {
        this.mEventHandlerList.put(handler, 0);
    }

    public void removeEventHandler(AGEventHandler handler) {
        this.mEventHandlerList.remove(handler);
    }

    final IRtcEngineEventHandler mRtcEventHandler = new IRtcEngineEventHandler() {
        private final Logger log = LoggerFactory.getLogger(this.getClass());

        @Override
        public void onFirstRemoteVideoDecoded(int uid, int width, int height, int elapsed) {
            log.debug("onFirstRemoteVideoDecoded " + (uid & 0xFFFFFFFFL) + " " + width + " " + height + " " + elapsed);

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onFirstRemoteVideoDecoded(uid, width, height, elapsed);
                }
            }
        }

        @Override
        public void onFirstLocalVideoFrame(int width, int height, int elapsed) {
            log.debug("onFirstLocalVideoFrame " + width + " " + height + " " + elapsed);
        }

        @Override
        public void onUserJoined(int uid, int elapsed) {
            log.debug("onUserJoined " + (uid & 0xFFFFFFFFL) + elapsed);

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onUserJoined(uid);
                }
            }
        }

        @Override
        public void onUserOffline(int uid, int reason) {
            log.debug("onUserOffline " + (uid & 0xFFFFFFFFL) + " " + reason);

            // FIXME this callback may return times
            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onUserOffline(uid, reason);
                }
            }
        }

        @Override
        public void onUserMuteVideo(int uid, boolean muted) {
            log.debug("onUserMuteVideo " + (uid & 0xFFFFFFFFL) + " " + muted);

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_USER_VIDEO_MUTED, uid, muted);
                }
            }
        }

        @Override
        public void onRtcStats(RtcStats stats) {
        }

        @Override
        public void onRemoteVideoStats(RemoteVideoStats stats) {
            log.debug("onRemoteVideoStats " + stats.uid + " " + stats.delay + " " + stats.receivedBitrate + " " + stats.rendererOutputFrameRate + " " + stats.width + " " + stats.height);

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_USER_VIDEO_STATS, stats);
                }
            }
        }

        @Override
        public void onAudioVolumeIndication(AudioVolumeInfo[] speakerInfos, int totalVolume) {
            if (speakerInfos == null) {
                // quick and dirty fix for crash
                // TODO should reset UI for no sound
                return;
            }

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_SPEAKER_STATS, (Object) speakerInfos);
                }
            }
        }

        @Override
        public void onLeaveChannel(RtcStats stats) {

        }

        @Override
        public void onLastmileQuality(int quality) {
            log.debug("onLastmileQuality " + quality);
            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof BeforeCallEventHandler) {
                    ((BeforeCallEventHandler) handler).onLastmileQuality(quality);
                }
            }
        }

        @Override
        public void onLastmileProbeResult(IRtcEngineEventHandler.LastmileProbeResult result) {
            log.debug("onLastmileProbeResult " + result);
            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof BeforeCallEventHandler) {
                    ((BeforeCallEventHandler) handler).onLastmileProbeResult(result);
                }
            }
        }

        @Override
        public void onError(int error) {
            log.debug("onError " + error + " " + RtcEngine.getErrorDescription(error));

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_AGORA_MEDIA_ERROR, error, RtcEngine.getErrorDescription(error));
                }
            }
        }

        @Override
        public void onStreamMessage(int uid, int streamId, byte[] data) {
            log.debug("onStreamMessage " + (uid & 0xFFFFFFFFL) + " " + streamId + " " + Arrays.toString(data));

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_DATA_CHANNEL_MSG, uid, data);
                }
            }
        }

        public void onStreamMessageError(int uid, int streamId, int error, int missed, int cached) {
            log.warn("onStreamMessageError " + (uid & 0xFFFFFFFFL) + " " + streamId + " " + error + " " + missed + " " + cached);

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_AGORA_MEDIA_ERROR, error, "on stream msg error " + (uid & 0xFFFFFFFFL) + " " + missed + " " + cached);
                }
            }
        }

        @Override
        public void onConnectionLost() {
            log.debug("onConnectionLost");

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_APP_ERROR, ConstantApp.AppError.NO_CONNECTION_ERROR);
                }
            }
        }

        @Override
        public void onJoinChannelSuccess(String channel, int uid, int elapsed) {
            log.debug("onJoinChannelSuccess " + channel + " " + (uid & 0xFFFFFFFFL) + "(" + uid + ") " + elapsed);

            mConfig.mUid = uid;

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onJoinChannelSuccess(channel, uid, elapsed);
                }
            }
        }

        public void onRejoinChannelSuccess(String channel, int uid, int elapsed) {
            log.debug("onRejoinChannelSuccess " + channel + " " + uid + " " + elapsed);
        }

        @Override
        public void onAudioRouteChanged(int routing) {
            log.debug("onAudioRouteChanged " + routing);

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_AUDIO_ROUTE_CHANGED, routing);
                }
            }
        }

        public void onWarning(int warn) {
            log.debug("onWarning " + warn);

            String msg = "Check io.agora.rtc.Constants for details";

            Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
            while (it.hasNext()) {
                AGEventHandler handler = it.next();
                if (handler instanceof DuringCallEventHandler) {
                    ((DuringCallEventHandler) handler).onExtraCallback(AGEventHandler.EVENT_TYPE_ON_AGORA_MEDIA_ERROR, warn, msg);
                }
            }
        }

        @Override
        public void onAudioMixingStateChanged(int state, int errorCode) {
            log.debug("onAudioMixingStateChanged() state = [" + state + "], errorCode = [" + errorCode + "]");
        }

        @Override
        public void onAudioMixingFinished() {
            log.debug("onAudioMixingFinished");
        }
    };

}
