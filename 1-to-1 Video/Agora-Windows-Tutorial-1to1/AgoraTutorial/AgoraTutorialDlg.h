
#pragma once
#include "afxmenubutton.h"
#include "AgoraObject.h"
#include "afxwin.h"


class CAgoraTutorialDlg : public CDialog
{
public:
	CAgoraTutorialDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_AGORATUTORIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedBtnjoin();
	afx_msg void OnBnClickedBtnleave();
	afx_msg void OnBnClickedBtndisvid();
	afx_msg void OnBnClickedBtnmutelocaud();

	// ENGINE EVENT MESSAGE
	afx_msg LRESULT OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CString			m_strChannelName;

	CAgoraObject	*m_lpAgoraObject;

	CStatic		m_wndRemote;
	CStatic		m_wndLocal;
};
