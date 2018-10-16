#pragma once
#include "afxwin.h"
#include <afxcoll.h>

// CChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
	enum { IDD = IDD_MESSAGE_DIALOG };

    void AddChatMessage(UINT nUID, LPCTSTR lpMessage);
    void ClearHistory();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()


private:
    CEdit m_edtChatBox;

    CStringList m_strMsgList;
    int     m_nStreamID;
};
