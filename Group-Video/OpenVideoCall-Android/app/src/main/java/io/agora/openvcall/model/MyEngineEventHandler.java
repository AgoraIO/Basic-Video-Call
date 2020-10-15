package io.agora.openvcall.model;

import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.RtcEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;

public class MyEngineEventHandler extends IRtcEngineEventHandler {
    private final Logger log = LoggerFactory.getLogger(this.getClass());
    private final ConcurrentHashMap<AGEventHandler, Integer> mEventHandlerList = new ConcurrentHashMap<>();

    public void addEventHandler(AGEventHandler handler) {
        this.mEventHandlerList.put(handler, 0);
    }

    public void removeEventHandler(AGEventHandler handler) {
        this.mEventHandlerList.remove(handler);
    }

    /**
     * Occurs when the first remote video frame is received and decoded.
     * This callback is triggered in either of the following scenarios:
     * - The remote user joins the channel and sends the video stream.
     * - The remote user stops sending the video stream and re-sends it after 15 seconds. Possible reasons include:
     * - The remote user leaves channel.
     * - The remote user drops offline.
     * - The remote user calls the muteLocalVideoStream method.
     * - The remote user calls the disableVideo method.
     * @param uid User ID of the remote user sending the video streams.
     * @param width Width (pixels) of the video stream.
     * @param height Height (pixels) of the video stream.
     * @param elapsed Time elapsed (ms) from the local user calling the joinChannel method until this callback is triggered.
     */
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

    /**
     * Occurs when the first local video frame is rendered.
     * This callback is triggered after the first local video frame is rendered on the local video window.
     *
     * @param width Width (pixels) of the first local video frame.
     * @param height Height (pixels) of the first local video frame.
     * @param elapsed Time elapsed (ms) from the local user calling joinChannel until this callback is triggered. If startPreview is called before joinChannel, elapsed is the time elapsed (ms) from the local user calling startPreview until this callback is triggered.
     */
    @Override
    public void onFirstLocalVideoFrame(int width, int height, int elapsed) {
        log.debug("onFirstLocalVideoFrame " + width + " " + height + " " + elapsed);
    }

    /**
     * Occurs when a remote user (Communication)/host (Live Broadcast) joins the channel.
     *
     *     Communication profile: This callback notifies the app when another user joins the channel. If other users are already in the channel, the SDK also reports to the app on the existing users.
     *     Live Broadcast profile: This callback notifies the app when the host joins the channel. If other hosts are already in the channel, the SDK also reports to the app on the existing hosts. We recommend having at most 17 hosts in a channel
     *
     * The SDK triggers this callback under one of the following circumstances:
     *
     *     A remote user/host joins the channel by calling the joinChannel method.
     *     A remote user switches the user role to the host by calling the setClientRole method after joining the channel.
     *     A remote user/host rejoins the channel after a network interruption.
     *     The host injects an online media stream into the channel by calling the addInjectStreamUrl method.
     *
     * @param uid ID of the user or host who joins the channel.
     * @param elapsed Time delay (ms) from the local user calling joinChannel/setClientRole until this callback is triggered.
     */
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

    /**
     * Occurs when a remote user (Communication)/host (Live Broadcast) leaves the channel.
     *
     * There are two reasons for users to become offline:
     *
     *     Leave the channel: When the user/host leaves the channel, the user/host sends a goodbye message. When this message is received, the SDK determines that the user/host leaves the channel.
     *     Drop offline: When no data packet of the user or host is received for a certain period of time (20 seconds for the communication profile, and more for the live broadcast profile), the SDK assumes that the user/host drops offline. A poor network connection may lead to false detections, so we recommend using the Agora RTM SDK for reliable offline detection.
     *
     * @param uid ID of the user or host who leaves the channel or goes offline.
     * @param reason Reason why the user goes offline:
     *
     *     USER_OFFLINE_QUIT(0): The user left the current channel.
     *     USER_OFFLINE_DROPPED(1): The SDK timed out and the user dropped offline because no data packet was received within a certain period of time. If a user quits the call and the message is not passed to the SDK (due to an unreliable channel), the SDK assumes the user dropped offline.
     *     USER_OFFLINE_BECOME_AUDIENCE(2): (Live broadcast only.) The client role switched from the host to the audience.
     */
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

    /**
     * Occurs when a remote user stops/resumes sending the video stream.
     * The SDK triggers this callback when the remote user stops or resumes sending the video stream by calling the muteLocalVideoStream method.
     * @param uid ID of the remote user.
     * @param muted Whether the remote user's video stream playback pauses/resumes.
     */
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

    /**
     * Reports the statistics of the RtcEngine once every two seconds.
     * @param stats RTC engine statistics: RtcStats.
     */
    @Override
    public void onRtcStats(RtcStats stats) {
    }

    /**
     * Reports the statistics of the video stream from each remote user/host. The SDK triggers this callback once every two seconds for each remote user/host. If a channel includes multiple remote users, the SDK triggers this callback as many times.
     * @param stats  of the received remote video streams: RemoteVideoStats.
     */
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

    /**
     * Reports which users are speaking and the speakers' volume, and whether the local user is speaking.
     *
     * This callback reports the IDs and volumes of the loudest speakers (at most 3) at the moment in the channel, and whether the local user is speaking.
     *
     * By default, this callback is disabled. You can enable it by calling the enableAudioVolumeIndication method. Once enabled, this callback is triggered at the set interval, regardless of whether a user speaks or not.
     *
     * The SDK triggers two independent onAudioVolumeIndication callbacks at one time, which separately report the volume information of the local user and all the remote speakers. For more information, see the detailed parameter descriptions.
     *
     * @param speakerInfos An array containing the user ID and volume information for each speaker: AudioVolumeInfo.
     *
     *     In the local user’s callback, this array contains the following members:
     *         uid = 0,
     *         volume = totalVolume, which reports the sum of the voice volume and audio-mixing volume of the local user, and
     *         vad, which reports the voice activity status of the local user.
     *
     *     In the remote speakers' callback, this array contains the following members:
     *         uid of each remote speaker,
     *         volume, which reports the sum of the voice volume and audio-mixing volume of each remote speaker, and
     *         vad = 0.
     *
     *     An empty speakers array in the callback indicates that no remote user is speaking at the moment
     * @param totalVolume Total volume after audio mixing. The value ranges between 0 (lowest volume) and 255 (highest volume).
     *
     *     In the local user’s callback, totalVolume is the sum of the voice volume and audio-mixing volume of the local user.
     *     In the remote speakers' callback, totalVolume is the sum of the voice volume and audio-mixing volume of all remote speakers.
     */
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

    /**
     * Occurs when a user leaves the channel.
     *
     * When the app calls the leaveChannel method, the SDK uses this callback to notify the app when the user leaves the channel.
     *
     * With this callback, the application retrieves the channel information, such as the call duration and statistics.
     *
     * @param stats Statistics of the call: RtcStats
     */
    @Override
    public void onLeaveChannel(RtcStats stats) {

    }

    /**
     * Reports the last mile network quality of the local user once every two seconds before the user joins the channel. Last mile refers to the connection between the local device and Agora's edge server. After the application calls the enableLastmileTest method, this callback reports once every two seconds the uplink and downlink last mile network conditions of the local user before the user joins the channel.
     *
     * @param quality The last mile network quality based on the uplink and dowlink packet loss rate and jitter:
     *
     *     QUALITY_UNKNOWN(0): The quality is unknown.
     *     QUALITY_EXCELLENT(1): The quality is excellent.
     *     QUALITY_GOOD(2): The quality is quite good, but the bitrate may be slightly lower than excellent.
     *     QUALITY_POOR(3): Users can feel the communication slightly impaired.
     *     QUALITY_BAD(4): Users can communicate not very smoothly.
     *     QUALITY_VBAD(5): The quality is so bad that users can barely communicate.
     *     QUALITY_DOWN(6): The network is disconnected and users cannot communicate at all.
     *     QUALITY_DETECTING(8): The SDK is detecting the network quality.
     */
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

    /**
     * Reports the last-mile network probe result.
     *
     * Since
     *     v2.4.0.
     *
     * The SDK triggers this callback within 30 seconds after the app calls the startLastmileProbeTest method.
     *
     * @param result The uplink and downlink last-mile network probe test result. For details, see LastmileProbeResult.
     */
    @Override
    public void onLastmileProbeResult(LastmileProbeResult result) {
        log.debug("onLastmileProbeResult " + result);
        Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
        while (it.hasNext()) {
            AGEventHandler handler = it.next();
            if (handler instanceof BeforeCallEventHandler) {
                ((BeforeCallEventHandler) handler).onLastmileProbeResult(result);
            }
        }
    }


    /**
     * Reports an error during SDK runtime.
     *
     * In most cases, the SDK cannot fix the issue and resume running. The SDK requires the app to take action or informs the user about the issue.
     *
     * For example, the SDK reports an ERR_START_CALL error when failing to initialize a call. The app informs the user that the call initialization failed and invokes the leaveChannel method to leave the channel. For detailed error codes, see Error Codes.
     *
     * @param error Error Code
     */
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

    /**
     * Occurs when the local user receives a remote data stream.
     *
     * The SDK triggers this callback when the local user receives the stream message that the remote user sends by calling the sendStreamMessage method.
     *
     * @param uid User ID of the remote user sending the data stream.
     * @param streamId Stream ID.
     * @param data Data received by the local user.
     */
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

    /**
     * Occurs when the local user fails to receive a remote data stream.
     *
     * The SDK triggers this callback when the local user fails to receive the stream message that the remote user sends by calling the sendStreamMessage method.
     *
     * @param uid User ID of the remote user sending the data stream.
     * @param streamId Stream ID.
     * @param error Error Code.
     * @param missed The number of lost messages.
     * @param cached The number of incoming cached messages when the data stream is interrupted.
     */
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

    /**
     * Occurs when the SDK cannot reconnect to Agora's edge server 10 seconds after its connection to the server is interrupted.
     * The SDK triggers this callback when it cannot connect to the server 10 seconds after calling joinChannel(), regardless of whether it is in the channel or not.
     * If the SDK fails to rejoin the channel 20 minutes after being disconnected from Agora's edge server, the SDK stops rejoining the channel.
     */
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

    /**
     * Occurs when the local user joins a specified channel.
     *
     * The channel name assignment is based on channelName specified in the joinChannel method.
     *
     * If the uid is not specified when joinChannel is called, the server automatically assigns a uid.
     *
     * @param channel Channel name.
     * @param uid User ID.
     * @param elapsed Time elapsed (ms) from the user calling joinChannel until this callback is triggered.
     */
    @Override
    public void onJoinChannelSuccess(String channel, int uid, int elapsed) {
        log.debug("onJoinChannelSuccess " + channel + " " + (uid & 0xFFFFFFFFL) + "(" + uid + ") " + elapsed);

        Iterator<AGEventHandler> it = mEventHandlerList.keySet().iterator();
        while (it.hasNext()) {
            AGEventHandler handler = it.next();
            if (handler instanceof DuringCallEventHandler) {
                ((DuringCallEventHandler) handler).onJoinChannelSuccess(channel, uid, elapsed);
            }
        }
    }

    /**
     * Occurs when the local audio playback route changes.
     * This callback returns that the audio route switched to an earpiece, speakerphone, headset, or Bluetooth device.
     *
     * @param routing The definition of the routing is listed as follows:
     *
     *      AUDIO_ROUTE_DEFAULT(-1): Default audio route.
     *      AUDIO_ROUTE_HEADSET(0): Headset.
     *      AUDIO_ROUTE_EARPIECE(1): Earpiece.
     *      AUDIO_ROUTE_HEADSETNOMIC(2): Headset with no microphone.
     *      AUDIO_ROUTE_SPEAKERPHONE(3): Speakerphone.
     *      AUDIO_ROUTE_LOUDSPEAKER(4): Loudspeaker.
     *      AUDIO_ROUTE_HEADSETBLUETOOTH(5): Bluetooth headset.
     */
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

    /**
     * Reports a warning during SDK runtime.
     *
     * In most cases, the app can ignore the warning reported by the SDK because the SDK can usually fix the issue and resume running.
     *
     * For instance, the SDK may report a WARN_LOOKUP_CHANNEL_TIMEOUT warning upon disconnection with the server and tries to reconnect. For detailed warning codes, see Warning Codes.
     *
     * @param warn Warning Code
     */
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

    /**
     * Occurs when the state of the local user's audio mixing file changes.
     *
     * Since
     *     v2.4.0. When you call the startAudioMixing method and the state of audio mixing file changes, the Agora SDK triggers this callback.
     *
     *         When the audio mixing file plays, pauses playing, or stops playing, this callback returns 710, 711, or 713 in state, and 0 in errorCode.
     *         When exceptions occur during playback, this callback returns 714 in state and an error in errorCode.
     *         If the local audio mixing file does not exist, or if the SDK does not support the file format or cannot access the music file URL, the SDK returns WARN_AUDIO_MIXING_OPEN_ERROR = 701.
     *
     * @param state The state code:
     *
     *     MEDIA_ENGINE_AUDIO_EVENT_MIXING_PLAY(710): the audio mixing file is playing.
     *     MEDIA_ENGINE_AUDIO_EVENT_MIXING_PAUSED(711): the audio mixing file pauses playing.
     *     MEDIA_ENGINE_AUDIO_EVENT_MIXING_STOPPED(713): the audio mixing file stops playing.
     *     MEDIA_ENGINE_AUDIO_EVENT_MIXING_ERROR(714): an exception occurs when playing the audio mixing file. See the errorCode for details.
     *
     * @param errorCode The error code:
     *
     *     MEDIA_ENGINE_AUDIO_ERROR_MIXING_OPEN(701): the SDK cannot open the audio mixing file.
     *     MEDIA_ENGINE_AUDIO_ERROR_MIXING_TOO_FREQUENT(702): the SDK opens the audio mixing file too frequently.
     *     MEDIA_ENGINE_AUDIO_EVENT_MIXING_INTERRUPTED_EOF(703): the audio mixing file playback is interrupted.
     */
    @Override
    public void onAudioMixingStateChanged(int state, int errorCode) {
        log.debug("onAudioMixingStateChanged() state = [" + state + "], errorCode = [" + errorCode + "]");
    }
}
