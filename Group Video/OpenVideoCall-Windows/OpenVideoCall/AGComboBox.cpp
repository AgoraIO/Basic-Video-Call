// AGComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "AGComboBox.h"

IMPLEMENT_DYNAMIC(CAGComboBoxList, CWnd)

CAGComboBoxList::CAGComboBoxList()
: m_nItemCount(0)
, m_nCurSel(-1)
, m_nCurPointIndex(-1)
, m_crBack(RGB(0xF5, 0xF5, 0xF5))
, m_crBorder(RGB(0xC8, 0xC8, 0xC8))
, m_crText(RGB(0x00, 0x00, 0x00))
, m_crTextHot(RGB(0x00, 0x9E, 0xEB))
, m_lpWndFont(NULL)
{
	m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
}

CAGComboBoxList::~CAGComboBoxList()
{
	m_imgSel.DeleteImageList();
	m_penBorder.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAGComboBoxList, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
//	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
//	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CAGComboBoxList::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CAGComboBox *pAGComboBox, UINT nID)
{
	ASSERT(pAGComboBox != NULL);

	m_lpComboBox = pAGComboBox;

	m_ctrVScroll.Create(SBS_VERT | SBS_LEFTALIGN | WS_CHILD | WS_VISIBLE, CRect(5, 30, 30, 130), this, nID+1);

	return CWnd::Create(NULL, NULL, dwStyle, rect, GetDesktopWindow(), nID, NULL);
}

int CAGComboBoxList::GetCount() const
{
	return m_nItemCount;
}

int CAGComboBoxList::GetCurSel() const
{
	return m_nCurSel;
}

DWORD_PTR CAGComboBoxList::GetItemData(int nIndex) const
{
	return m_arrItemPtrData.GetAt(nIndex);
}

void CAGComboBoxList::GetText(int nIndex, CString& rString) const
{
	rString.Empty();

	if (m_nItemCount == 0)
		return;

	if (nIndex >= 0 && nIndex < m_nItemCount) {
		rString = m_arrItemString.GetAt(nIndex);
		return;
	}
}

int CAGComboBoxList::SetItemData(int nIndex, DWORD_PTR dwItemData)
{
	ASSERT(nIndex >= 0 && nIndex <= m_nItemCount);

	m_arrItemPtrData.SetAt(nIndex, dwItemData);

	return nIndex;
}

int CAGComboBoxList::AddString(LPCTSTR lpszString)
{
	return InsertString(m_nItemCount, lpszString);
}

int CAGComboBoxList::InsertString(int nIndex, LPCTSTR lpszString)
{
	ASSERT(nIndex >= 0 && nIndex <= m_nItemCount);

	int nRet = -1;
	DWORD_PTR dwItemData = 0;

	if (nIndex >= 0 && nIndex <= m_nItemCount) {
		m_arrItemString.InsertAt(nIndex, lpszString);
		m_arrItemPtrData.InsertAt(nIndex, dwItemData);
		m_nItemCount++;

		if (m_nCurSel >= nIndex)
			m_nCurSel += 1;
	}

	return nRet;
}

int CAGComboBoxList::SetCurSel(int nSelect)
{
	if (nSelect < -1 || nSelect >= m_nItemCount)
		return -1;

	m_nCurSel = nSelect;

	return m_nCurSel;
}

int CAGComboBoxList::DeleteString(int nIndex)
{
	if (nIndex < -1 || nIndex >= m_nItemCount)
		return -1;

	m_arrItemPtrData.RemoveAt(nIndex, 1);
	m_arrItemString.RemoveAt(nIndex, 1);
	
	if (m_nCurSel == nIndex)
		m_nCurSel = -1;
	else if (m_nCurSel > nIndex)
		m_nCurSel -= 1;
	
	m_nItemCount--;

	return m_nItemCount;
}

void CAGComboBoxList::ResetContent()
{
	m_arrItemPtrData.RemoveAll();
	m_arrItemString.RemoveAll();
	m_nItemCount = 0;
	m_nCurSel = -1;
}

void CAGComboBoxList::SetFont(CFont* pFont, BOOL bRedraw)
{
	ASSERT(pFont != NULL);

	m_lpWndFont = pFont;
	Invalidate(bRedraw);
}

void CAGComboBoxList::SetItemHeight(int nItemHeight)
{
	m_nPerItemHeight = nItemHeight;
	Invalidate(TRUE);
}

int	CAGComboBoxList::GetItemHeight() const
{
	return m_nPerItemHeight;
}

void CAGComboBoxList::ShowVScrollBar(BOOL bShow)
{

}


void CAGComboBoxList::OnPaint()
{
	CPaintDC dc(this);

	CRect		rcClient;
	CRect		rcBack;
	CRect		rcText;
	CRect		rcSel;
	SCROLLINFO  si;

	memset(&si, 0, sizeof(SCROLLINFO));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	int nStartIndex = si.nPos / m_nPerItemHeight;

	GetClientRect(&rcClient);
	rcBack.SetRect(rcClient.left + 1, rcClient.top + 1, rcClient.right - 1, rcClient.bottom - 1);
	rcText.SetRect(5, 1, rcClient.Width() - 35, m_nPerItemHeight-2);

	dc.SetBkMode(TRANSPARENT);
	CPen *lpOldPen = dc.SelectObject(&m_penBorder);

	dc.Rectangle(&rcClient);
	dc.FillSolidRect(&rcBack, m_crBack);
	dc.SelectObject(m_lpWndFont);

	for (int nIndex = 0; nIndex < m_nItemCount - nStartIndex; nIndex++) {

		dc.SetTextColor(m_crText);
		const CString &strItem = m_arrItemString.GetAt(nIndex + nStartIndex);

		dc.FillSolidRect(&rcText, m_crBack);
		if (m_nCurSel == (nStartIndex + nIndex)){
			;
		}

		if (m_nCurPointIndex == (nStartIndex + nIndex))
			dc.SetTextColor(m_crTextHot);

		dc.DrawText(strItem, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
		rcText.OffsetRect(0, m_nPerItemHeight);
	}

	dc.SelectObject(lpOldPen);
}

void CAGComboBoxList::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rcClient;
	CRect rcDirty;
	int nCurPointIndex = 0;

	GetClientRect(&rcClient);
	if (rcClient.PtInRect(point) && m_nItemCount > 0)
		nCurPointIndex = point.y / m_nPerItemHeight;
	else
		nCurPointIndex = -1;

	if (nCurPointIndex != m_nCurPointIndex) {
		rcDirty.SetRect(0, nCurPointIndex*m_nPerItemHeight, rcClient.Width(), (nCurPointIndex + 1)*m_nPerItemHeight);
		m_nCurPointIndex = nCurPointIndex;
		Invalidate(FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CAGComboBoxList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcClient;
	SCROLLINFO  si;

	memset(&si, 0, sizeof(SCROLLINFO));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	GetScrollInfo(SB_VERT, &si);
	int nStartIndex = si.nPos / m_nPerItemHeight;

	GetClientRect(&rcClient);
	if (!rcClient.PtInRect(point))
		m_nCurPointIndex = -1;
	else {
		if (m_nCurSel != m_nCurPointIndex + nStartIndex) {
			m_nCurSel = m_nCurPointIndex + nStartIndex;
			m_lpComboBox->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(m_lpComboBox->GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)m_lpComboBox->GetSafeHwnd());
		}
		
		if (m_nCurSel != -1)
			m_lpComboBox->SetWindowText(m_arrItemString.GetAt(m_nCurSel));
	}
	m_lpComboBox->ShowDropDown(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CAGComboBoxList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SCROLLINFO  si;
	int			nVertPos = 0;

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);
	nVertPos = si.nPos;
	si.nPos -= 2 * zDelta;
	si.fMask = SIF_POS;
	SetScrollPos(SB_VERT, si.nPos, TRUE);
	Invalidate(FALSE);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

int CAGComboBoxList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	return 0;
}


void CAGComboBoxList::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码
	if (bShow)
		SetCapture();
	else
		ReleaseCapture();

	CWnd::OnShowWindow(bShow, nStatus);
}

/*
BOOL CAGComboBoxList::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect		rcClient;
	CRect		rcBack;

	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	rcBack.CopyRect(&rcClient);
	rcBack.left += 1;
	rcBack.top += 1;
	rcBack.right -= 1;
	rcBack.bottom -= 1;
	
	CPen *lpOldPen = pDC->SelectObject(&m_penBorder);
	pDC->Rectangle(&rcClient);
	pDC->FillSolidRect(&rcBack, m_crBack);
	
	pDC->SelectObject(lpOldPen);

	return TRUE;
	//	return CWnd::OnEraseBkgnd(pDC);
}
*/

// CAGComboBox

IMPLEMENT_DYNAMIC(CAGComboBox, CWnd)

CAGComboBox::CAGComboBox()
: m_nMaxListHeight(200)
, m_nBtnStat(0)
, m_crBack(RGB(0xF5, 0xF5, 0xF5))
, m_crBorder(RGB(0xC8, 0xC8, 0xC8))
, m_crTextNormal(RGB(0x00, 0x00, 0x00))
, m_crTextHot(RGB(0x00, 0x9E, 0xEB))
, m_bMouseTrack(FALSE)
, m_lpWndFont(NULL)
{
	m_penBorder.CreatePen(PS_SOLID, 1, RGB(0xC8, 0xC8, 0xC8));
}

CAGComboBox::~CAGComboBox()
{
	m_imgDropBtn.DeleteImageList();

	m_penBorder.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGComboBox, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CAGComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	CRect rcListBox;

	rcListBox.left = rect.left;
	rcListBox.right = rect.right;
	rcListBox.top = rect.bottom + 1;
	rcListBox.bottom = rcListBox.top + rect.bottom - rect.top;

    m_ctrList.Create(WS_CHILD | WS_VSCROLL, rcListBox, GetDesktopWindow(), this, nID + 1);
	m_ctrList.ShowWindow(SW_HIDE);

	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID, NULL);
}

int CAGComboBox::GetCount() const
{
	return m_ctrList.GetCount();
}

int CAGComboBox::GetCurSel() const
{
	return m_ctrList.GetCurSel();
}

DWORD_PTR CAGComboBox::GetItemData(int nIndex) const
{
	return m_ctrList.GetItemData(nIndex);
}

void CAGComboBox::GetLBText(int nIndex, CString& rString) const
{
	m_ctrList.GetText(nIndex, rString);
}

int CAGComboBox::SetItemData(int nIndex, DWORD_PTR dwItemData)
{
	return m_ctrList.SetItemData(nIndex, dwItemData);
}

void CAGComboBox::ShowDropDown(BOOL bShowIt)
{
	CRect	rcClient;
	int		nItemHeight = m_ctrList.GetItemHeight();

	GetClientRect(&rcClient);
	if (nItemHeight == 0) {
		nItemHeight = rcClient.Height();
		m_ctrList.SetItemHeight(nItemHeight);
	}
	rcClient.OffsetRect(0, rcClient.Height() + 1);

	int	nMaxCanvasHeight = m_ctrList.GetCount()*nItemHeight;
	m_ctrList.ShowVScrollBar(FALSE);
	if (nMaxCanvasHeight == 0)
		rcClient.bottom = rcClient.top + m_ctrList.GetItemHeight();
	else if (nMaxCanvasHeight > 0 && nMaxCanvasHeight < m_nMaxListHeight)
		rcClient.bottom = rcClient.top + nMaxCanvasHeight;
	else{
		rcClient.bottom = rcClient.top + m_nMaxListHeight;
		if (nMaxCanvasHeight > m_nMaxListHeight)
			m_ctrList.ShowVScrollBar(TRUE);
	}

	if (bShowIt)
		m_ctrList.ShowWindow(SW_SHOW);
	else {
		m_ctrList.ShowWindow(SW_HIDE);
		Invalidate(TRUE);
	}

	ClientToScreen(&rcClient);
	m_ctrList.MoveWindow(&rcClient, FALSE);
}

void CAGComboBox::SetListMaxHeight(int nMaxHeight)
{
	m_nMaxListHeight = nMaxHeight;
}

int CAGComboBox::AddString(LPCTSTR lpszString)
{
	return m_ctrList.AddString(lpszString);
}

int CAGComboBox::InsertString(int nIndex, LPCTSTR lpszString)
{
	return m_ctrList.InsertString(nIndex, lpszString);
}

int CAGComboBox::SetCurSel(int nSelect)
{
	CString str;
	int nCurSel = m_ctrList.SetCurSel(nSelect);

	GetLBText(nCurSel, str);
	Invalidate(TRUE);

	return nCurSel;
}

int CAGComboBox::DeleteString(int nIndex)
{
	int nCurSel = m_ctrList.GetCurSel();
	int nCurCount = m_ctrList.DeleteString(nIndex);

	if (nCurSel == nIndex)
		Invalidate(TRUE);

	return nCurCount;
}

void CAGComboBox::ResetContent()
{
	m_ctrList.ResetContent();
	Invalidate(TRUE);
}

BOOL CAGComboBox::GetDroppedState() const
{
	return m_ctrList.IsWindowVisible();
}

int CAGComboBox::GetListMaxHeight() const
{
	return m_nMaxListHeight;
}

BOOL CAGComboBox::SetButtonImage(UINT nIDButton, int cx, int cy, COLORREF crMask)
{
	CBitmap bmpButton;

	if (!bmpButton.LoadBitmap(nIDButton))
		return FALSE;
	
	m_imgDropBtn.DeleteImageList();
	m_imgDropBtn.Create(cx, cy, ILC_COLOR24 | ILC_MASK, 4, 1);
	m_imgDropBtn.Add(&bmpButton, crMask);

	bmpButton.DeleteObject();

	Invalidate(FALSE);

	return TRUE;
}

void CAGComboBox::SetFaceColor(COLORREF crBorder, COLORREF crBack)
{
	m_crBorder = crBorder;
	m_crBack = crBack;

	Invalidate();
}

void CAGComboBox::SetTextColor(COLORREF crNormal, COLORREF crHot)
{
	m_crTextNormal = crNormal;
	m_crTextHot = crHot;
}

void CAGComboBox::SetFont(CFont* pFont, BOOL bRedraw)
{
	ASSERT(pFont != NULL);

	m_lpWndFont = pFont;
	m_ctrList.SetFont(m_lpWndFont, bRedraw);

	Invalidate(bRedraw);
}

// CAGComboBox 消息处理程序

void CAGComboBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CString		str;
	CRect		rcClient;
	CRect		rcText;
	CPoint		ptButton;
	IMAGEINFO	imgInfo;
	CFont		*lpOldFont = NULL;

	GetClientRect(&rcClient);
	rcText.CopyRect(&rcClient);
	rcText.left += 10;
	rcText.right -= (rcClient.Height() + 15);

	dc.SetBkMode(TRANSPARENT);
	COLORREF crOldColor = dc.SetBkColor(m_crBack);

	if (m_imgDropBtn.GetSafeHandle() != NULL) {
		m_imgDropBtn.GetImageInfo(0, &imgInfo);

        int nImageHeight = imgInfo.rcImage.bottom - imgInfo.rcImage.top;
		ptButton.x = rcClient.Width() - imgInfo.rcImage.right - 1;
        ptButton.y = (rcClient.Height() - nImageHeight) / 2;
		m_imgDropBtn.Draw(&dc, m_nBtnStat, ptButton, ILD_NORMAL);
	}

	if (m_lpWndFont != NULL)
		lpOldFont = dc.SelectObject(m_lpWndFont);

	if (m_nBtnStat == AGCBXBTN_NORMAL || m_nBtnStat == AGCBXBTN_DISABLE)
		dc.SetTextColor(m_crTextNormal);
	else
		dc.SetTextColor(m_crTextHot);

	GetLBText(m_ctrList.GetCurSel(), str);
	dc.DrawText(str, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

	if (lpOldFont != NULL)
		dc.SelectObject(lpOldFont);
}

void CAGComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect	rcClient;
	UINT	nBtnStat = AGCBXBTN_NORMAL;

	if (!GetDroppedState())
		nBtnStat = AGCBXBTN_HOVER;
	
	if (m_nBtnStat != nBtnStat)
		Invalidate(FALSE);

	m_nBtnStat = nBtnStat;

	if (!m_bMouseTrack) {

		TRACKMOUSEEVENT tmEvent;

		tmEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		tmEvent.dwFlags = TME_LEAVE;
		tmEvent.hwndTrack = GetSafeHwnd();
		tmEvent.dwFlags = TME_LEAVE;
		tmEvent.dwHoverTime = 1;
		::TrackMouseEvent(&tmEvent);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CAGComboBox::OnMouseLeave()
{
	UINT nBtnStat = AGCBXBTN_NORMAL;

	m_bMouseTrack = FALSE;
	if (!GetDroppedState())
		nBtnStat = AGCBXBTN_NORMAL;

	if (m_nBtnStat != nBtnStat)
		Invalidate(FALSE);

	m_nBtnStat = nBtnStat;

	CWnd::OnMouseLeave();
}

void CAGComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	BOOL bDropped = GetDroppedState();

	if (bDropped) {
		m_nBtnStat = AGCBXBTN_NORMAL;
	}
	else {
		m_nBtnStat = AGCBXBTN_PUSH;
	}

	ShowDropDown(!bDropped);

	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CAGComboBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect		rcClient;
	CRect		rcBack;
	
	GetClientRect(&rcClient);
	rcBack.CopyRect(&rcClient);
	rcBack.left += 1;
	rcBack.right -= 1;
	rcBack.top += 1;
	rcBack.bottom -= 1;

	pDC->SetBkMode(TRANSPARENT);
	COLORREF crOldColor = pDC->SetBkColor(m_crBack);

	CPen *lpOldPen = pDC->SelectObject(&m_penBorder);
	
    if ( (GetStyle()&WS_BORDER) != 0)
        pDC->Rectangle(&rcClient);

	pDC->FillSolidRect(&rcBack, m_crBack);

	return TRUE;
}


