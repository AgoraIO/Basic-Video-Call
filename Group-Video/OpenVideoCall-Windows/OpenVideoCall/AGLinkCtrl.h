#pragma once
#include <afxcmn.h>

// CAGLinkCtrl

class CAGLinkCtrl : public CStatic
{
    DECLARE_DYNAMIC(CAGLinkCtrl)

public:
    CAGLinkCtrl();
    virtual ~CAGLinkCtrl();

    void SetTextColor(COLORREF crText, COLORREF crHover);
    void SetBackColor(COLORREF crBack);
    COLORREF GetTextColor() const;
    COLORREF GetHoverColor() const;

protected:
    virtual void PreSubclassWindow();

    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseLeave();

    DECLARE_MESSAGE_MAP()

private:
    COLORREF m_crText;
    COLORREF m_crHover;       // Hover colour
    COLORREF m_crBack;
    BOOL	 m_bOverControl;
    BOOL	 m_bTrackMouseEvent;
};