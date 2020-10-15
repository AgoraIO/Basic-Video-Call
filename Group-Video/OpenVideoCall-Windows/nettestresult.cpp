#include "nettestresult.h"
#include "ui_nettestresult.h"
#include "agorawindowmanager.h"

NetTestResult::NetTestResult(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetTestResult)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    AgoraWindowManager::getInstance()->insertItem(qWndType_NetTestResult,this);
}

NetTestResult::~NetTestResult()
{
    delete ui;
}

void NetTestResult::receiver_network_result(int quality,const LastmileProbeResult& lr)
{
    m_nQuality = quality;
    m_lr = lr;
    QUALITY_TYPE qt = static_cast<QUALITY_TYPE>(quality);
    qDebug("%s(%d)",__FUNCTION__,quality);
    switch(qt) {
        case QUALITY_TYPE::QUALITY_EXCELLENT:
        case QUALITY_TYPE::QUALITY_GOOD: {
            ui->lb_status->setStyleSheet("QLabel{image: url(:/uiresource/pic-good.png)}");
            ui->lb_status_desc->setText("Your NetWork is Great");
        }
            break;
        case QUALITY_TYPE::QUALITY_POOR:{
            ui->lb_status->setStyleSheet("QLabel{image: url(:/uiresource/pic-poor.png)}");
            ui->lb_status_desc->setText("Your NetWork is Poor");

        }
            break;
        case QUALITY_TYPE::QUALITY_BAD:
        case QUALITY_TYPE::QUALITY_VBAD:
        case QUALITY_TYPE::QUALITY_DOWN:{
             ui->lb_status->setStyleSheet("QLabel{image: url(:/uiresource/pic-bad.png)}");
             ui->lb_status_desc->setText("Your NetWork is Bad");
        }
            break;
    default: {
             ui->lb_status->setStyleSheet("");
             ui->lb_status_desc->setText("Your NetWork is Unknown");
        }
        break;
    };
}

void NetTestResult::mousePressEvent(QMouseEvent *e)
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

void NetTestResult::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bMousePressed == true) {
        QPoint movePos = e->globalPos() - m_mousePosition;
        move(movePos);
        e->accept();
    }
}

void NetTestResult::mouseReleaseEvent(QMouseEvent *e)
{
   m_bMousePressed = false;
}

void NetTestResult::on_btn_detail_clicked()
{
    this->hide();
    m_upntd.reset(new NetTestDetail());
    connect(this,SIGNAL(sender_network_detail(int,LastmileProbeResult)),
            m_upntd.get(),SLOT(receiver_network_detail(int,LastmileProbeResult)));
    emit sender_network_detail(m_nQuality,m_lr);
    m_upntd->show();
}

void NetTestResult::on_btn_ok_clicked()
{
    on_btn_back_clicked();
}

void NetTestResult::on_btn_min_clicked()
{
    this->showMinimized();
}

void NetTestResult::on_btn_close_clicked()
{
    on_btn_back_clicked();
}

void NetTestResult::on_btn_back_clicked()
{
    this->hide();
    QMainWindow* pWnd = AgoraWindowManager::getInstance()->getQWnd(qWndType_OpenVideoCall);
    if(pWnd)
        pWnd->show();
}
