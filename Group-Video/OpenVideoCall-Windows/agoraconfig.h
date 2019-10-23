#ifndef CAGORACONFIG_H
#define CAGORACONFIG_H

#include <QSettings>

class CAgoraConfig
{
public:
    CAgoraConfig();

    void setAppId(const QString &str);
    QString getAppId();
    void setChannelName(const QString &str);
    QString getChannelName();
    void setEnableVideo(bool bEnable);
    bool getEnableVideo();
    void setEnableAudio(bool bEnable);
    bool getEnableAudio();

private:
    std::shared_ptr<QSettings>  m_spConfig;
};

#endif // CAGORACONFIG_H
