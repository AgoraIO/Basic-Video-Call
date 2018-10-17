#pragma once
#include "AGButton.h"
#include "AGComboBox.h"
#include "AGSliderCtrl.h"
#include "AGLinkCtrl.h"
#include "AGVideoTestWnd.h"

#include "AgoraPlayoutManager.h"
#include "AgoraAudInputManager.h"
#include "AgoraCameraManager.h"

// CDeviceDlg 对话框

class CDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceDlg)

public:
	CDeviceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceDlg();

// 对话框数据
	enum { IDD = IDD_DEVICE_DIALOG };

	void EnableDeviceTest(BOOL bEnable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

    afx_msg void OnStnClickedSlkainDevice();
    afx_msg void OnStnClickedSlkaoutDevice();
    afx_msg void OnStnClickedSlkcamDevice();
	afx_msg void OnStnClickedSlkechoDevice();

	afx_msg void OnBnClickedBtncancelDevice();
	afx_msg void OnBnClickedBtnconfirmDevice();

	afx_msg LRESULT OnEIDAudioVolumeIndication(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void DrawClient(CDC *lpDC);


private:
    CFont       m_ftLink;
    CFont		m_ftDes;		// text in ctrl
	CFont		m_ftBtn;		// button

    CPen            m_penFrame;
	CAGComboBox		m_cbxAIn;
	CAGComboBox		m_cbxAOut;
	CAGComboBox		m_cbxCam;

    CAGLinkCtrl       m_slkAudInTest;
    CAGLinkCtrl       m_slkAudOutTest;
    CAGLinkCtrl       m_slkCamTest;
	CAGLinkCtrl		  m_slkEchoTest;

	CAGSliderCtrl	m_sldAInVol;
	CAGSliderCtrl	m_sldAOutVol;

	CAGButton		m_btnCancel;
	CAGButton		m_btnConfirm;

	CAGVideoTestWnd	m_wndVideoTest;

private:
	IRtcEngine			    *m_lpRtcEngine;
	CAgoraPlayoutManager	m_agPlayout;
	CAgoraAudInputManager	m_agAudioin;
	CAgoraCameraManager		m_agCamera;

public:
};
