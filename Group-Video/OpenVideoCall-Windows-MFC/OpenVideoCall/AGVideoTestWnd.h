#pragma once


// CAGVideoTestWnd

class CAGVideoTestWnd : public CWnd
{
	DECLARE_DYNAMIC(CAGVideoTestWnd)

public:
	CAGVideoTestWnd();
	virtual ~CAGVideoTestWnd();

	HWND GetVideoSafeHwnd() { return m_wndVideoWnd.GetSafeHwnd(); };

	// 音量条指示

	void SetVolbarColor(DWORD dwFreeColor = RGB(184, 184, 184), DWORD dwBusyColor = RGB(0, 255, 0), DWORD dwBackColor = RGB(0, 0, 0));	// 设定空闲颜色
	void SetVolRange(int nRange = 100);
	void SetCurVol(int nCurVol = 0);

protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

private:
	CWnd		m_wndVideoWnd;	// the wnd for show video

	int			m_nVolbarWidth;

	int			m_nVolbarPos;			// the vol bar pos
	int			m_nVolRange;			// the max vol
	int			m_nCurVol;				// the current vol

	COLORREF	m_crBackColor;
	COLORREF	m_crVolbarFreeColor;
	COLORREF	m_crVolbarBusyColor;
	COLORREF	m_crVolbarBackColor;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


