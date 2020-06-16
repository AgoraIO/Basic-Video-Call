// AGSliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "AGSliderCtrl.h"


// CAGSliderCtrl

IMPLEMENT_DYNAMIC(CAGSliderCtrl, CSliderCtrl)

CAGSliderCtrl::CAGSliderCtrl()
: m_dwChannelHeight(2)
, m_nThumbHeight(0)
, m_nThumbWidth(0)
, m_bHover(FALSE)
{
	m_brChannel.CreateSolidBrush(RGB(0xC8, 0xC8, 0xC8));
	m_brChannelHot.CreateSolidBrush(RGB(0x00, 0xA0, 0xEB));
	m_brBack.CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
}

CAGSliderCtrl::~CAGSliderCtrl()
{
	m_brChannel.DeleteObject();
	m_brChannelHot.DeleteObject();
	m_brBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGSliderCtrl, CSliderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CAGSliderCtrl::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CAGSliderCtrl 消息处理程序
BOOL CAGSliderCtrl::SetThumbBitmap(UINT nIDNormal, UINT nIDHover, UINT nIDPush, COLORREF crMaskColor)
{
	CBitmap bmpMap;
	BITMAP  bmpInfo;
	BOOL	bSuccess = FALSE;

	bSuccess = bmpMap.LoadBitmap(nIDNormal);
	if (!bSuccess)
		return FALSE;

	bmpMap.GetBitmap(&bmpInfo);
	m_imgThumb.DeleteImageList();
	m_imgThumb.Create(bmpInfo.bmWidth, bmpInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 3, 1);
	m_nThumbHeight = bmpInfo.bmHeight;
	m_nThumbWidth = bmpInfo.bmWidth;

	m_imgThumb.Add(&bmpMap, crMaskColor);
	if (nIDHover != 0) {
		bmpMap.DeleteObject();
		bSuccess = bmpMap.LoadBitmap(nIDHover);
	}

	if (bSuccess)
		m_imgThumb.Add(&bmpMap, crMaskColor);
	if (nIDPush != 0) {
		bmpMap.DeleteObject();
		bSuccess = bmpMap.LoadBitmap(nIDPush);
	}

	if (bSuccess)
		m_imgThumb.Add(&bmpMap, crMaskColor);

	bmpMap.DeleteObject();
	
	return bSuccess;
}

BOOL CAGSliderCtrl::SetChannelColor(COLORREF crNormal, COLORREF crHot)
{
	m_brChannel.DeleteObject();
	m_brChannelHot.DeleteObject();

	m_brChannel.CreateSolidBrush(crNormal);
	m_brChannelHot.CreateSolidBrush(crHot);

	return TRUE;
}
BOOL CAGSliderCtrl::SetBkColor(COLORREF crBkColor)
{
	m_brBack.DeleteObject();

	return m_brBack.CreateSolidBrush(crBkColor);
}

void CAGSliderCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW lpCustomDraw = (LPNMCUSTOMDRAW)pNMHDR;
	CDC *pDC = CDC::FromHandle(lpCustomDraw->hdc);
	
	switch (lpCustomDraw->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
	{
		switch (lpCustomDraw->dwItemSpec)
		{
		case TBCD_TICS:
			*pResult = CDRF_DODEFAULT;
			break;
		case TBCD_THUMB:
			*pResult = OnDrawThumb(pDC, lpCustomDraw);
			break;
		case TBCD_CHANNEL:
			*pResult = OnDrawChannel(pDC, lpCustomDraw);
			break;
		}
		break;
	}
	}
}

LRESULT CAGSliderCtrl::OnDrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpCustomDraw)
{
	if (m_imgThumb.GetSafeHandle() == NULL || m_imgThumb.GetImageCount() == 0)
		return CDRF_DODEFAULT;

	CRect rcThumb;
	GetThumbRect(&rcThumb);
	CPoint ptThumb = rcThumb.CenterPoint();
	ptThumb.Offset(-m_nThumbWidth/2, -m_nThumbHeight/2);

	if ( (lpCustomDraw->uItemState&CDIS_SELECTED) || m_bHover )
		m_imgThumb.Draw(pDC, 2, ptThumb, ILD_NORMAL);
	else
		m_imgThumb.Draw(pDC, 0, ptThumb, ILD_NORMAL);

	Invalidate(FALSE);
	lpCustomDraw->rc.left = ptThumb.x;
	lpCustomDraw->rc.top = ptThumb.y;
	lpCustomDraw->rc.right = ptThumb.x + m_nThumbWidth;
	lpCustomDraw->rc.right = ptThumb.y + m_nThumbHeight;

	return CDRF_SKIPDEFAULT;
}

LRESULT CAGSliderCtrl::OnDrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpCustomDraw)
{
	CRect rcThumb;
	CRect rcChannel;
	CRect rcDraw;

	GetClientRect(&rcDraw);
	GetThumbRect(&rcThumb);
	GetChannelRect(&rcChannel);

	pDC->FillRect(&rcDraw, &m_brBack);

	rcChannel.top = rcThumb.CenterPoint().y - m_dwChannelHeight / 2;
	rcChannel.bottom = rcThumb.CenterPoint().y + m_dwChannelHeight / 2;

	rcDraw.CopyRect(&rcChannel);
	rcDraw.right = rcThumb.left;
	pDC->FillRect(&rcDraw, &m_brChannelHot);

	rcDraw.CopyRect(&rcChannel);
	rcDraw.left = rcThumb.right;
	pDC->FillRect(&rcDraw, &m_brChannel);

	return CDRF_SKIPDEFAULT;
}


void CAGSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcThumbRect;
	BOOL  bLastState = m_bHover;

	GetThumbRect(&rcThumbRect);
	if (rcThumbRect.PtInRect(point))
		m_bHover = TRUE;
	else
		m_bHover = FALSE;

	if (bLastState != m_bHover)
		InvalidateRect(rcThumbRect);

	CSliderCtrl::OnMouseMove(nFlags, point);
}

void CAGSliderCtrl::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bHover = FALSE;

	CRect rcThumb;
	GetThumbRect(&rcThumb);
	InvalidateRect(rcThumb);

	CSliderCtrl::OnMouseLeave();
}