#ifndef INROOM_H
#define INROOM_H

#include <Memory>
#include <QVariant>
#include <QMainWindow>
#include "enterroom.h"
#include <mutex>
#include <qtimer.h>

#include <IAgoraRtcEngine.h>
using namespace agora::rtc;

namespace Ui {
class InRoom;
}

typedef struct eTagVideoStats {
    eTagVideoStats(){;}
    eTagVideoStats(unsigned int ui,int nlastmiledelay = 0,int nwidth = 0,int nheight = 0,int nfps = 0,int nbitrate = 0,QWidget *pwidget = Q_NULLPTR,int nindex = 0):
        uid(ui),nLastmileDelay(nlastmiledelay),nWidth(nwidth),nHeight(nheight),nFps(nfps),nBitrate(nbitrate),pWidget(pwidget),nIndex(nindex){;}
    eTagVideoStats(const eTagVideoStats& vss) {
        *this = vss;
    }
    eTagVideoStats& operator=(const eTagVideoStats& vss) {
        if(this != &vss) {
            uid = vss.uid;
            nLastmileDelay = vss.nLastmileDelay;
            nWidth = vss.nWidth;
            nHeight = vss.nHeight;
            nFps = vss.nFps;
            nBitrate = vss.nBitrate;
            pWidget = vss.pWidget;
            nIndex = vss.nIndex;
        }

        return *this;
    }

    unsigned int uid;
    int nLastmileDelay;
    int nWidth;
    int nHeight;
    int nFps;
    int nBitrate;
    QWidget* pWidget;
    int nIndex;
}Video_Stats,*PVideo_Stats,*LPVideoStats;
typedef QMap<unsigned int,Video_Stats> qIVS;

class InRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit InRoom(QWidget *parent = 0);
    ~InRoom();

private:
    Ui::InRoom *ui;

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

public slots:
    void joinchannel(const QString& qsChannelId,const QString& qsChannelKey,uint uid);
    void leavechannel();

    void receive_videoStopped();
    void receive_joinedChannelSuccess(const QString &qsChannel, unsigned int uid, int elapsed);
    void receive_userJoined(unsigned int uid, int elapsed);
    void receive_userOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason);
    void receive_firstLocalVideoFrame(int width, int height, int elapsed);
    void receive_firstRemoteVideoDecoded(unsigned int uid, int width, int height, int elapsed);
    void receive_firstRemoteVideoFrameDrawn(unsigned int uid, int width, int height, int elapsed);
    void receive_localVideoStats(const LocalVideoStats &stats);
    void receive_remoteVideoStats(const RemoteVideoStats &stats);
    void receive_rtcStats(const RtcStats &stats);

    void receive_timer_pfs();

    bool eventFilter(QObject *watched,QEvent *event);

private slots:

private:
     void adjustPos();

private:
    const int lnTitleWidth = 1366;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;
    std::unique_ptr<EnterRoom> m_uper;
    std::mutex  m_vsMutex;
    qIVS    m_qivs;
    int     m_nlastmileDelay;
    QTimer  *m_timer_fps;
    uid_t   m_uid;
    uid_t   m_bigUid;
    QString m_qsChannel;
};

#endif // INROOM_H
