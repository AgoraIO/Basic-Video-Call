#ifndef NETTESTRESULT_H
#define NETTESTRESULT_H

#include <qevent.h>
#include <Memory>
#include "nettestdetail.h"
#include <QMainWindow>
#include <IAgoraRtcEngine.h>
using namespace agora::rtc;

namespace Ui {
class NetTestResult;
}

class NetTestResult : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetTestResult(QWidget *parent = 0);
    ~NetTestResult();

signals:
    void sender_network_detail(int quality,const LastmileProbeResult& lr);

protected slots:
    void receiver_network_result(int quality,const LastmileProbeResult& lr);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void on_btn_detail_clicked();

    void on_btn_ok_clicked();

    void on_btn_min_clicked();

    void on_btn_close_clicked();

    void on_btn_back_clicked();

private:
    const int lnGapWidth = 18;
    const int lnGapHeight = 12;
    const int lnTitleWidth = 718;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;

private:
    int                 m_nQuality;
    LastmileProbeResult m_lr;
    QMainWindow*     m_pLastWnd;
    Ui::NetTestResult *ui;
    std::unique_ptr<NetTestDetail>  m_upntd;
};

#endif // NETTESTRESULT_H
