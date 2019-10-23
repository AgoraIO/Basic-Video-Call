#include "stdafx.h"
#include "agoraconfig.h"

CAgoraConfig::CAgoraConfig()
{
   m_spConfig = std::make_shared<QSettings>("AgoraConfigOpenLive.ini",QSettings::IniFormat);
}

void CAgoraConfig::setAppId(const QString &str)
{
    return m_spConfig->setValue("/BaseInfo/AppId",str);
}

QString CAgoraConfig::getAppId()
{
    return m_spConfig->value("/BaseInfo/AppId").toString();
}

void CAgoraConfig::setChannelName(const QString &str)
{
    return m_spConfig->setValue("/BaseInfo/CameraName",str);
}

QString CAgoraConfig::getChannelName()
{
    return m_spConfig->value("/BaseInfo/ChannelName").toString();
}

void CAgoraConfig::setEnableVideo(bool bEnable)
{
    return m_spConfig->setValue("/BaseInfo/EnableVideo",bEnable);
}

bool CAgoraConfig::getEnableVideo()
{
    return m_spConfig->value("/BaseInfo/EnableVideo").toBool();
}

void CAgoraConfig::setEnableAudio(bool bEnable)
{
    return m_spConfig->setValue("/BaseInfo/EnableAudio",bEnable);
}

bool CAgoraConfig::getEnableAudio()
{
    return m_spConfig->value("/BaseInfo/EnableAudio").toBool();
}
