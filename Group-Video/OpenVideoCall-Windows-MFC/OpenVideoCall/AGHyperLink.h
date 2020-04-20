#pragma once


// CAGHyperLink

class CAGHyperLink : public CStatic
{
	DECLARE_DYNAMIC(CAGHyperLink)

public:
	CAGHyperLink();
	virtual ~CAGHyperLink();

	void SetURL(CString strURL);
	CString GetURL() const;

	void SetTextColor(COLORREF crLink, COLORREF crVisited, COLORREF crHover);
	void SetBackColor(COLORREF crBack);
	COLORREF GetLinkColor() const;
	COLORREF GetVisitedColor() const;
	COLORREF GetHoverColor() const;

protected:
	virtual void PreSubclassWindow();
	
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnStnClicked();
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_crLink;
	COLORREF m_crVisited;     // Hyperlink colours
	COLORREF m_crHover;       // Hover colour
	COLORREF m_crBack;
	BOOL	 m_bOverControl;
	BOOL     m_bVisited;                            // Has it been visited?
	BOOL	 m_bTrackMouseEvent;
	CString  m_strURL;                              // hyperlink URL
};


