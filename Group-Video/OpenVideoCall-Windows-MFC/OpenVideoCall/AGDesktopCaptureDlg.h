#pragma once
#include "DskcapsetDlg.h"

// CAGDesktopCaptureDlg 对话框

#define WM_DESKTOPSHARE			WM_USER+304

typedef struct _DESKTOP_SHARE_PARAM
{
	int nX;
	int nY;
	int nWidth;
	int nHeight;
	int nFPS;
	int nBitrate;

} DESKTOP_SHARE_PARAM, *PDESKTOP_SHARE_PARAM, *LPDESKTOP_SHARE_PARAM;

class CAGDesktopCaptureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGDesktopCaptureDlg)

public:
	CAGDesktopCaptureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAGDesktopCaptureDlg();

	BOOL SaveScreen(LPCRECT lpRect);
// 对话框数据
	enum { IDD = IDD_SCRCAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	CBitmap		m_bmpDesktop;
	BOOL		m_bMouseLDown;

	CPoint		m_ptStart;
	CPoint		m_ptEnd;
	CRect		m_rcRegion;

//	CComboBox	m_cmbFrameRate;
	CDskcapsetDlg	m_dlgCapSet;
};
