#ifndef OPENVIDEOCALL_H
#define OPENVIDEOCALL_H

#include <QMainWindow>
#include <qevent.h>
#include "avdevice.h"
#include "nettesting.h"
#include <memory>
#include "inroom.h"

namespace Ui {
class OpenVideoCall;
}

class OpenVideoCall : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenVideoCall(QWidget *parent = 0);
    ~OpenVideoCall();

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    void initWindow();

private slots:
    void on_btn_close_clicked();
    void on_btn_min_clicked();
    void on_btn_config_clicked();
    void on_led_classid_returnPressed();
    void on_btn_test_network_clicked();
    void on_btn_join_clicked();

    void on_com_encryp_currentIndexChanged(const QString &arg1);

private:
    const int lnGapWidth = 18;
    const int lnGapHeight = 12;
    const int lnTitleWidth = 718;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;

private:
    Ui::OpenVideoCall *ui;
    std::unique_ptr<AVDevice>   m_upAvDevice;
    std::unique_ptr<NetTesting> m_upNetTesting;
    std::unique_ptr<InRoom> m_upInRoom;
};

#endif // OPENVIDEOCALL_H
