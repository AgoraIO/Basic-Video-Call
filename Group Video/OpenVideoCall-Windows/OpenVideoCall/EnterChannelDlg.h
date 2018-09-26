#pragma once
#include "AGButton.h"
#include "AGEdit.h"
#include "AGComboBox.h"
#include "DeviceDlg.h"
#include "afxwin.h"

// CEnterChannelDlg 对话框

class CEnterChannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterChannelDlg)

public:
	CEnterChannelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEnterChannelDlg();

	CString GetChannelName();

	void SetVideoString(LPCTSTR lpVideoString);
    void CleanEncryptionSecret();

// 对话框数据
	enum { IDD = IDD_ENTERCHANNEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtntestChannel();
	afx_msg void OnBnClickedBtnjoinChannel();
	afx_msg void OnBnClickedBtnsetChannel();

	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void DrawClient(CDC *lpDC);

private:
	CAGEdit			m_ctrChannel;
	CAGEdit			m_ctrEncKey;
    CAGComboBox     m_cmbEncType;
    CAGButton		m_btnTest;
	CAGButton		m_btnJoin;
	CAGButton		m_btnSetup;

	CFont			m_ftEncy;
	CFont			m_ftHead;
	CFont			m_ftDesc;
	CFont			m_ftBtn;

    CPen            m_penFrame;

	CDeviceDlg		m_dlgDevice;
};
