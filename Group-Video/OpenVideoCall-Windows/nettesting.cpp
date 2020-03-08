#include "nettesting.h"
#include "ui_nettesting.h"
#include <QMovie>
#include "agoraobject.h"
#include "agorawindowmanager.h"

NetTesting::NetTesting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetTesting)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    CAgoraObject* pInstance = CAgoraObject::getInstance();
    connect(pInstance,SIGNAL(sender_lastmileQuality(int)),this,SLOT(receive_lastmileQuality(int)));
    connect(pInstance,SIGNAL(sender_lastmileProbeResult(const LastmileProbeResult&)),this,SLOT(receive_lastmileProbeResult(LastmileProbeResult)));

    CAgoraObject::getInstance()->startLastMileProbeTest(true);
    QMovie* pMovie = new QMovie("uiresource\\loading.gif");
    ui->lb_status->setMovie(pMovie);
    pMovie->start();

    AgoraWindowManager::getInstance()->insertItem(qWndType_NetTesting,this);
}

NetTesting::~NetTesting()
{
    delete ui;
}

void NetTesting::on_btn_back_clicked()
{
    this->hide();
    CAgoraObject::getInstance()->startLastMileProbeTest(false);
    QMainWindow* pWnd = AgoraWindowManager::getInstance()->getQWnd(qWndType_OpenVideoCall);
    if(pWnd)
        pWnd->show();
}

void NetTesting::on_btn_min_clicked()
{
    this->showMinimized();
}

void NetTesting::on_btn_close_clicked()
{
    on_btn_back_clicked();
}

void NetTesting::receive_lastmileQuality(int quality)
{
    m_nQuality = quality;
}

void NetTesting::receive_lastmileProbeResult(const LastmileProbeResult &result)
{
    if(m_nQuality >=1 && m_nQuality <= 6)
        CAgoraObject::getInstance()->startLastMileProbeTest(false);
    this->hide();
    m_lp = result;

    m_upNtr.reset(new NetTestResult());
    connect(this,SIGNAL(sender_network_result(int,LastmileProbeResult)),
            m_upNtr.get(),SLOT(receiver_network_result(int,LastmileProbeResult)));
    emit sender_network_result(m_nQuality,m_lp);
    m_upNtr->show();
}

void NetTesting::mousePressEvent(QMouseEvent *e)
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

void NetTesting::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bMousePressed == true) {
        QPoint movePos = e->globalPos() - m_mousePosition;
        move(movePos);
        e->accept();
    }
}

void NetTesting::mouseReleaseEvent(QMouseEvent *e)
{
   m_bMousePressed = false;
}

