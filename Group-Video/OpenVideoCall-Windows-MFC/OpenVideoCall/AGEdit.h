#pragma once


// CAGEdit

class CAGEdit : public CEdit
{
	DECLARE_DYNAMIC(CAGEdit)

public:
	CAGEdit();
	virtual ~CAGEdit();

    void SetTip(LPCTSTR lpTip);
    CString GetTip() { return m_strTip; };
    void SetColor(COLORREF crBorder, COLORREF crBack, COLORREF crText, COLORREF crTip = RGB(0xD8, 0xD8, 0xD8));

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
    
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEnChange();

	DECLARE_MESSAGE_MAP()

private:
    CString     m_strTip;

	COLORREF	m_crBorder;
	COLORREF	m_crBack;
	COLORREF	m_crText;
    COLORREF    m_crTip;

	CPen		m_penBorder;
	CBrush		m_brushBack;

	BOOL		m_bTrackMouseEvent;
    BOOL		m_bOverControl;

    BOOL        m_bEmpty;
    BOOL        m_bTexting;
};


