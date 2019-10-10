#include "agoraobject.h"
#include <qmessagebox.h>
#include <cassert>

class AgoraRtcEngineEvent : public agora::rtc::IRtcEngineEventHandler
{
    CAgoraObject& m_pInstance;
public:
    AgoraRtcEngineEvent(CAgoraObject& engine)
        :m_pInstance(engine)
    {}

    virtual void onVideoStopped() override
    {
        emit m_pInstance.sender_videoStopped();
    }
    virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override
    {
       emit m_pInstance.sender_joinedChannelSuccess(QString(channel),uid,elapsed);
    }
    virtual void onUserJoined(uid_t uid, int elapsed) override
    {
        //qDebug("%s %u",__FUNCTION__,uid);
        emit m_pInstance.sender_userJoined(uid, elapsed);
    }
    virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override
    {
        emit m_pInstance.sender_userOffline(uid, reason);
    }
    virtual void onFirstLocalVideoFrame(int width, int height, int elapsed) override
    {
        emit m_pInstance.sender_firstLocalVideoFrame(width, height, elapsed);
    }
    virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed) override
    {
        emit m_pInstance.sender_firstRemoteVideoDecoded(uid, width, height, elapsed);
    }
    virtual void onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed) override
    {
       emit m_pInstance.sender_firstRemoteVideoFrameDrawn(uid, width, height, elapsed);
    }
    virtual void onLocalVideoStats(const LocalVideoStats &stats) override
    {
        emit m_pInstance.sender_localVideoStats(stats);
    }
    virtual void onRemoteVideoStats(const RemoteVideoStats &stats) override
    {
        emit m_pInstance.sender_remoteVideoStats(stats);
    }
    virtual void onRtcStats(const RtcStats &stats) override
    {
        emit m_pInstance.sender_rtcStats(stats);
    }
    virtual void onLastmileQuality(int quality) override
    {
        emit m_pInstance.sender_lastmileQuality(quality);
    }
    virtual void onLastmileProbeResult(const LastmileProbeResult &result) override
    {
        emit m_pInstance.sender_lastmileProbeResult(result);
    }
};

CAgoraObject* CAgoraObject::getInstance(QObject *parent)
{
    std::lock_guard<std::mutex> autoLock(m_mutex);
    if(nullptr == m_pInstance)
        m_pInstance = new CAgoraObject(parent);

    return m_pInstance;
}

CAgoraObject* CAgoraObject::m_pInstance = nullptr;
std::mutex  CAgoraObject::m_mutex;

CAgoraObject::CAgoraObject(QObject *parent):
    QObject(parent),
    m_rtcEngine(createAgoraRtcEngine()),
    m_eventHandler(new AgoraRtcEngineEvent(*this))
{
    agora::rtc::RtcEngineContext context;
    context.eventHandler = m_eventHandler.get();
    context.appId = APPID;
    if (*context.appId == '\0')
    {
        QMessageBox::critical(nullptr, ("AgoraOpenLive"),
                                       ("You must specify APP ID before using the demo"));
    }
    m_rtcEngine->initialize(context);
}

CAgoraObject::~CAgoraObject()
{
    if(m_rtcEngine)
        m_rtcEngine->release();
}

int CAgoraObject::joinChannel(const QString& key, const QString& channel, uint uid)
{
    if (channel.isEmpty()) {
        QMessageBox::warning(nullptr,("AgoraHighSound"),("channelname is empty"));
        return -1;
    }

    m_rtcEngine->startPreview();
    int r = m_rtcEngine->joinChannel(key.toUtf8().data(), channel.toUtf8().data(), nullptr, uid);

    return r;
}

int CAgoraObject::leaveChannel()
{
    m_rtcEngine->stopPreview();
    int r = m_rtcEngine->leaveChannel();
    return r;
}

int CAgoraObject::muteLocalAudioStream(bool muted)
{
    RtcEngineParameters rep(*m_rtcEngine);
    return rep.muteLocalAudioStream(muted);
}

BOOL CAgoraObject::LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn, RENDER_MODE_TYPE renderType/* = RENDER_MODE_TYPE::RENDER_MODE_FIT*/)
{
    int nRet = 0;

    if (bPreviewOn) {
        VideoCanvas vc;

        vc.uid = 0;
        vc.view = hVideoWnd;
        vc.renderMode = renderType;

        m_rtcEngine->setupLocalVideo(vc);
        nRet = m_rtcEngine->startPreview();
    }
    else
        nRet = m_rtcEngine->stopPreview();

    return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::RemoteVideoRender(uid_t uid, HWND hVideoWnd, RENDER_MODE_TYPE renderType/* = RENDER_MODE_TYPE::RENDER_MODE_HIDDEN*/)
{
    int nRet = 0;

    VideoCanvas vc;

    vc.uid = uid;
    vc.view = hVideoWnd;
    vc.renderMode = renderType;

    m_rtcEngine->setupRemoteVideo(vc);

    return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::startLastMileProbeTest(bool enable)
{
    int nres = 0;
    if(enable) {
        LastmileProbeConfig lp;
        lp.probeUplink =true;
        lp.probeDownlink = true;
        lp.expectedDownlinkBitrate = 5000;
        lp.expectedUplinkBitrate = 5000;

        nres = m_rtcEngine->startLastmileProbeTest(lp);
    }
    else {
        nres = m_rtcEngine->stopLastmileProbeTest();
    }

    return nres == 0? TRUE : FALSE;
}

int CAgoraObject::enableVideo(bool enabled)
{
    return enabled ? m_rtcEngine->enableVideo() : m_rtcEngine->disableVideo();
}

int CAgoraObject::enableAudio(bool enabled)
{
    return enabled ? m_rtcEngine->enableAudio() : m_rtcEngine->disableAudio();
}

BOOL CAgoraObject::setLogPath(const QString &strDir)
{
    int ret = 0;

    RtcEngineParameters rep(*m_rtcEngine);
    ret = rep.setLogFile(strDir.toUtf8().data());

    return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetChannelProfile(CHANNEL_PROFILE_TYPE channelType)
{
    int ret = 0;
    ret = m_rtcEngine->setChannelProfile(channelType);

    return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetClientRole(CLIENT_ROLE_TYPE roleType)
{
    int ret = 0;

    ret = m_rtcEngine->setClientRole(roleType);

    return ret == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::EnableWebSdkInteroperability(BOOL bEnable)
{
    RtcEngineParameters rep(*m_rtcEngine);

    int	nRet = rep.enableWebSdkInteroperability(static_cast<bool>(bEnable));

    return nRet == 0 ? TRUE : FALSE;
}

qSSMap CAgoraObject::getRecordingDeviceList()
{
    qSSMap devices;
    AAudioDeviceManager audioDeviceManager(m_rtcEngine);
    if (!audioDeviceManager)
        return devices;

    agora::util::AutoPtr<IAudioDeviceCollection> spCollection(audioDeviceManager->enumerateRecordingDevices());
    if (!spCollection)
        return devices;
    char name[MAX_DEVICE_ID_LENGTH], guid[MAX_DEVICE_ID_LENGTH];
    int count = spCollection->getCount();
    if (count > 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (!spCollection->getDevice(i, name, guid))
            {
                devices[name] = guid;
            }
        }
    }
    return devices;
}

qSSMap CAgoraObject::getPlayoutDeviceList()
{
    qSSMap devices;
    AAudioDeviceManager audioDeviceManager(m_rtcEngine);
    if (!audioDeviceManager)
        return devices;

    agora::util::AutoPtr<IAudioDeviceCollection> spCollection(audioDeviceManager->enumeratePlaybackDevices());
    if (!spCollection)
        return devices;
    char name[MAX_DEVICE_ID_LENGTH], guid[MAX_DEVICE_ID_LENGTH];
    int count = spCollection->getCount();
    if (count > 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (!spCollection->getDevice(i, name, guid))
            {
                devices[name] = guid;
            }
        }
    }
    return devices;
}

qSSMap CAgoraObject::getVideoDeviceList()
{
	m_rtcEngine->enableVideo();
    qSSMap devices;
    AVideoDeviceManager videoDeviceManager(m_rtcEngine);
    if (!videoDeviceManager)
        return devices;

    agora::util::AutoPtr<IVideoDeviceCollection> spCollection(videoDeviceManager->enumerateVideoDevices());
    if (!spCollection)
        return devices;
    char name[MAX_DEVICE_ID_LENGTH], guid[MAX_DEVICE_ID_LENGTH];
    int count = spCollection->getCount();
    if (count > 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (!spCollection->getDevice(i, name, guid))
            {
                devices[name] = guid;
            }
        }
    }
    return devices;
}

int CAgoraObject::setRecordingDevice(const QString& guid)
{
    if (guid.isEmpty())
        return -1;
    AAudioDeviceManager audioDeviceManager(m_rtcEngine);
    if (!audioDeviceManager)
        return -1;
    return audioDeviceManager->setRecordingDevice(guid.toUtf8().data());
}

int CAgoraObject::setPlayoutDevice(const QString& guid)
{
    if (guid.isEmpty())
        return -1;
    AAudioDeviceManager audioDeviceManager(m_rtcEngine);
    if (!audioDeviceManager)
        return -1;
    return audioDeviceManager->setPlaybackDevice(guid.toUtf8().data());
}

int CAgoraObject::setVideoDevice(const QString& guid)
{
    if (guid.isEmpty())
        return -1;
    AVideoDeviceManager videoDeviceManager(m_rtcEngine);
    if (!videoDeviceManager)
        return -1;
    return videoDeviceManager->setDevice(guid.toUtf8().data());
}

BOOL CAgoraObject::setVideoProfile(int nWidth,int nHeight)
{
    int res = 0;
    VideoEncoderConfiguration vec;
    vec = VideoEncoderConfiguration(nWidth,nHeight,FRAME_RATE_FPS_15,500,ORIENTATION_MODE_FIXED_LANDSCAPE);
    res = m_rtcEngine->setVideoEncoderConfiguration(vec);

    return res ==0 ? TRUE : FALSE;
}

BOOL CAgoraObject::setRecordingIndex(int nIndex)
{
    int res = 0;
    AAudioDeviceManager audioDeviceManager(m_rtcEngine);
    if (!audioDeviceManager)
        return FALSE;

    agora::util::AutoPtr<IAudioDeviceCollection> spCollection(audioDeviceManager->enumerateRecordingDevices());
    if (!spCollection)
        return FALSE;
    char name[MAX_DEVICE_ID_LENGTH], guid[MAX_DEVICE_ID_LENGTH];
    int count = spCollection->getCount();
    assert(res < count);
    spCollection->getDevice(nIndex,name,guid);
    res = spCollection->setDevice(guid);

    return res ==0 ? TRUE:FALSE;
}

BOOL CAgoraObject::setPlayoutIndex(int nIndex)
{
    int res = 0;
    AAudioDeviceManager audioDeviceManager(m_rtcEngine);
    if (!audioDeviceManager)
        return FALSE;

    agora::util::AutoPtr<IAudioDeviceCollection> spCollection(audioDeviceManager->enumeratePlaybackDevices());
    if (!spCollection)
        return FALSE;
    char name[MAX_DEVICE_ID_LENGTH], guid[MAX_DEVICE_ID_LENGTH];
    int count = spCollection->getCount();
    assert(res < count);
    spCollection->getDevice(nIndex,name,guid);
    res = spCollection->setDevice(guid);

    return res ==0 ? TRUE:FALSE;
}

BOOL CAgoraObject::setVideoIndex(int nIndex)
{

    int res = 0;
    AVideoDeviceManager videoDeviceManager(m_rtcEngine);
    if (!videoDeviceManager)
        return FALSE;

    agora::util::AutoPtr<IVideoDeviceCollection> spCollection(videoDeviceManager->enumerateVideoDevices());
    if (!spCollection)
        return FALSE;
    char name[MAX_DEVICE_ID_LENGTH], guid[MAX_DEVICE_ID_LENGTH];
    int count = spCollection->getCount();
    assert(nIndex < count);
    spCollection->getDevice(nIndex,name,guid);
    res = spCollection->setDevice(guid);

    return res == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::MuteLocalVideo(BOOL bMute)
{
     int nRet = 0;

    RtcEngineParameters rep(*m_rtcEngine);
    nRet = rep.enableLocalVideo(!bMute);

    return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::MuteLocalAudio(BOOL bMute)
{
    int nRet = 0;

    RtcEngineParameters rep(*m_rtcEngine);
    nRet = m_rtcEngine->enableLocalAudio(!bMute);

    return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraObject::SetEncryptionMode(const char* secret,const char* secretMode)
{
    int nRet = 0;
    if(secret != NULL && strlen(secret) > 0 && secretMode != NULL) {
        nRet = m_rtcEngine->setEncryptionSecret(secret);
        nRet = m_rtcEngine->setEncryptionMode(secretMode);
    }

    return nRet == 0 ? TRUE : FALSE;
}
