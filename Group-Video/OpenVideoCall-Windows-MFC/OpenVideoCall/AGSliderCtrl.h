#pragma once



// CAGSliderCtrl

class CAGSliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(CAGSliderCtrl)

public:
	CAGSliderCtrl();
	virtual ~CAGSliderCtrl();

	BOOL SetThumbBitmap(UINT nIDNormal, UINT nIDHover = 0, UINT nIDPush = 0, COLORREF crMaskColor = RGB(0xFF, 0, 0xFF));
	BOOL SetChannelColor(COLORREF crNormal = RGB(0xC8, 0xC8, 0xC8), COLORREF crHot = RGB(0x00, 0xA0, 0xEB));
	BOOL SetChannelHeight(DWORD dwChannelHeight = 2);
	BOOL SetBkColor(COLORREF crBkColor = RGB(0xFF, 0xFF, 0xFF));

protected:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

protected:
	LRESULT OnDrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpCustomDraw);
	LRESULT OnDrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpCustomDraw);

private:
	CImageList	m_imgThumb;

	CBrush		m_brBack;
	CBrush		m_brChannel;
	CBrush		m_brChannelHot;

	DWORD		m_dwChannelHeight;
	int			m_nThumbHeight;
	int			m_nThumbWidth;

	BOOL		m_bHover;
};


