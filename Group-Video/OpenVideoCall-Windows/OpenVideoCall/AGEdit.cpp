// AGEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "AGEdit.h"


// CAGEdit

IMPLEMENT_DYNAMIC(CAGEdit, CEdit)

CAGEdit::CAGEdit()
: m_crBorder(RGB(0xD8, 0xD8, 0xD8))
, m_crBack(RGB(0xFF, 0xFF, 0xFF))
, m_crText(RGB(0x00, 0xA0, 0xE9))
, m_crTip(RGB(0xD8, 0xD8, 0xD8))
, m_strTip(_T("请输入文本"))
, m_bTrackMouseEvent(FALSE)
, m_bOverControl(FALSE)
, m_bEmpty(TRUE)
, m_bTexting(FALSE)
{
	m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
	m_brushBack.CreateSolidBrush(m_crBack);
}

CAGEdit::~CAGEdit()
{
	m_penBorder.DeleteObject();
	m_brushBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_CONTROL_REFLECT(EN_CHANGE, &CAGEdit::OnEnChange)
    ON_WM_CREATE()
END_MESSAGE_MAP()



// CAGEdit 消息处理程序
void CAGEdit::SetTip(LPCTSTR lpTip)
{
    m_strTip = lpTip;

    Invalidate(TRUE);
}

void CAGEdit::SetColor(COLORREF crBorder, COLORREF crBack, COLORREF crText, COLORREF crTip)
{
    if (crBorder != m_crBorder){
        m_penBorder.DeleteObject();
        m_crBorder = crBorder;
        m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
    }

    if (crBack != m_crBack){
        m_brushBack.DeleteObject();
        m_crBack = crBack;
        m_brushBack.CreateSolidBrush(m_crBack);
    }

	Invalidate();
}

BOOL CAGEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcWindow;
	
	GetWindowRect(&rcWindow);

	CPen *lpDefaultPen = pDC->SelectObject(&m_penBorder);
	pDC->Rectangle(&rcWindow);
	pDC->SelectObject(lpDefaultPen);

	return CEdit::OnEraseBkgnd(pDC);
}

HBRUSH CAGEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何属性
	pDC->SetBkColor(m_crBack);

    if (m_bEmpty)
        pDC->SetTextColor(m_crTip);
    else
        pDC->SetTextColor(m_crText);

	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
	return (HBRUSH)m_brushBack;	//编辑框底色画刷
}

void CAGEdit::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CEdit::OnMouseHover(nFlags, point);
}


void CAGEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!m_bTrackMouseEvent) {
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		TrackMouseEvent.hwndTrack = GetSafeHwnd();
		TrackMouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
		TrackMouseEvent.dwHoverTime = HOVER_DEFAULT;

		m_bTrackMouseEvent = ::TrackMouseEvent(&TrackMouseEvent);
	}

	CEdit::OnMouseMove(nFlags, point);
}


void CAGEdit::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bTrackMouseEvent = FALSE;

	CEdit::OnMouseLeave();
}


void CAGEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    // TODO:  在此处添加消息处理程序代码
    if (m_bEmpty) {
        m_bTexting = FALSE;
        SetWindowText(_T(""));
    }

}


void CAGEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);

    // TODO:  在此处添加消息处理程序代码
    m_bTexting = FALSE;
    if (m_bEmpty)
        SetWindowText(m_strTip);
}


void CAGEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    m_bTexting = TRUE;

    if (GetWindowTextLength() <= 2)
        Invalidate();

    CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CAGEdit::OnEnChange()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CEdit::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    if (m_bTexting)
        m_bEmpty = (GetWindowTextLength() == 0) ? TRUE : FALSE;
}


int CAGEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CEdit::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    m_bTexting = FALSE;
    if (m_bEmpty)
        SetWindowText(m_strTip);

    return 0;
}
