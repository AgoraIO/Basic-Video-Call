#pragma once
#include "afxcoll.h"

#define WM_WINDOWSHARE			WM_USER+303
// CAGScreenCaptureWnd

class CAGScreenCaptureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGScreenCaptureDlg)

public:
	CAGScreenCaptureDlg(CWnd* pParent = NULL);
	virtual ~CAGScreenCaptureDlg();

	enum { IDD = IDD_SCRCAP_DIALOG };

	int	 RefreashWndInfo();
	BOOL SaveScreen(LPCRECT lpRect);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

	static BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam);

private:
	CBitmap		m_bmpDesktop;

	CList<HWND>	m_listWnd;
	HWND		m_hMarkWnd;

public:
	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


