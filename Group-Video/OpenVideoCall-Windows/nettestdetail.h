#ifndef NETTESTDETAIL_H
#define NETTESTDETAIL_H

#include <qevent.h>
#include <QMainWindow>
#include <IAgoraRtcEngine.h>
using namespace agora::rtc;

namespace Ui {
class NetTestDetail;
}

class NetTestDetail : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetTestDetail(QWidget *parent = 0);
    ~NetTestDetail();

protected slots:
    void receiver_network_detail(int quality,const LastmileProbeResult& lr);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void on_btn_back_clicked();

    void on_btn_min_clicked();

    void on_btn_close_clicked();

    void on_btn_ok_clicked();

private:
    const int lnGapWidth = 18;
    const int lnGapHeight = 12;
    const int lnTitleWidth = 718;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;


private:
    Ui::NetTestDetail *ui;
};

#endif // NETTESTDETAIL_H
