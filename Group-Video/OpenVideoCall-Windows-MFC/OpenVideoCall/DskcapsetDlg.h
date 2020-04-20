#pragma once
#include "afxwin.h"


// CDskcapsetDlg 对话框

class CDskcapsetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDskcapsetDlg)

public:
	CDskcapsetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDskcapsetDlg();

// 对话框数据
	enum { IDD = IDD_DSKCAPTB_DIALOG };

	int GetCaptureFPS();
	void SetCaptureRect(LPCRECT lpRect);
	void GetCaptureRect(LPRECT lpRect);

	int GetBitrate() { return m_nBitrate; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);


	DECLARE_MESSAGE_MAP()

// data
private:
	CRect		m_rcRegion;
	int			m_nBitrate;

// controls
private:
	CComboBox m_cbxCaptureFPS;
};
