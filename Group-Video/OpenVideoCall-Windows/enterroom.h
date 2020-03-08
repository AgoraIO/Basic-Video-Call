#ifndef ENTERROOM_H
#define ENTERROOM_H

#include <qevent.h>
#include <QWidget>
#include <QMainWindow>

namespace Ui {
class wgEnterRoom;
}

class EnterRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit EnterRoom(QWidget *parent = 0);
    ~EnterRoom();

    void joinchannel(const QString& qsChannel,uint uid);
    void leavechannel();

    void setChannelName(const QString& qsChannelInfo);
    void setParam(const QString& qsParam);

    void setAllRAtt(int nRCount);
    void setR1(unsigned int uid);
    void setR2(unsigned int uid);
    void setR3(unsigned int uid);
    int getR1();
    int getR2();
    int getR3();

    void switchUidText(const QString &qsSrc,const QString &qsDest,int nIndex);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

signals:
    void sender_exitChannel();
    void sender_updateVideo();

private slots:

    void on_rbCamera_stateChanged(int arg1);
    void on_rbMic_stateChanged(int arg1);
    void on_btnLeave_clicked();

private:
    const int lnTitleWidth = 1366;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;

private:
    Ui::wgEnterRoom *ui;
    QMainWindow* m_pLastWnd;
    unsigned int   m_uidr1;
    unsigned int   m_uidr2;
    unsigned int   m_uidr3;
};

#endif // ENTERROOM_H
