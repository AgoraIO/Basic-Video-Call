// AGScreenCaptureWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "AGScreenCaptureDlg.h"


// CAGScreenCaptureWnd

IMPLEMENT_DYNAMIC(CAGScreenCaptureDlg, CDialogEx)

CAGScreenCaptureDlg::CAGScreenCaptureDlg(CWnd* pParent)
	: m_hMarkWnd(NULL)
{
	
}

CAGScreenCaptureDlg::~CAGScreenCaptureDlg()
{
}


BEGIN_MESSAGE_MAP(CAGScreenCaptureDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CAGScreenCaptureWnd 消息处理程序
int	 CAGScreenCaptureDlg::RefreashWndInfo()
{
	m_listWnd.RemoveAll();
	::EnumWindows(&CAGScreenCaptureDlg::WndEnumProc, (LPARAM)&m_listWnd);

	return static_cast<int>(m_listWnd.GetCount());
}

BOOL CAGScreenCaptureDlg::SaveScreen(LPCRECT lpRect)
{
	CDC			dcDesktop;
	CRect		rcDesktop;
	CRect		rcDest;

	CBitmap		*lpOldBitmap = NULL;

	CWnd	*lpWndDesktop = GetDesktopWindow();
	CDC		*lpDCDesktop = lpWndDesktop->GetDC();

	lpWndDesktop->GetWindowRect(&rcDesktop);

	if (lpRect == NULL)
		rcDest.CopyRect(&rcDesktop);
	else {
		if (lpRect->left < rcDesktop.left || lpRect->top < rcDesktop.top || lpRect->right > rcDesktop.right || lpRect->bottom > rcDesktop.bottom)
			return FALSE;
		else
			rcDest.CopyRect(lpRect);
	}

	dcDesktop.CreateCompatibleDC(lpDCDesktop);
	
	if (m_bmpDesktop.GetSafeHandle() != NULL)
		m_bmpDesktop.DeleteObject();

	m_bmpDesktop.CreateCompatibleBitmap(lpDCDesktop, rcDest.Width(), rcDest.Height());
	lpOldBitmap = dcDesktop.SelectObject(&m_bmpDesktop);
	dcDesktop.BitBlt(0, 0, rcDesktop.Width(), rcDesktop.Height(), lpDCDesktop, 0, 0, SRCCOPY);
	dcDesktop.SelectObject(lpOldBitmap);

	return TRUE;
}

void CAGScreenCaptureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	POSITION	pos = m_listWnd.GetHeadPosition();
	CRect		rcMarkWnd;
	HWND		hMarkWnd = NULL;
	HWND		hFound = NULL;

	while (pos != NULL) {
		hFound = m_listWnd.GetNext(pos);
		::GetWindowRect(hFound, &rcMarkWnd);
		if (rcMarkWnd.PtInRect(point)) {
			hMarkWnd = hFound;
			break;
		}
	}

	if (hMarkWnd != m_hMarkWnd) {
		m_hMarkWnd = hMarkWnd;
		Invalidate();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CAGScreenCaptureDlg::OnPaint()
{
	CPaintDC	dc(this);
	CDC			dcBuffer;
	CBitmap		bmpBuffer;
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect			rcWindow;
	BLENDFUNCTION	blend;
	GetWindowRect(&rcWindow);
	
	CBrush brBack;
	brBack.CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	dcBuffer.CreateCompatibleDC(&dc);
	bmpBuffer.CreateCompatibleBitmap(&dc, rcWindow.Width(), rcWindow.Height());

	CBitmap *lpOldBitmap = dcBuffer.SelectObject(&bmpBuffer);
	dcBuffer.FillRect(&rcWindow, &brBack);

	blend.BlendOp = AC_SRC_OVER;
	blend.AlphaFormat = 0;
	blend.SourceConstantAlpha = 0x80;
	blend.BlendFlags = 0;

	dc.AlphaBlend(0, 0, rcWindow.Width(), rcWindow.Height(), &dcBuffer, 0, 0, rcWindow.Width(), rcWindow.Height(), blend);

	if (m_hMarkWnd != NULL) {
		CRect	rcMarkWnd;
		CDC		dcBitmap;
		CPen	penMark;
		CString	strTip;

		penMark.CreatePen(PS_SOLID, 4, RGB(0x00, 0xA0, 0xE9));
		::GetWindowRect(m_hMarkWnd, &rcMarkWnd);
		dcBitmap.CreateCompatibleDC(&dc);
		dcBitmap.SelectObject(&m_bmpDesktop);
		dc.BitBlt(rcMarkWnd.left, rcMarkWnd.top, rcMarkWnd.Width(), rcMarkWnd.Height(), &dcBitmap, rcMarkWnd.left, rcMarkWnd.top, SRCCOPY);
		
		dc.SelectObject(&penMark);
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(&rcMarkWnd);

		strTip = _T("double click to share this window or ESC to abort");
//		dc.SetBkMode(TRANSPARENT);
		dc.SetBkColor(RGB(0x00, 0xA0, 0xE9));
		dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		dc.TextOut(rcMarkWnd.left + 10, rcMarkWnd.top+10, strTip);
	}
}


BOOL CALLBACK CAGScreenCaptureDlg::WndEnumProc(HWND hWnd, LPARAM lParam)
{
	CList<HWND> *lpListctrl = (CList<HWND> *)lParam;

	LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);

	if ((lStyle&WS_VISIBLE) != 0 && (lStyle&(WS_POPUP | WS_SYSMENU)) != 0)
		lpListctrl->AddTail(hWnd);

	return TRUE;
}


BOOL CAGScreenCaptureDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CDC dcBuffer;
	dcBuffer.CreateCompatibleDC(pDC);
	dcBuffer.SelectObject(&m_bmpDesktop);

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	pDC->BitBlt(0, 0, rcWindow.Width(), rcWindow.Height(), &dcBuffer, 0, 0, SRCCOPY);

	return TRUE;
}


void CAGScreenCaptureDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码
//	if (!bShow)
//		m_bmpDesktop.DeleteObject();
	
}


void CAGScreenCaptureDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	GetParent()->SendMessage(WM_WINDOWSHARE, (WPARAM)m_hMarkWnd, 0);
//	CDialogEx::OnLButtonDblClk(nFlags, point);
	CDialogEx::OnOK();
}


void CAGScreenCaptureDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

//	CDialogEx::OnRButtonDown(nFlags, point);
	CDialogEx::OnOK();
}
