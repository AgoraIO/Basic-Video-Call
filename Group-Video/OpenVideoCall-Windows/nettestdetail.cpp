#include "nettestdetail.h"
#include "ui_nettestdetail.h"
#include "agorawindowmanager.h"
#include <QString>

NetTestDetail::NetTestDetail(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetTestDetail)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    AgoraWindowManager::getInstance()->insertItem(qWndType_NetTestDetail,this);
}

NetTestDetail::~NetTestDetail()
{
    delete ui;
}

void NetTestDetail::receiver_network_detail(int quality,const LastmileProbeResult& lr)
{
    qDebug(__FUNCTION__);

    ui->lb_rtt_up->setText(QString::number(lr.rtt));
    ui->lb_lost_up->setText(QString::number(lr.uplinkReport.packetLossRate));
    ui->lb_lost_down->setText(QString::number(lr.downlinkReport.packetLossRate));
    ui->lb_bandwidth_up->setText(QString::number(lr.uplinkReport.availableBandwidth));
    ui->lb_bandwidth_down->setText(QString::number(lr.downlinkReport.availableBandwidth));
}

void NetTestDetail::mousePressEvent(QMouseEvent *e)
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

void NetTestDetail::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bMousePressed == true) {
        QPoint movePos = e->globalPos() - m_mousePosition;
        move(movePos);
        e->accept();
    }
}

void NetTestDetail::mouseReleaseEvent(QMouseEvent *e)
{
   m_bMousePressed = false;
}

void NetTestDetail::on_btn_back_clicked()
{
    this->hide();
    QMainWindow* pWnd = AgoraWindowManager::getInstance()->getQWnd(qWndType_OpenVideoCall);
    if(pWnd)
        pWnd->show();
}

void NetTestDetail::on_btn_min_clicked()
{
    this->showMinimized();
}

void NetTestDetail::on_btn_close_clicked()
{
    on_btn_back_clicked();
}

void NetTestDetail::on_btn_ok_clicked()
{
    on_btn_back_clicked();
}
