#ifndef NETTESTING_H
#define NETTESTING_H

#include <qevent.h>
#include <Memory>
#include "nettestresult.h"
#include <QMainWindow>
#include <IAgoraRtcEngine.h>
using namespace agora::rtc;

namespace Ui {
class NetTesting;
}

class NetTesting : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetTesting(QWidget *parent = 0);
    ~NetTesting();

signals:
    void sender_network_result(int quality,const LastmileProbeResult& lr);

private slots:
    void on_btn_back_clicked();
    void on_btn_min_clicked();
    void on_btn_close_clicked();

    void receive_lastmileQuality(int quality);
    void receive_lastmileProbeResult(const LastmileProbeResult &result);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    const int lnGapWidth = 18;
    const int lnGapHeight = 12;
    const int lnTitleWidth = 718;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;

    int    m_nQuality;
    LastmileProbeResult m_lp;

private:
    std::unique_ptr<NetTestResult>  m_upNtr;
    Ui::NetTesting *ui;
};

#endif // NETTESTING_H
