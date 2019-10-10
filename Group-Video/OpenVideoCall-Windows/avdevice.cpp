#include "avdevice.h"
#include "ui_avdevice.h"
#include "agoraobject.h"
#include "agorawindowmanager.h"

AVDevice::AVDevice(QMainWindow* pLastWnd,const QString &qsChanne,QWidget *parent) :
    QMainWindow(parent),
    m_lastWnd(pLastWnd),
    ui(new Ui::AVDevice),
    m_bEnableAudio(true),
    m_bEnableVideo(true),
    m_strChannel(qsChanne)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    initCtrl();
    AgoraWindowManager::getInstance()->insertItem(qWndType_AvDevice,this);
}

AVDevice::~AVDevice()
{
    delete ui;
}

void AVDevice::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        m_mousePosition = e->pos();
        if(m_mousePosition.x() < lnTitleWidth &&
           m_mousePosition.x() >= lnGapWidth &&
           m_mousePosition.y() >= lnGapHeight &&
           m_mousePosition.y() < lnTitleHeight)
            m_bMousePressed = true;
    }
}

void AVDevice::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bMousePressed == true) {
        QPoint movePos = e->globalPos() - m_mousePosition;
        move(movePos);
        e->accept();
    }
}

void AVDevice::mouseReleaseEvent(QMouseEvent *e)
{
   m_bMousePressed = false;
}

void AVDevice::initCtrl()
{
    ui->lb_channel->setText(m_strChannel);
    setStyleSheet_audio(m_bEnableAudio);
    setStyleSheet_video(m_bEnableVideo);

    //videoProfile
    ui->cb_solution->clear();
    ui->cb_solution->addItem("160x120");
    ui->cb_solution->addItem("320x180");
    ui->cb_solution->addItem("320x240");
    ui->cb_solution->addItem("640x360");
    ui->cb_solution->addItem("640x480");
    ui->cb_solution->addItem("1280x720");
    ui->cb_solution->addItem("1920x1080");
    ui->cb_solution->addItem("3840x2160");
    ui->cb_solution->setCurrentIndex(3);

    //microphone.
    ui->cb_audio->clear();
    QString qDeviceName;
    qSSMap devicelist = CAgoraObject::getInstance()->getRecordingDeviceList();
    for (qSSMap::iterator it = devicelist.begin(); devicelist.end() != it; it++) {
        ui->cb_audio->addItem(it.key());
    }

    //playout.
    ui->cb_speaker->clear();
    devicelist.clear();
    devicelist = CAgoraObject::getInstance()->getPlayoutDeviceList();
    for(qSSMap::iterator it = devicelist.begin(); devicelist.end() != it; it++) {
        ui->cb_speaker->addItem(it.key());
    }

    //cameralist
    ui->cb_video->clear();
    devicelist.clear();
    devicelist = CAgoraObject::getInstance()->getVideoDeviceList();
    for(qSSMap::iterator it = devicelist.begin(); devicelist.end() != it; it++) {
        ui->cb_video->addItem(it.key());
    }
}


void AVDevice::setStyleSheet_audio(bool bEnable)
{
    QString qsStyle ;
    if(bEnable) {
         qsStyle = "QPushButton:!pressed {\
                width:50px;\
                height:30px;\
                border-radius:19px;\
                image: url(:/uiresource/switch-open.png)\
                }";
    }
    else {
        qsStyle = "QPushButton:!pressed {\
                width:50px;\
                height:30px;\
                border-radius:19px;\
                image: url(:/uiresource/switch-off.png)\
                }";
    }

    ui->btn_audio->setStyleSheet(qsStyle);
}

void AVDevice::setStyleSheet_video(bool bEnable)
{
    QString qsStyle;
    if(bEnable) {
        qsStyle = "QPushButton:!pressed {\
               width:50px;\
               height:30px;\
               border-radius:19px;\
               image: url(:/uiresource/switch-open.png)\
               }";
    }
    else {
        qsStyle = "QPushButton:!pressed {\
                width:50px;\
                height:30px;\
                border-radius:19px;\
                image: url(:/uiresource/switch-off.png)\
                }";
    }
    ui->btn_video->setStyleSheet(qsStyle);
}

void AVDevice::on_btn_back_clicked()
{
    if(m_lastWnd)
            m_lastWnd->show();
    this->hide();
}

void AVDevice::on_cb_solution_activated(int index)
{
}

void AVDevice::on_cb_audio_activated(int index)
{
    CAgoraObject::getInstance()->setRecordingIndex(index);
}

void AVDevice::on_cb_video_activated(int index)
{
    CAgoraObject::getInstance()->setVideoIndex(index);
}

void AVDevice::on_cb_speaker_activated(int index)
{
    CAgoraObject::getInstance()->setPlayoutIndex(index);
}

void AVDevice::on_btn_audio_clicked()
{
    m_bEnableAudio = !m_bEnableAudio;
    setStyleSheet_audio(m_bEnableAudio);
}

void AVDevice::on_btn_video_clicked()
{
    m_bEnableVideo = !m_bEnableVideo;
    setStyleSheet_video(m_bEnableVideo);
}

void AVDevice::on_btn_close_clicked()
{
    on_btn_back_clicked();
}

void AVDevice::on_btn_min_clicked()
{
    this->showMinimized();
}

void AVDevice::on_cb_solution_activated(const QString &arg1)
{
    int nWidth,nHeight = 0;
    sscanf(arg1.toUtf8().data(),"%dx%d", &nWidth, &nHeight);
    CAgoraObject::getInstance()->setVideoProfile(nWidth, nHeight);
}
