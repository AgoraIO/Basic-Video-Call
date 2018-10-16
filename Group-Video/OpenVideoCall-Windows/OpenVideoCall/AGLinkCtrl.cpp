// AGLinkCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "AGLinkCtrl.h"


// CAGLinkCtrl

IMPLEMENT_DYNAMIC(CAGLinkCtrl, CStatic)

CAGLinkCtrl::CAGLinkCtrl()
: m_crText(RGB(0x00, 0x9E, 0xEB))
, m_crHover(RGB(0x00, 0xA0, 0xE9))
, m_crBack(RGB(0xFF, 0xFF, 0xFF))
, m_bOverControl(FALSE)
, m_bTrackMouseEvent(FALSE)
{

}

CAGLinkCtrl::~CAGLinkCtrl()
{

}


BEGIN_MESSAGE_MAP(CAGLinkCtrl, CStatic)
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CAGLinkCtrl 消息处理程序

void CAGLinkCtrl::PreSubclassWindow()
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ModifyStyle(0, SS_NOTIFY, 0);

    // Check that the window text isn't empty. If it is, set it as the URL.
 
    CStatic::PreSubclassWindow();
}


void CAGLinkCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    if (!m_bOverControl){
        m_bOverControl = TRUE;
        Invalidate(FALSE);
    }

    if (!m_bTrackMouseEvent) {
        TRACKMOUSEEVENT TrackMouseEvent;
        TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        TrackMouseEvent.hwndTrack = GetSafeHwnd();
        TrackMouseEvent.dwFlags = TME_LEAVE;
        TrackMouseEvent.dwHoverTime = HOVER_DEFAULT;

        m_bTrackMouseEvent = ::TrackMouseEvent(&TrackMouseEvent);
    }

    CStatic::OnMouseMove(nFlags, point);
}

void CAGLinkCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO:  在此处添加消息处理程序代码
    // 不为绘图消息调用 CLinkCtrl::OnPaint()

    CString strWindowText;
    CRect	rcClient;

    GetClientRect(&rcClient);

    CFont *lpDefaultFont = dc.SelectObject(GetFont());
    dc.SetTextColor(m_crText);
    dc.SetBkColor(m_crBack);
    if (m_bOverControl)
        dc.SetTextColor(m_crHover);
    
    GetWindowText(strWindowText);
    dc.DrawText(strWindowText, &rcClient, DT_LEFT);
    dc.SelectObject(lpDefaultFont);
}


BOOL CAGLinkCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    HCURSOR hLinkCursor = ::LoadCursor(NULL, IDC_HAND);
    if (hLinkCursor != NULL) {
        ::SetCursor(hLinkCursor);
        return TRUE;
    }

    return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CAGLinkCtrl::OnMouseLeave()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    m_bTrackMouseEvent = FALSE;
    m_bOverControl = FALSE;
    Invalidate(FALSE);

    CStatic::OnMouseLeave();
}

void CAGLinkCtrl::SetTextColor(COLORREF crText, COLORREF crHover)
{
    m_crText = crText;
    m_crHover = crHover;
}

void CAGLinkCtrl::SetBackColor(COLORREF crBack)
{
    m_crBack = crBack;
}

COLORREF CAGLinkCtrl::GetTextColor() const
{
    return m_crText;
}

COLORREF CAGLinkCtrl::GetHoverColor() const
{
    return m_crHover;
}