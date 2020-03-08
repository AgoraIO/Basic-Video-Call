#ifndef CAGORAOBJECT_H
#define CAGORAOBJECT_H

#include <Memory>
#include <mutex>

#include "agoraconfig.h"
extern CAgoraConfig gAgoraConfig;

//Specify your APP ID here
#define APPID ""
#define APP_TOKEN   ""

#include <QString>
#include <QVariant>
#include <IAgoraRtcEngine.h>
#include <IAgoraMediaEngine.h>
#include <AgoraBase.h>
using namespace agora;
using namespace agora::rtc;

typedef QMap<QString,QString> qSSMap;

class CAgoraObject:public QObject
{
    Q_OBJECT
public:

    int joinChannel(const QString& key, const QString& channel, uint uid);
    int leaveChannel();
    int muteLocalAudioStream(bool muted);
    BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn, RENDER_MODE_TYPE renderType = RENDER_MODE_TYPE::RENDER_MODE_FIT);
    BOOL RemoteVideoRender(uid_t uid, HWND hVideoWnd, RENDER_MODE_TYPE renderType = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN);

    BOOL startLastMileProbeTest(bool enable);
    int enableVideo(bool enabled);
    int enableAudio(bool enabled);
    BOOL MuteLocalVideo(BOOL bMute);
    BOOL MuteLocalAudio(BOOL bMute);
    BOOL SetEncryptionMode(const char* secret,const char* secretMode);

    BOOL setLogPath(const QString &strDir);
    BOOL SetChannelProfile(CHANNEL_PROFILE_TYPE channelType);
    BOOL SetClientRole(CLIENT_ROLE_TYPE roleType);
    BOOL EnableWebSdkInteroperability(BOOL bEnable);

    qSSMap getRecordingDeviceList();
    qSSMap getPlayoutDeviceList();
    qSSMap getVideoDeviceList();

    int setRecordingDevice(const QString& guid);
    int setPlayoutDevice(const QString& guid);
    int setVideoDevice(const QString& guid);

    BOOL setVideoProfile(int nWidth,int nHeight);
    BOOL setRecordingIndex(int nIndex);
    BOOL setPlayoutIndex(int nIndex);
    BOOL setVideoIndex(int nIndex);
    BOOL SetLogFilter(LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath);

	bool setBeautyEffectOptions(bool enabled, BeautyOptions& options);
signals:
    void sender_videoStopped();
    void sender_joinedChannelSuccess(const QString &qsChannel, unsigned int uid, int elapsed);
    void sender_userJoined(unsigned int uid, int elapsed);
    void sender_userOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason);
    void sender_firstLocalVideoFrame(int width, int height, int elapsed);
    void sender_firstRemoteVideoDecoded(unsigned int uid, int width, int height, int elapsed);
    void sender_firstRemoteVideoFrameDrawn(unsigned int uid, int width, int height, int elapsed);
    void sender_localVideoStats(const LocalVideoStats &stats);
    void sender_remoteVideoStats(const RemoteVideoStats &stats);
    void sender_rtcStats(const RtcStats &stats);
    void sender_lastmileQuality(int quality);
    void sender_lastmileProbeResult(const LastmileProbeResult &result);

public:
    static CAgoraObject* getInstance(QObject *parent = 0);
    static CAgoraObject* m_pInstance;
    static std::mutex    m_mutex;

private:
    explicit CAgoraObject(QObject *parent = 0);
    ~CAgoraObject();

    agora::rtc::IRtcEngine* m_rtcEngine;
    std::unique_ptr<agora::rtc::IRtcEngineEventHandler> m_eventHandler;
};

#endif // CAGORAOBJECT_H
