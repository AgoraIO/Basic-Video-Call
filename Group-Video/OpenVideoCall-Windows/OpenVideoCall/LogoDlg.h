#pragma once
#include "AGButton.h"

// CLogoDlg 对话框

class CLogoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogoDlg)

public:
	CLogoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogoDlg();

// 对话框数据
	enum { IDD = IDD_LOGO_DIALOG };

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnclose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void DrawClient(CDC *lpDC);

private:
	CAGButton	m_btnMin;
	CAGButton	m_btnClose;

	CFont		m_ftTitle;
	CFont		m_ftDescp;

	UINT_PTR	m_nTimerID;
	
private:
    CBitmap     m_bmpLogo;
};
