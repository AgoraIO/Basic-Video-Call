#include "avdevice.h"
#include "ui_avdevice.h"
#include "agoraobject.h"
#include "agorawindowmanager.h"
#include <QtWidgets/QScrollbar>
AVDevice::AVDevice(QMainWindow* pLastWnd,const QString &qsChanne,QWidget *parent) :
    QMainWindow(parent),
    m_lastWnd(pLastWnd),
    ui(new Ui::AVDevice),
    m_bEnableAudio(true),
    m_bEnableVideo(true),
	m_bEnableBeauty(false),
    m_strChannel(qsChanne)
{
    ui->setupUi(this);
	ui->scrollArea->verticalScrollBar()->setStyleSheet(QLatin1String(""
		"QScrollBar:vertical {border: none;background-color: rgb(255,255,255);width: 10px;margin: 0px 0 0px 0;}"
		" QScrollBar::handle:vertical { background:  rgba(240, 240, 240, 255); min-height: 20px;width: 6px;border: 1px solid  rgba(240, 240, 240, 255);border-radius: 3px;margin-left:2px;margin-right:2px;}"
		" QScrollBar::add-line:vertical {background-color: rgb(255,255,255);height: 4px;}"
		" QScrollBar::sub-line:vertical {background-color: rgb(255,255,255);height: 4px;}"
		" QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {height: 0px;}"
		" QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}"));

	m_bEnableBeauty = gAgoraConfig.getEnableBeauty();
	m_bEnableAudio  = gAgoraConfig.getEnableAudio();
	m_bEnableVideo  = gAgoraConfig.getEnableVideo();
	connect(ui->horizontalSlider_Redness, &QSlider::valueChanged, this, &AVDevice::on_valueChanged_horizontalSlider_Redness);
	connect(ui->horizontalSlider_Smoothness, &QSlider::valueChanged, this, &AVDevice::on_valueChanged_horizontalSlider_Smoothness);
	connect(ui->horizontalSliderLightening, &QSlider::valueChanged, this, &AVDevice::on_valueChanged_horizontalSlider_Lightening);

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

    //log filter
    ui->cb_log->clear();
    ui->cb_log->addItem("OFF");
    ui->cb_log->addItem("DEBUG");
    ui->cb_log->addItem("INFO");
    ui->cb_log->addItem("WARN");
    ui->cb_log->addItem("ERROR");
    ui->cb_log->addItem("CRITICAL");
    ui->cb_log->addItem("MASK");
    ui->cb_log->setCurrentIndex(2);
   
	m_bEnableBeauty = gAgoraConfig.getEnableBeauty();

	if (!m_bEnableBeauty) {
        QString str = "QPushButton:!hover {\
                border-image: url(:/uiresource/switch-off.png);\
                background-image: url(:/uiresource/switch-off.png);\
                }\
                \
                QPushButton:hover {\
                border-image: url(:/uiresource/switch-off.png);\
                background-image: url(:/uiresource/switch-off.png);\
                }";

        ui->optVideo_Beauty->setStyleSheet(str);
    }
    else {
        QString str = "QPushButton:!hover {\
                border-image: url(:/uiresource/switch-open.png);\
                background-image: url(:/uiresource/switch-open.png);\
                }\
                \
                QPushButton:hover {\
                border-image: url(:/uiresource/switch-open.png);\
                background-image: url(:/uiresource/switch-open.png);\
                }";

		ui->optVideo_Beauty->setStyleSheet(str);
    }
	//beauty
	ui->cbContrastLevel->clear();
	ui->cbContrastLevel->addItem(QString("Lightening Contrast Low"));
	ui->cbContrastLevel->addItem(QString("Lightening Contrast Normal"));
	ui->cbContrastLevel->addItem(QString("Lightening Contrast High"));

	ui->cbContrastLevel->setCurrentIndex(gAgoraConfig.getLigtheningContrastLevel());

	ui->horizontalSlider_Redness->setValue(gAgoraConfig.getRedness());
	ui->horizontalSliderLightening->setValue(gAgoraConfig.getLightenging());
	ui->horizontalSlider_Smoothness->setValue(gAgoraConfig.getSmoothness());
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
                }" \
				"QPushButton:pressed {\
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
                }"\
				"QPushButton:pressed {\
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
               }"\
			   "QPushButton:pressed {\
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
                }"\
				"QPushButton:pressed {\
                width:50px;\
                height:30px;\
                border-radius:19px;\
                image: url(:/uiresource/switch-off.png)\
                }";
    }
    ui->btn_video->setStyleSheet(qsStyle);
}


void AVDevice::enableVideoBeutyControl(bool bEnable)
{
	ui->cbContrastLevel->setDisabled(bEnable);
	ui->horizontalSliderLightening->setDisabled(bEnable);
	ui->horizontalSlider_Redness->setDisabled(bEnable);
	ui->horizontalSlider_Smoothness->setDisabled(bEnable);
}

void AVDevice::on_optVideo_Beauty_clicked()
{
	m_bEnableBeauty = !m_bEnableBeauty;
	enableVideoBeutyControl(!m_bEnableBeauty);
	updateBeautyOptions();
	if (!m_bEnableBeauty) {
        QString str = "QPushButton:!hover {\
                border-image: url(:/uiresource/switch-off.png);\
                background-image: url(:/uiresource/switch-off.png);\
                }\
                \
                QPushButton:hover {\
                border-image: url(:/uiresource/switch-off.png);\
                background-image: url(:/uiresource/switch-off.png);\
                }";

        ui->optVideo_Beauty->setStyleSheet(str);
	
    }
    else {
        QString str = "QPushButton:!hover {\
                border-image: url(:/uiresource/switch-open.png);\
                background-image: url(:/uiresource/switch-open.png);\
                }\
                \
                QPushButton:hover {\
                border-image: url(:/uiresource/switch-open.png);\
                background-image: url(:/uiresource/switch-open.png);\
                }";

		ui->optVideo_Beauty->setStyleSheet(str);
    }

	gAgoraConfig.setEnableBeauty(m_bEnableBeauty);
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
	gAgoraConfig.setEnableAudio(m_bEnableAudio);
}

void AVDevice::on_btn_video_clicked()
{
    m_bEnableVideo = !m_bEnableVideo;
    setStyleSheet_video(m_bEnableVideo);
	gAgoraConfig.setEnableVideo(m_bEnableVideo);
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

void AVDevice::on_cb_log_activated(int index)
{
    LOG_FILTER_TYPE	logFilterType = LOG_FILTER_OFF;
    switch (index)
    {
    case 0:logFilterType = LOG_FILTER_OFF; break;
    case 1:logFilterType = LOG_FILTER_DEBUG;break;
    case 2:logFilterType = LOG_FILTER_INFO; break;
    case 3:logFilterType = LOG_FILTER_WARN; break;
    case 4:logFilterType = LOG_FILTER_ERROR; break;
    case 5:logFilterType = LOG_FILTER_CRITICAL; break;
    case 6:logFilterType = LOG_FILTER_MASK; break;
    default:break;
    }
   CAgoraObject::getInstance()->SetLogFilter(logFilterType,L"");
}

void AVDevice::updateBeautyOptions()
{
	BeautyOptions options;
	options.lighteningContrastLevel = (BeautyOptions::LIGHTENING_CONTRAST_LEVEL)ui->cbContrastLevel->currentIndex();
	options.lighteningLevel = ui->horizontalSliderLightening->value() / 100.0f;
	options.rednessLevel = ui->horizontalSlider_Redness->value() / 100.0f;
	options.smoothnessLevel = ui->horizontalSlider_Smoothness->value() / 100.0f;

	CAgoraObject::getInstance()->setBeautyEffectOptions(m_bEnableBeauty, options);
}

void AVDevice::on_cbContrastLevel_activated(int index)
{
	updateBeautyOptions();
}

void AVDevice::on_valueChanged_horizontalSlider_Redness(int value)
{
	QString redness;
	redness.sprintf("Redness(%.02f)", value / 100.0f);
	ui->labelRedness->setText(redness);
	gAgoraConfig.setRedness(value);
	updateBeautyOptions();
}

void AVDevice::on_valueChanged_horizontalSlider_Smoothness(int value)
{
	QString smooth;
	smooth.sprintf("Smoothness(%.02f)", value / 100.0f);
	ui->labelSmoothness->setText(smooth);
	gAgoraConfig.setSmoothness(value);
	updateBeautyOptions();
}

void AVDevice::on_valueChanged_horizontalSlider_Lightening(int value)
{
	QString lightening;
	lightening.sprintf("Lightening(%.02f)", value / 100.0f);
	ui->labelLightening->setText(lightening);
	gAgoraConfig.setLightenging(value);
	updateBeautyOptions();
}
