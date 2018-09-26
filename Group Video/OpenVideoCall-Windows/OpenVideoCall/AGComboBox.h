#pragma once

#include <afxtempl.h>

#define WM_AGCBXNOTIFY_SELCHANGE	WM_USER+

class CAGComboBox;
// CAGComboBox
class CAGComboBoxList : public CWnd
{
	DECLARE_DYNAMIC(CAGComboBoxList)

public:
	CAGComboBoxList();
	virtual ~CAGComboBoxList();

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CAGComboBox *pAGComboBox, UINT nID);

	int GetCount() const;
	int GetCurSel() const;
	DWORD_PTR GetItemData(int nIndex) const;
	void GetText(int nIndex, CString& rString) const;

	int SetItemData(int nIndex, DWORD_PTR dwItemData);
	int AddString(LPCTSTR lpszString);
	int InsertString(int nIndex, LPCTSTR lpszString);

	int SetCurSel(int nSelect);
	int DeleteString(int nIndex);
	void ResetContent();

	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	void SetItemHeight(int nItemHeight);
	int	GetItemHeight() const;

	void ShowVScrollBar(BOOL bShow);
protected:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

protected:

	// the resource to draw the control
private:
	CImageList		m_imgSel;
	CPen			m_penBorder;
	CFont			*m_lpWndFont;

	COLORREF		m_crBack;
	COLORREF		m_crBorder;
	COLORREF		m_crText;
	COLORREF		m_crTextHot;

	// the controls
private:
	CAGComboBox		*m_lpComboBox;
	CScrollBar		m_ctrVScroll;

	// private data
private:
	CStringArray			m_arrItemString;
	CArray<DWORD_PTR>		m_arrItemPtrData;

	int						m_nItemCount;
	int						m_nCurSel;
	int						m_nCurPointIndex;
	int						m_nPerItemHeight;

public:
	
	

};

class CAGComboBox : public CWnd
{
	enum {
		AGCBXBTN_NORMAL = 0,
		AGCBXBTN_HOVER,
		AGCBXBTN_PUSH,
		AGCBXBTN_DISABLE,
	};

	DECLARE_DYNAMIC(CAGComboBox)

public:
	CAGComboBox();
	virtual ~CAGComboBox();

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	int GetCount() const;
	int GetCurSel() const;
	DWORD_PTR GetItemData(int nIndex) const;
	void GetLBText(int nIndex, CString& rString) const;

	int SetItemData(int nIndex, DWORD_PTR dwItemData);
	void ShowDropDown(BOOL bShowIt);
	void SetListMaxHeight(int nMaxHeight);

	int AddString(LPCTSTR lpszString);
	int InsertString(int nIndex, LPCTSTR lpszString);

	int SetCurSel(int nSelect);
	int DeleteString(int nIndex);
	void ResetContent();

	BOOL GetDroppedState() const;
	int GetListMaxHeight() const;

	BOOL SetButtonImage(UINT nIDButton, int cx, int cy, COLORREF crMask);
	void SetFaceColor(COLORREF crBorder, COLORREF crBack);
	void SetTextColor(COLORREF crNormal, COLORREF crHot);
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);

protected:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	DECLARE_MESSAGE_MAP()

protected:
	void OnDrawTextCtrl(CDC *pDC);
	void OnDrawBtnCtrl(CDC *pDC);

private:
	CImageList	m_imgDropBtn;
	CPen		m_penBorder;

	COLORREF		m_crBack;
	COLORREF		m_crBorder;
	
	COLORREF		m_crTextNormal;
	COLORREF		m_crTextHot;

	UINT			m_nBtnStat;
	int				m_nMaxListHeight;
	BOOL			m_bMouseTrack;

	CFont			*m_lpWndFont;

private:
	CAGComboBoxList		m_ctrList;

};


