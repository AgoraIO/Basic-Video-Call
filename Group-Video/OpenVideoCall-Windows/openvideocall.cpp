#include "openvideocall.h"
#include "ui_openvideocall.h"
#include "agorawindowmanager.h"
#include "agoraobject.h"

OpenVideoCall::OpenVideoCall(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenVideoCall),
    m_upAvDevice(Q_NULLPTR)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    AgoraWindowManager::getInstance()->insertItem(qWndType_OpenVideoCall,this);
    initWindow();
}

OpenVideoCall::~OpenVideoCall()
{
    delete ui;
}

void OpenVideoCall::initWindow()
{
    QString strDir = QCoreApplication::applicationDirPath();
    strDir.append("\\AgoraSDK.log");
    CAgoraObject::getInstance(this)->setLogPath(strDir);

    CAgoraObject::getInstance()->enableAudio(true);
    CAgoraObject::getInstance()->enableVideo(true);
    CAgoraObject::getInstance()->EnableWebSdkInteroperability(true);
    CAgoraObject::getInstance()->SetChannelProfile(CHANNEL_PROFILE_COMMUNICATION);

    ui->com_encryp->addItem("AES-128-XTS");
    ui->com_encryp->addItem("AES-256-XTS");
    ui->com_encryp->addItem("AES-128-ECB");
    ui->com_encryp->addItem("SM4-128-ECB");
    ui->com_encryp->setCurrentIndex(0);
}

void OpenVideoCall::mousePressEvent(QMouseEvent *e)
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

void OpenVideoCall::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bMousePressed == true) {
        QPoint movePos = e->globalPos() - m_mousePosition;
        move(movePos);
        e->accept();
    }
}

void OpenVideoCall::mouseReleaseEvent(QMouseEvent *e)
{
   m_bMousePressed = false;
}

void OpenVideoCall::on_btn_close_clicked()
{
    this->close();
}

void OpenVideoCall::on_btn_min_clicked()
{
    this->showMinimized();
}

void OpenVideoCall::on_btn_config_clicked()
{
    this->hide();
    QString qsChannel = ui->led_classid->text();
    m_upAvDevice.reset(new AVDevice(this,qsChannel));
    m_upAvDevice->show();
}

void OpenVideoCall::on_led_classid_returnPressed()
{
    //todo;ignore.
}

void OpenVideoCall::on_btn_test_network_clicked()
{
    this->hide();
    m_upNetTesting.reset(new NetTesting());
    m_upNetTesting->show();
}

void OpenVideoCall::on_btn_join_clicked()
{
    //to do joinchannel.
    QString qsClassId = ui->led_classid->text();
    this->hide();
    QString qsChannel = ui->led_classid->text();

    CAgoraObject::getInstance()->SetDefaultParameters();

    if(m_upAvDevice)
        m_upAvDevice->SetCustomVideoProfile();
    else
        CAgoraObject::getInstance()->SetCustomVideoProfile();

    QString qsEncrypSecret = ui->led_key->text();
    QString qsEncrypMode = ui->com_encryp->currentText();

    // configuration of encrypt
    EncryptionConfig config;
    // set encrypt mode
	config.encryptionMode = (ENCRYPTION_MODE)(ui->com_encryp->currentIndex() + 1);
    // set encrypt key
    config.encryptionKey = qsEncrypSecret.toUtf8().data();
    // EnableEncryption of engine.
    CAgoraObject::getInstance()->EnableEncryption(true, config);
    m_upInRoom.reset(new InRoom());
    QString token = APP_TOKEN;
    if(token.isEmpty())
        token = CAgoraObject::getInstance()->GetAppToken();
    m_upInRoom->joinchannel(qsClassId,token,0);
}

void OpenVideoCall::on_com_encryp_currentIndexChanged(const QString &arg1)
{
    QString qsEncrypSecret = ui->led_key->text();
    // configuration of encrypt
    EncryptionConfig config;
    // set encrypt mode
    config.encryptionMode = (ENCRYPTION_MODE)(ui->com_encryp->currentIndex() + 1);
    // set encrypt key
    config.encryptionKey = qsEncrypSecret.toUtf8().data();
    // EnableEncryption of engine.
    CAgoraObject::getInstance()->EnableEncryption(true, config);
}


