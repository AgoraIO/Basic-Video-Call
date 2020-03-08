#include "inroomdig.h"
#include "ui_inroomdig.h"
#include "agoraobject.h"
#include "qquickwidget.h"
#include "QHBoxLayout"
#include "QStackedLayout"

InRoomDig::InRoomDig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InRoomDig)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    initWindow();
}

InRoomDig::~InRoomDig()
{
    delete ui;
}

void InRoomDig::initWindow()
{
    QString strDir = QCoreApplication::applicationDirPath();
    strDir.append("\\AgoraSDK.log");
    CAgoraObject::getInstance(this)->setLogPath(strDir);

    CAgoraObject::getInstance()->enableAudio(true);
    CAgoraObject::getInstance()->enableVideo(true);
    CAgoraObject::getInstance()->EnableWebSdkInteroperability(true);
    CAgoraObject::getInstance()->SetChannelProfile(CHANNEL_PROFILE_COMMUNICATION);
    CAgoraObject::getInstance()->joinChannel("","233",0);

    QHBoxLayout *main_layout = new QHBoxLayout();
    QStackedLayout *stacked_layout = new QStackedLayout();
    stacked_layout->setMargin(0);
    stacked_layout->setStackingMode(QStackedLayout::StackAll);

    QLabel* player_status_ = new QLabel("test", this);
    player_status_->setGeometry(618,20,130,32);
    player_status_->move(618,20);
    player_status_->resize(130,32);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    player_status_->setPalette(pe);
    player_status_->setAlignment(Qt::AlignCenter|Qt::AlignCenter);

    stacked_layout->addWidget(player_status_);
    main_layout->addLayout(stacked_layout);
    setLayout(main_layout);
    player_status_->raise();

    QWidget* player_video_ = new QWidget();
    player_video_->setGeometry(0,0,1366,768);
    player_video_->setLayout(main_layout);
   // this->setCentralWidget(player_video_);
    player_video_->show();

    CAgoraObject::getInstance()->LocalVideoPreview((HWND)(player_video_->winId()),TRUE);
}
