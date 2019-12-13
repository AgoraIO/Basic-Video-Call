#ifndef AVDEVICE_H
#define AVDEVICE_H

#include <qevent.h>
#include <QMainWindow>

namespace Ui {
class AVDevice;
}

class AVDevice : public QMainWindow
{
    Q_OBJECT

public:
    explicit AVDevice(QMainWindow* pLastWnd,const QString &qsChanne,QWidget *parent = 0);
    ~AVDevice();

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    void initCtrl();
    void setStyleSheet_audio(bool bEnable);
    void setStyleSheet_video(bool bEnable);

private slots:
    void on_btn_back_clicked();
    void on_cb_solution_activated(int index);
    void on_cb_audio_activated(int index);
    void on_cb_video_activated(int index);
    void on_cb_speaker_activated(int index);
    void on_btn_audio_clicked();
    void on_btn_video_clicked();

	void on_optVideo_Beauty_clicked();

    void on_btn_close_clicked();

    void on_btn_min_clicked();

    void on_cb_solution_activated(const QString &arg1);

    void on_cb_log_activated(int index);

	void on_cbContrastLevel_activated(int index);
	void on_valueChanged_horizontalSlider_Redness(int value);
	void on_valueChanged_horizontalSlider_Smoothness(int value);
	void on_valueChanged_horizontalSlider_Lightening(int value);
private:
	void enableVideoBeutyControl(bool bEnable);
	void updateBeautyOptions();

    const int lnGapWidth = 18;
    const int lnGapHeight = 12;
    const int lnTitleWidth = 718;
    const int lnTitleHeight = 30;
    QPoint m_mousePosition;
    bool   m_bMousePressed;

private:
    Ui::AVDevice *ui;
    QMainWindow* m_lastWnd;
    QString m_strChannel;
    bool    m_bEnableVideo;
    bool    m_bEnableAudio;
	bool    m_bEnableBeauty;
};

#endif // AVDEVICE_H
