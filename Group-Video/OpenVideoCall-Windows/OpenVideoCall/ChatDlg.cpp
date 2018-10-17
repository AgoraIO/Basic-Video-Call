// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "ChatDlg.h"
#include "afxdialogex.h"


// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
{

}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDCHAT_MESSAGE, m_edtChatBox);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CChatDlg 消息处理程序


BOOL CChatDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    LONG lExStyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
    ::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, lExStyle | WS_EX_LAYERED);

    SetBackgroundColor(RGB(0, 0, 0));
    SetLayeredWindowAttributes(0, 130, LWA_ALPHA);
    m_nStreamID = CAgoraObject::GetAgoraObject()->CreateMessageStream();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}

void CChatDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO:  在此处添加消息处理程序代码
    if (::IsWindow(m_edtChatBox.GetSafeHwnd()))
        m_edtChatBox.MoveWindow(1, cy - 21, cx - 2, 20);
}


BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO:  在此添加专用代码和/或调用基类
    CString str;

    if (pMsg->message == WM_KEYDOWN){
        if (pMsg->wParam == VK_RETURN) {
            m_edtChatBox.GetWindowText(str);
            if (str.GetLength() > 0) {
                CAgoraObject::GetAgoraObject()->SendChatMessage(m_nStreamID, str);
                AddChatMessage(0, str);
                m_edtChatBox.SetWindowText(_T(""));
            }
        }
        
        return FALSE;
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CChatDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO:  在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()

    int y = 100;

    POSITION pos = m_strMsgList.GetHeadPosition();
    dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
    dc.SetBkColor(RGB(0, 0, 0));
    
    while (pos != NULL) {
        CString str = m_strMsgList.GetNext(pos);
        dc.TextOut(20, y, str);
        y -= 20;
    }
}

void CChatDlg::AddChatMessage(UINT nUID, LPCTSTR lpMessage)
{
    CString str;

    if (nUID != 0)
        str.Format(_T("%d: %s"), nUID, lpMessage);
    else
        str.Format(_T("me: %s"), lpMessage);
    
    if (m_strMsgList.GetCount() >= 5)
        m_strMsgList.RemoveTail();

    m_strMsgList.AddHead(str);

    Invalidate(TRUE);
}

void CChatDlg::ClearHistory()
{
    m_strMsgList.RemoveAll();

    Invalidate(TRUE);
}