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