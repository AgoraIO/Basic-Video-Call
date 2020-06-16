// AGButton.cpp : 实现文件
//

#include "stdafx.h"
#include "AGButton.h"


// CAGButton

IMPLEMENT_DYNAMIC(CAGButton, CButton)

CAGButton::CAGButton()
{	
    m_crBorderNormal = RGB(0, 160, 239);
    m_crBorderHover = RGB(0, 160, 239);
    m_crBorderPush = RGB(0, 160, 239);
    m_crBorderDisable = RGB(0, 160, 239);

	m_crBackNormal = RGB(0, 160, 239);
	m_crBackHover = RGB(0, 160, 239);
	m_crBackPush = RGB(0, 160, 239);
	m_crBackDisable = RGB(0, 160, 239);

	m_crTextNormal = RGB(0xFF, 0xFF, 0xFF);
	m_crTextHover = RGB(0xFF, 0xC8, 0x64);
	m_crTextPush = RGB(0xFF, 0xC8, 0x64);
	m_crTextDisable = RGB(0xCC, 0xCC, 0xCC);

	m_bHover = FALSE;
	m_bMouseTrack = FALSE;
	m_bFrameEffect = TRUE;

	m_nDefStatus = AGBTN_NORMAL;
}

CAGButton::~CAGButton()
{
	m_imgBack.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CAGButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CAGButton 消息处理程序

BOOL CAGButton::SetBackImage(UINT nIDResource, COLORREF crMask)
{
	CBitmap bmBackImage;
	BITMAP  bmImageInfo;

	if (!bmBackImage.LoadBitmap(nIDResource))
		return FALSE;

	bmBackImage.GetBitmap(&bmImageInfo);

	m_imgBack.DeleteImageList();
	if (!m_imgBack.Create(bmImageInfo.bmWidth / 4, bmImageInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 4, 1))
		return FALSE;

	m_imgBack.Add(&bmBackImage, crMask);
	bmBackImage.DeleteObject();

	Invalidate(FALSE);

	return TRUE;
}

void CAGButton::SetBackColor(COLORREF crNormal, COLORREF crHover, COLORREF crPush, COLORREF crDisable)
{
	m_crBackNormal = crNormal;
	m_crBackHover = crHover;
	m_crBackPush = crPush;
	m_crBackDisable = crDisable;

	Invalidate(FALSE);
}

void CAGButton::SetTextColor(COLORREF crNormal, COLORREF crHover, COLORREF crPush, COLORREF crDisable)
{
	m_crTextNormal = crNormal;
	m_crTextHover = crHover;
	m_crTextPush = crPush;
	m_crTextDisable = crDisable;

	Invalidate(FALSE);
}

void CAGButton::SetBorderColor(COLORREF crNormal, COLORREF crHover, COLORREF crPush, COLORREF crDisable)
{
    m_crBorderNormal = crNormal;
    m_crBorderHover = crHover;
    m_crBorderPush = crPush;
    m_crBorderDisable = crDisable;
    
    Invalidate(FALSE);
}

void CAGButton::EnableFrameEffect(BOOL bEnable)
{
	m_bFrameEffect = bEnable;
	Invalidate(FALSE);
}

void CAGButton::PreSubclassWindow()
{
	// TODO:  在此添加专用代码和/或调用基类
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}
 


void CAGButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	UINT nStat = m_nDefStatus;
	UINT uStyle = DFCS_BUTTONPUSH;

    if (m_bHover && m_nDefStatus == AGBTN_NORMAL)
		nStat = AGBTN_HOVER;

	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		uStyle |= DFCS_PUSHED;
		nStat = AGBTN_PUSH;
	}
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		nStat = AGBTN_DISABLE;

	DrawBack(lpDrawItemStruct, nStat);
	DrawText(lpDrawItemStruct, nStat);

	if (m_bFrameEffect)
		DrawFrame(lpDrawItemStruct, nStat);
}

void CAGButton::DrawBack(LPDRAWITEMSTRUCT lpDrawItemStruct, UINT nStat)
{
	int		 nImageIndex = -1;
	COLORREF crBackColor = m_crBackNormal;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	switch (nStat)
	{
	case AGBTN_NORMAL:
		nImageIndex = 0;
		crBackColor = m_crBackNormal;
		break;
	case AGBTN_HOVER:
		nImageIndex = 1;
		crBackColor = m_crBackHover;
		break;
	case AGBTN_PUSH:
		nImageIndex = 2;
		crBackColor = m_crBackPush;
		break;
	case AGBTN_DISABLE:
		nImageIndex = 3;
		crBackColor = m_crBackDisable;
		break;
	default:
		nImageIndex = 0;
		crBackColor = m_crBackNormal;
		break;
	}

	if (m_imgBack.GetSafeHandle() == NULL || nImageIndex >= m_imgBack.GetImageCount())
		nImageIndex = -1;

    CRect rcButton;
    CBrush brushBack;

    rcButton.CopyRect(&lpDrawItemStruct->rcItem);
    brushBack.CreateSolidBrush(crBackColor);
	pDC->FillRect(&rcButton, &brushBack);

    brushBack.DeleteObject();

	if (nImageIndex != -1)
		m_imgBack.Draw(pDC, nImageIndex, CPoint(0, 0), ILD_NORMAL);
}

void CAGButton::DrawText(LPDRAWITEMSTRUCT lpDrawItemStruct, UINT nStat)
{
	CRect		rcText;
	CString		strText;

	GetWindowText(strText);
	rcText.CopyRect(&lpDrawItemStruct->rcItem);

	COLORREF crTextColor = m_crTextNormal;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	switch (nStat)
	{
	case AGBTN_NORMAL:
		crTextColor = m_crTextNormal;
		break;
	case AGBTN_HOVER:
		crTextColor = m_crTextHover;
		break;
	case AGBTN_PUSH:
		crTextColor = m_crTextPush;
		rcText.left += 1;
		rcText.bottom += 1;
		break;
	case AGBTN_DISABLE:
		crTextColor = m_crTextDisable;
		break;
	default:
		crTextColor = m_crTextDisable;
		break;
	}

	COLORREF crOldColor = pDC->SetTextColor(crTextColor);
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->SetTextColor(crOldColor);
}

void CAGButton::DrawFrame(LPDRAWITEMSTRUCT lpDrawItemStruct, UINT nStat)
{
    CBrush brushBorder;

    CRect rcFocus(&lpDrawItemStruct->rcItem);
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    rcFocus.left += 3;
    rcFocus.right -= 3;

    rcFocus.top += 3;
    rcFocus.bottom -= 3;

    switch (nStat)
    {
    case AGBTN_NORMAL:
        brushBorder.CreateSolidBrush(m_crBorderNormal);
		pDC->FrameRect(&lpDrawItemStruct->rcItem, &brushBorder);
    	break;
    case AGBTN_HOVER:
        brushBorder.CreateSolidBrush(m_crBorderHover);
		pDC->DrawEdge(&lpDrawItemStruct->rcItem, BDR_RAISEDOUTER, BF_RECT);
        break;
    case AGBTN_PUSH:
        brushBorder.CreateSolidBrush(m_crBorderPush);
		pDC->DrawEdge(&lpDrawItemStruct->rcItem, BDR_SUNKENINNER, BF_RECT);
        break;
    case AGBTN_DISABLE:
        brushBorder.CreateSolidBrush(m_crBorderDisable);
		pDC->FrameRect(&lpDrawItemStruct->rcItem, &brushBorder);
        break;

    default:
        break;
    }

    if (lpDrawItemStruct->itemState & ODS_FOCUS)
        pDC->DrawFocusRect(&rcFocus);
}

void CAGButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!m_bHover){
		m_bHover = TRUE;
		Invalidate(FALSE);
	}

	if (!m_bMouseTrack) {

		TRACKMOUSEEVENT tmEvent;

		tmEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		tmEvent.dwFlags = TME_LEAVE;
		tmEvent.hwndTrack = GetSafeHwnd();
		tmEvent.dwFlags = TME_LEAVE;
		tmEvent.dwHoverTime = 1;
		::TrackMouseEvent(&tmEvent);
	}

	CButton::OnMouseMove(nFlags, point);
}


void CAGButton::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bHover = FALSE;
	m_bMouseTrack = FALSE;
	Invalidate(FALSE);

	CButton::OnMouseLeave();
}

BOOL CAGButton::OnEraseBkgnd(CDC* pDC)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    return TRUE;
//    return CButton::OnEraseBkgnd(pDC);
}
