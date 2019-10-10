#include "agoraobject.h"
#include "inroom.h"
#include "ui_inroom.h"
#include "agorawindowmanager.h"

#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

InRoom::InRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InRoom)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint
                         | Qt::WindowMinMaxButtonsHint );

    CAgoraObject* pObject = CAgoraObject::getInstance();
    AgoraWindowManager::getInstance()->insertItem(qWndType_InRoom,this);

    connect(pObject,SIGNAL(sender_videoStopped()),
            this,SLOT(receive_videoStopped()));
    connect(pObject,SIGNAL(sender_joinedChannelSuccess(const QString&,unsigned int ,int)),
            this,SLOT(receive_joinedChannelSuccess(const QString&,unsigned int,int)));
    connect(pObject,SIGNAL(sender_userJoined(unsigned int,int)),
            this,SLOT(receive_userJoined(unsigned int,int)));
    connect(pObject,SIGNAL(sender_userOffline(unsigned int,USER_OFFLINE_REASON_TYPE)),
            this,SLOT(receive_userOffline(unsigned int,USER_OFFLINE_REASON_TYPE)));
    connect(pObject,SIGNAL(sender_firstLocalVideoFrame(int,int,int)),
            this,SLOT(receive_firstLocalVideoFrame(int,int,int)));
    connect(pObject,SIGNAL(sender_firstRemoteVideoDecoded(unsigned int,int,int,int)),
            this,SLOT(receive_firstRemoteVideoDecoded(unsigned int,int,int,int)));
    connect(pObject,SIGNAL(sender_firstRemoteVideoFrameDrawn(unsigned int,int,int,int)),
            this,SLOT(receive_firstRemoteVideoFrameDrawn(unsigned int,int,int,int)));
    connect(pObject,SIGNAL(sender_localVideoStats(LocalVideoStats)),
            this,SLOT(receive_localVideoStats(LocalVideoStats)));
    connect(pObject,SIGNAL(sender_remoteVideoStats(RemoteVideoStats)),
            this,SLOT(receive_remoteVideoStats(RemoteVideoStats)));
    connect(pObject,SIGNAL(sender_rtcStats(RtcStats)),
            this,SLOT(receive_rtcStats(RtcStats)));

    ui->widget_l->lower();
    ui->widget_r1->raise();
    ui->widget_r2->raise();
    ui->widget_r3->raise();
    ui->widget_r1->setWindowOpacity(1);//opaque.
    ui->widget_r2->setWindowOpacity(1);
    ui->widget_r3->setWindowOpacity(1);

    ui->widget_l->installEventFilter(this);
    ui->widget_r1->installEventFilter(this);
    ui->widget_r2->installEventFilter(this);
    ui->widget_r3->installEventFilter(this);

    m_timer_fps = new QTimer(this);
    connect(m_timer_fps,SIGNAL(timeout()),this,SLOT(receive_timer_pfs()));
}

InRoom::~InRoom()
{
    delete ui;
}

void InRoom::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        m_mousePosition = e->pos();
        if(m_mousePosition.x() < lnTitleWidth &&
           m_mousePosition.y() < lnTitleHeight)
            m_bMousePressed = true;
    }

    QApplication::sendEvent(m_uper.get(),e);
}

void InRoom::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bMousePressed == true) {
        QPoint movePos = e->globalPos() - m_mousePosition;
        move(movePos);
        e->accept();
    }


    QApplication::sendEvent(m_uper.get(),e);
}

void InRoom::mouseReleaseEvent(QMouseEvent *e)
{
   m_bMousePressed = false;

   QApplication::sendEvent(m_uper.get(),e);
}

void InRoom::focusInEvent(QFocusEvent *event)
{
    QApplication::sendEvent(m_uper.get(),event);
}

void InRoom::focusOutEvent(QFocusEvent *event)
{
    QApplication::sendEvent(m_uper.get(),event);
}

void InRoom::joinchannel(const QString& qsChannelId,const QString& qsChannelKey,uint uid)
{
   this->show();

   CAgoraObject::getInstance()->joinChannel((qsChannelKey.length() > 0 ? qsChannelKey:NULL),qsChannelId,uid);

   m_uper.reset(new EnterRoom);
   m_uper->joinchannel(qsChannelId,uid);
}

void InRoom::leavechannel()
{
     m_timer_fps->stop();
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        m_qivs.clear();
        delete m_timer_fps;
        m_bigUid = 0;
        m_uid = 0;
    }

    CAgoraObject::getInstance()->leaveChannel();
    this->hide();

    QMainWindow* pLastWnd = AgoraWindowManager::getInstance()->getQWnd(qWndType_OpenVideoCall);
    pLastWnd->show();
}

void InRoom::receive_videoStopped()
{
}

void InRoom::receive_joinedChannelSuccess(const QString &qsChannel, unsigned int uid, int elapsed)
{
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        m_uid = uid;
        if(!m_qivs.contains(uid)) {
            m_qivs[uid] = Video_Stats(uid);
        }
    }

    adjustPos();

    QString qsChannelInfo;
    m_qsChannel = qsChannel;
    qsChannelInfo.sprintf("%s %u",qsChannel.toStdString().c_str(),uid);
    m_uper->setChannelName(qsChannelInfo);
    m_bigUid = uid;
    m_timer_fps->start(2000);
}

void InRoom::receive_userJoined(uid_t uid, int elapsed)
{
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        if(!m_qivs.contains(uid)) {
            m_qivs[uid] = Video_Stats(uid);
        }
    }

    adjustPos();
}

void InRoom::receive_userOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        if(m_qivs.contains(uid)) {
            auto it = m_qivs.find(uid);
            m_qivs.erase(it);
        }
    }

    adjustPos();
}

void InRoom::receive_firstLocalVideoFrame(int width, int height, int elapsed)
{

}

void InRoom::receive_firstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{

}

void InRoom::receive_firstRemoteVideoFrameDrawn(uid_t uid, int width, int height, int elapsed)
{

}

void InRoom::receive_localVideoStats(const LocalVideoStats &stats)
{
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        if(m_qivs.contains(m_uid)) {
            m_qivs[m_uid].nWidth = stats.encodedFrameWidth;
            m_qivs[m_uid].nHeight = stats.encodedFrameHeight;
            m_qivs[m_uid].nFps = stats.sentFrameRate;
            m_qivs[m_uid].nBitrate = stats.sentBitrate;
        }
    }
}

void InRoom::receive_remoteVideoStats(const RemoteVideoStats &stats)
{
   {
       std::lock_guard<std::mutex> autoLock(m_vsMutex);
       if(m_qivs.contains(stats.uid)) {
           m_qivs[stats.uid].nLastmileDelay = stats.delay;
           m_qivs[stats.uid].nWidth = stats.width;
           m_qivs[stats.uid].nHeight = stats.height;
           m_qivs[stats.uid].nFps = stats.rendererOutputFrameRate;
           m_qivs[stats.uid].nBitrate = stats.receivedBitrate;
       }
   }
}

void InRoom::receive_rtcStats(const RtcStats &stats)
{
    m_nlastmileDelay = stats.lastmileDelay;

    std::lock_guard<std::mutex> autoLock(m_vsMutex);
    if(m_qivs.contains(m_uid))
        m_qivs[m_uid].nLastmileDelay = stats.lastmileDelay;
}

void InRoom::receive_timer_pfs()
{
    std::lock_guard<std::mutex> autoLock(m_vsMutex);
    if(m_qivs.contains(m_bigUid)) {
        QString qsFps;
        auto it = m_qivs.value(m_bigUid);
        qsFps.sprintf("SD-RTN: %dms.  Video: %dfps %dx%d",it.nLastmileDelay,it.nFps,it.nWidth,it.nHeight);
        m_uper->setParam(qsFps);
    }
}

void InRoom::adjustPos()
{
    ui->widget_l->setVisible(false);
    ui->widget_r1->setVisible(false);
    ui->widget_r2->setVisible(false);
    ui->widget_r3->setVisible(false);

    std::lock_guard<std::mutex> autoLock(m_vsMutex);
    auto it = m_qivs.begin();
    int nIndex = 0;
    while(m_qivs.end() != it) {
        if(it->uid == m_uid) {
            m_bigUid == m_uid;
            it->pWidget = ui->widget_l;
            it->nIndex = 0;
            CAgoraObject::getInstance()->LocalVideoPreview((HWND)(it->pWidget->winId()),
                                                           TRUE,
                                                           (it->uid == m_bigUid) ? RENDER_MODE_FIT : RENDER_MODE_HIDDEN);
            ui->widget_l->setVisible(true);
            ui->widget_l->setGeometry(0,0,1366,768);
            ui->widget_l->lower();
            QString qsChannelInfo;
            qsChannelInfo.sprintf("%s %u",m_qsChannel.toStdString().c_str(),m_uid);
            m_uper->setChannelName(qsChannelInfo);
        }
        else {
            nIndex++;
            if(nIndex == 1) {
                it->pWidget = ui->widget_r1;
                it->nIndex = 1;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
                m_uper->setR1(it->uid);
                ui->widget_r1->setVisible(true);
                ui->widget_r1->setGeometry(30,94,200,150);
                ui->widget_r1->raise();
            }

            if(nIndex == 2) {
                it->pWidget = ui->widget_r2;
                it->nIndex = 2;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
                m_uper->setR2(it->uid);
                ui->widget_r2->setVisible(true);
                ui->widget_r2->setGeometry(30,254,200,150);
                ui->widget_r2->raise();
            }

            if(nIndex == 3) {
                it->pWidget = ui->widget_r3;
                it->nIndex = 3;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
                m_uper->setR3(it->uid);
                ui->widget_r3->setVisible(true);
                ui->widget_r3->setGeometry(30,414,200,150);
                ui->widget_r3->raise();
            }
        }
        it++;
    }

    m_uper->setAllRAtt(nIndex);
}

bool InRoom::eventFilter(QObject *watched,QEvent *event)
{
    bool bFind = false;
    int nIndex = 0;
    Video_Stats vsr,vsbig;
    if(watched==ui->widget_r1 && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick r1");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uper->getR1();
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
            bFind = ( vsr.uid == m_bigUid ) ? false : true;
            nIndex = vsr.nIndex;
        }
    }
    else if(watched == ui->widget_r2 && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick r2");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uper->getR2();
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
			bFind = (vsr.uid == m_bigUid) ? false : true;
            nIndex = vsr.nIndex;
        }
    }
    else if(watched == ui->widget_r3 && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick r3");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uper->getR3();
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
			bFind = (vsr.uid == m_bigUid) ? false : true;
            nIndex = vsr.nIndex;
        }
    }

    else if(watched == ui->widget_l && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick local");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uid;
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
			bFind = (vsr.uid == m_bigUid) ? false : true;
            nIndex = vsr.nIndex;
        }
    }

    if(bFind) {
        std::lock_guard<std::mutex> autolock(m_vsMutex);
        QRect qrr,qrbig;
        if(vsr.pWidget)
            qrr = vsr.pWidget->geometry();
        if(vsbig.pWidget)
            qrbig = vsbig.pWidget->geometry();

        vsr.pWidget->setGeometry(qrbig);
        vsbig.pWidget->setGeometry(qrr);
        vsr.pWidget->lower();
        vsbig.pWidget->raise();
        m_qivs[vsr.uid].nIndex = 0;
        m_qivs[vsbig.uid].nIndex = nIndex;

        QString qsSrc,qsDest;
        qsSrc.sprintf("%s %u",m_qsChannel.toStdString().c_str(),vsr.uid);
        qsDest.sprintf("  uid:%u",vsbig.uid);
        m_uper->switchUidText(qsSrc,qsDest,nIndex);
		
		m_bigUid = vsr.uid;
    }

    return QWidget::eventFilter(watched,event);
}
