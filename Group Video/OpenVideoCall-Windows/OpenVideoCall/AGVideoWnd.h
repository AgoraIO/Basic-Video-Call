#pragma once

#define WM_SHOWMODECHANGED	WM_USER+300
#define WM_SHOWBIG			WM_USER+301

class CAGInfoWnd : public CWnd
{
	DECLARE_DYNAMIC(CAGInfoWnd)

public:
	CAGInfoWnd();
	virtual ~CAGInfoWnd();

	void ShowTips(BOOL bShow = TRUE);
	void SetVideoResolution(int nWidth, int nHeight);
	void SetFrameRateInfo(int nFPS);
	void SetBitrateInfo(int nBitrate);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	BOOL		m_bShowTip;
	
	COLORREF	m_crBackColor;

	int		m_nWidth;
	int		m_nHeight;
	int		m_nFps;
	int		m_nBitrate;

	CBrush	m_brBack;
};


class CAGVideoWnd : public CWnd
{
	DECLARE_DYNAMIC(CAGVideoWnd)

public:
	CAGVideoWnd();
	virtual ~CAGVideoWnd();

	void SetUID(UINT dwUID);
	
	UINT GetUID();
	BOOL IsWndFree();

	void SetFaceColor(COLORREF crBackColor);
	BOOL SetBackImage(UINT nID, UINT nWidth, UINT nHeight, COLORREF crMask = RGB(0xFF, 0xff, 0xFF));

    void ShowBackground(BOOL bBackground);
    BOOL IsBackgroundMode() { return m_bBackground; };

	void SetVideoResolution(UINT nWidth, UINT nHeight);
	void GetVideoResolution(UINT *nWidth, UINT *nHeight);
	
	void SetBitrateInfo(int nReceivedBitrate);
	int	GetBitrateInfo() { return m_nBitRate; };
	
	void SetFrameRateInfo(int nReceiveFrameRate);
	int GetFrameRateInfo() { return m_nFrameRate; };

	void ShowVideoInfo(BOOL bShow);
	BOOL IsVideoInfoShowed() { return m_bShowVideoInfo; };

	void SetBigShowFlag(BOOL bBigShow);
	BOOL IsBigShow() { return m_bBigShow; };

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	CImageList		m_imgBackGround;
	COLORREF		m_crBackColor;

	CAGInfoWnd		m_wndInfo;

private:
	UINT		m_nUID;

	UINT		m_nWidth;
	UINT		m_nHeight;
	int			m_nFrameRate;
	int			m_nBitRate;
	BOOL		m_bShowVideoInfo;
	BOOL		m_bBigShow;

    BOOL        m_bBackground;
public:
    afx_msg void OnPaint();
};


