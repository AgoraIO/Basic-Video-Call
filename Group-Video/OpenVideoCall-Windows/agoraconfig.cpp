#include "agoraconfig.h"

CAgoraConfig::CAgoraConfig()
{
   m_spConfig = std::make_shared<QSettings>("AgoraConfigOpenVideoCall.ini",QSettings::IniFormat);
}

void CAgoraConfig::setAppId(const QString &str)
{
    return m_spConfig->setValue("/BaseInfo/AppId",str);
}

QString CAgoraConfig::getAppId()
{
    return m_spConfig->value("/BaseInfo/AppId").toString();
}

QString CAgoraConfig::getAppToken()
{
    return m_spConfig->value("/BaseInfo/AppToken").toString();
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


void CAgoraConfig::setEnableBeauty(bool bEnable)
{
	return m_spConfig->setValue("/BaseInfo/EnableBeauty", bEnable);
}

bool CAgoraConfig::getEnableBeauty()
{
	return m_spConfig->value("/BaseInfo/EnableBeauty").toBool();
}

void CAgoraConfig::setLigtheningContrastLevel(int level)
{
	return m_spConfig->setValue("/Beauty/LightengingContrastLevel", level);
}

int CAgoraConfig::getLigtheningContrastLevel()
{
	return m_spConfig->value("/Beauty/LightengingContrastLevel").toInt();
}

void CAgoraConfig::setRedness(int level)
{
	return m_spConfig->setValue("/Beauty/Redness", level);
}

int CAgoraConfig::getRedness()
{
	return m_spConfig->value("/Beauty/Redness").toInt();
}

void CAgoraConfig::setLightenging(int lightening)
{
	return m_spConfig->setValue("/Beauty/Lightening", lightening);
}

int CAgoraConfig::getLightenging()
{
	return m_spConfig->value("/Beauty/Lightening").toInt();
}

void CAgoraConfig::setSmoothness(int smooth)
{
	return m_spConfig->setValue("/Beauty/Smoothness", smooth);
}

int CAgoraConfig::getSmoothness()
{
	return m_spConfig->value("/Beauty/Smoothness").toInt();
}

void CAgoraConfig::getVideoResolution(int& width, int height)
{
    width  = m_spConfig->value("/VideoInfo/Width").toInt();
    height = m_spConfig->value("/VideoInfo/Height").toInt();
}

int CAgoraConfig::getFPS()
{
    return m_spConfig->value("/VideoInfo/FPS").toInt();
}

int CAgoraConfig::getBitrate()
{
    return m_spConfig->value("/VideoInfo/Bitrate").toInt();
}
bool CAgoraConfig::isCustomFPS()
{
    return m_spConfig->value("/VideoInfo/CustomFPS").toBool();
}
bool CAgoraConfig::isCustomBitrate()
{
    return m_spConfig->value("/VideoInfo/CustomBitrate").toBool();
}
bool CAgoraConfig::isCustomResolution()
{
    return m_spConfig->value("/VideoInfo/CustomResolution").toBool();
}
