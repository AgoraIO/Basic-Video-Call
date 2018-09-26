// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "AGEventDef.h"
#include "video_preprocessing_plugin.h"

// CVideoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{
	m_brHead.CreateSolidBrush(RGB(0, 160, 239));
	SetBackgroundColor(RGB(0x26, 0x26, 0x26));
	m_bLastStat = FALSE;
	m_nScreenMode = SCREEN_VIDEO1;
	m_bFullScreen = FALSE;
	m_lpBigShowed = NULL;
    m_bFilter = FALSE;
    m_bShowInfo = FALSE;
}

CVideoDlg::~CVideoDlg()
{
	m_brHead.DeleteObject();
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTNMIN_VIDEO, m_btnMin);
	DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
	DDX_Control(pDX, IDC_BTNCLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()

	ON_MESSAGE(WM_SHOWMODECHANGED, &CVideoDlg::OnShowModeChanged)
	ON_MESSAGE(WM_SHOWBIG, &CVideoDlg::OnShowBig)

	ON_MESSAGE(WM_WINDOWSHARE, &CVideoDlg::OnWindowShareStart)
	ON_MESSAGE(WM_DESKTOPSHARE, &CVideoDlg::OnDesktopShareStart)

	ON_MESSAGE(WM_MSGID(EID_JOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_REJOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDReJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), &CVideoDlg::OnEIDFirstLocalFrame)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CVideoDlg::OnEIDFirstRemoteFrameDecoded)
	ON_MESSAGE(WM_MSGID(EID_USER_OFFLINE), &CVideoDlg::OnEIDUserOffline)
	
	ON_MESSAGE(WM_MSGID(EID_REMOTE_VIDEO_STAT), &CVideoDlg::OnRemoteVideoStat)

	ON_MESSAGE(WM_MSGID(EID_START_RCDSRV), &CVideoDlg::OnStartRecordingService)
	ON_MESSAGE(WM_MSGID(EID_STOP_RCDSRV), &CVideoDlg::OnStopRecordingService)

    ON_MESSAGE(WM_MSGID(EID_STREAM_MESSAGE), &CVideoDlg::OnStreamMessage)


	ON_BN_CLICKED(IDC_BTNMIN_VIDEO, &CVideoDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE_VIDEO, &CVideoDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNRSTO_VIDEO, &CVideoDlg::OnBnClickedBtnrest)
	ON_BN_CLICKED(IDC_BTNENDCALL_VIDEO, &CVideoDlg::OnBnClickedBtnclose)

    ON_BN_CLICKED(IDC_BTNMESSAGE_VIDEO, &CVideoDlg::OnBnClickedBtnmessage)

	ON_BN_CLICKED(IDC_BTNSCRCAP_VIDEO, &CVideoDlg::OnBnClickedBtnScreenCapture)
    ON_BN_CLICKED(IDC_BTNMORE_VIDEO, &CVideoDlg::OnBnClickedBtnMore)
	ON_BN_CLICKED(IDC_BTNMODE_VIDEO, &CVideoDlg::OnBnClickedBtnmode)
	ON_BN_CLICKED(IDC_BTNAUDIO_VIDEO, &CVideoDlg::OnBnClickedBtnaudio)

    ON_BN_CLICKED(ID_IDR_VIDEOINFO, &CVideoDlg::OnBnClickedBtntip)
    ON_BN_CLICKED(ID_IDR_DEVICE, &CVideoDlg::OnBnClickedBtnsetup)
    ON_BN_CLICKED(ID_IDR_FILTER, &CVideoDlg::OnBnClickedBtnfilter)

//	ON_BN_CLICKED(IDC_BTNWHITEBOARD_VIDEO, &CVideoDlg::OnBnCliekedBtnWhiteBoard)
//	ON_BN_CLICKED(IDC_BTNCLOSEWB_VIDEO, &CVideoDlg::OnBnCliekedBtnCloseWhiteBoard)

	ON_BN_CLICKED(IDC_BTNSCR_VIDEO, &CVideoDlg::OnBnClickedBtnfullscr)
	
	ON_BN_CLICKED(ID_SCRSHARE_DESKTOPSHARE, &CVideoDlg::OnBnClickedScreenshare)
	ON_BN_CLICKED(ID_SCRSHARE_WINDOWSHARE, &CVideoDlg::OnBnClickedWindowshare)

//	ON_BN_CLICKED(ID_WHITEBOARD_HOSTMODE, &CVideoDlg::OnBnClickedHostMode)
//	ON_BN_CLICKED(ID_WHITEBOARD_GUESTMODE, &CVideoDlg::OnBnClickedGuestMode)
	ON_WM_SHOWWINDOW()
    ON_WM_MOVE()
END_MESSAGE_MAP()


// CVideoDlg 消息处理程序


void CVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_btnMin.GetSafeHwnd() != NULL)
		m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
	if (m_btnRst.GetSafeHwnd() != NULL)
		m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
	if (m_btnClose.GetSafeHwnd() != NULL)
		m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

	GetClientRect(&m_rcVideoArea);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;

	// 2人， 右上角子画面区域
	m_rcChildVideoArea.top = m_rcVideoArea.top + 10;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 14;
	m_rcChildVideoArea.left = m_rcChildVideoArea.right - 192;

    if (::IsWindow(m_dlgChat.GetSafeHwnd()) && m_dlgChat.IsWindowVisible()) {
        CRect   rcChatArea;

        rcChatArea.CopyRect(&m_rcVideoArea);
        rcChatArea.top = rcChatArea.bottom - 150;
        ClientToScreen(&rcChatArea);
        m_dlgChat.MoveWindow(&rcChatArea);
    }

	if (m_wndLocal.GetSafeHwnd() == NULL || m_wndVideo[0].GetSafeHwnd() == NULL)
		return;

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		AdjustSizeVideo1(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEO4:
		AdjustSizeVideo4(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEOMULTI:
		AdjustSizeVideoMulti(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	default:
		break;
	}

	Invalidate(TRUE);
}

void CVideoDlg::ShowButtonsNormal()
{
	CRect	rcClient;
	int nShowMode = SW_HIDE;

	if (!m_bFullScreen)
		nShowMode = SW_SHOW;

    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	
	m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);
	m_btnShow.ShowWindow(nShowMode);

	GetClientRect(&rcClient);
	AdjustButtonsNormal(rcClient.Width(), rcClient.Height());
}


void CVideoDlg::AdjustButtonsNormal(int cx, int cy)
{
//	if (m_btnSetup.GetSafeHwnd() != NULL)
//		m_btnSetup.MoveWindow(30, cy - 48, 24, 24, TRUE);

    if(m_btnMessage.GetSafeHwnd() != NULL)
        m_btnMessage.MoveWindow(cx / 2 - 312, cy - 60, 48, 48, TRUE);
	if (m_btnMode.GetSafeHwnd() != NULL)
		m_btnMode.MoveWindow(cx / 2 - 216, cy - 60, 48, 48, TRUE);
	if (m_btnAudio.GetSafeHwnd() != NULL)
		m_btnAudio.MoveWindow(cx / 2 - 120, cy - 60, 48, 48, TRUE);
	if (m_btnEndCall.GetSafeHwnd() != NULL)
		m_btnEndCall.MoveWindow(cx / 2 - 24, cy - 60, 48, 48, TRUE);

	if (m_btnScrCap.GetSafeHwnd() != NULL)
		m_btnScrCap.MoveWindow(cx / 2 + 72, cy - 60, 48, 48, TRUE);
    if (m_btnMore.GetSafeHwnd() != NULL)
        m_btnMore.MoveWindow(cx / 2 + 264, cy - 60, 48, 48, TRUE);

	if (m_btnShow.GetSafeHwnd() != NULL)
		m_btnShow.MoveWindow(cx - 72, cy - 48, 48, 48, TRUE);
}

void CVideoDlg::AdjustSizeVideo1(int cx, int cy)
{
	if (m_listWndInfo.GetCount() == 0)
		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
	else {
		m_wndVideo[0].MoveWindow(&m_rcVideoArea, FALSE);
		m_wndLocal.MoveWindow(&m_rcChildVideoArea, FALSE);
	}
}

void CVideoDlg::AdjustSizeVideo4(int cx, int cy)
{
	m_wndVideo[0].MoveWindow(0, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[1].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[2].MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[3].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);

	if (m_listWndInfo.GetCount() == 2)
		m_wndLocal.MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	else
		m_wndLocal.MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
}

void CVideoDlg::AdjustSizeVideoMulti(int cx, int cy)
{
	int nLocalIndex = 0;

	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);

	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.top + 16;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, 192, 144, TRUE);
				nLocalIndex++;
			}
		}
		else{
			m_wndLocal.MoveWindow(nXPos, nYPos, 192, 144, TRUE);
			nLocalIndex++;
		}
	}
}


void CVideoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CVideoDlg::NeedShowSizeBox(CPoint point)
{
	CRect rcClient;

	GetClientRect(&rcClient);

	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 5;
	rcClient.bottom -= 5;
	
	if (rcClient.PtInRect(point) || IsZoomed())
		return FALSE;

	return TRUE;
}

void CVideoDlg::EnableSize(BOOL bEnable)
{
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if (bEnable)
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle | WS_SIZEBOX);
	else
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle&(~WS_SIZEBOX));

	CRect rcClient;

	if (bEnable)
		GetWindowRect(&rcClient);
	else
		GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	if (m_bLastStat != bEnable) {
		m_bLastStat = bEnable;
		if (m_btnMin.GetSafeHwnd() != NULL)
			m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
		if (m_btnRst.GetSafeHwnd() != NULL)
			m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
		if (m_btnClose.GetSafeHwnd() != NULL)
			m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

//		if (m_btnTip.GetSafeHwnd() != NULL)
//			m_btnTip.MoveWindow(cx / 2 - 372, cy - 84, 72, 72, TRUE);

		if (m_btnScrCap.GetSafeHwnd() != NULL)
			m_btnScrCap.MoveWindow(48, cy - 84, 48, 48, TRUE);

//		if (m_btnSetup.GetSafeHwnd() != NULL)
//			m_btnSetup.MoveWindow(cx / 2 - 216, cy - 84, 72, 72, TRUE);
		if (m_btnMode.GetSafeHwnd() != NULL)
			m_btnMode.MoveWindow(cx / 2 - 96, cy - 84, 48, 48, TRUE);
		if (m_btnAudio.GetSafeHwnd() != NULL)
			m_btnAudio.MoveWindow(cx / 2 + 24, cy - 84, 48, 48, TRUE);
		if (m_btnShow.GetSafeHwnd() != NULL)
            m_btnShow.MoveWindow(cx - 72, cy - 84, 48, 48, TRUE);
		if (m_btnEndCall.GetSafeHwnd() != NULL)
			m_btnEndCall.MoveWindow(cx - 120, cy - 84, 48, 48, TRUE);

		Invalidate(FALSE);
	}
}

void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	DrawHead(&dc);
}


LRESULT CVideoDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CVideoDlg::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}

void CVideoDlg::OnBnClickedBtnclose()
{
	GetParent()->SendMessage(WM_LEAVECHANNEL, 0, 0);

	m_listWndInfo.RemoveAll();
    m_dlgChat.ShowWindow(SW_HIDE);
    m_dlgDevice.ShowWindow(SW_HIDE);

    // unmute local video
    CAgoraObject::GetAgoraObject()->MuteLocalVideo(FALSE);
    m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO);

    // unmute local audio
    CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);
    m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

    CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, 0, NULL, FALSE);
    m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

    m_dlgChat.ShowWindow(SW_HIDE);
    m_dlgChat.ClearHistory();
    m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	CDialogEx::OnOK();
}

void CVideoDlg::OnBnClickedBtnrest()
{
	if (IsZoomed()) {
		m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_RESTORE);
	}
	else{
		m_btnRst.SetBackImage(IDB_BTNRESTORE, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_MAXIMIZE);
	}

	CRect	rcClient;

	GetClientRect(&rcClient);
	m_rcVideoArea.CopyRect(&rcClient);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;
	  
	m_rcChildVideoArea.top = m_rcVideoArea.top + 16;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.left = m_rcVideoArea.right - 212;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 20;

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		AdjustSizeVideo1(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEO4:
		AdjustSizeVideo4(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	case SCREEN_VIDEOMULTI:
		AdjustSizeVideoMulti(cx, cy);
		AdjustButtonsNormal(cx, cy);
		break;
	default:
		break;
	}

	Invalidate(TRUE);
}


void CVideoDlg::OnBnClickedBtnfullscr()
{
	int nShowMode = 0;

	if (m_bFullScreen) {
		m_bFullScreen = FALSE;
		nShowMode = SW_SHOW;		
		ShowWindow(SW_RESTORE);

		GetClientRect(&m_rcVideoArea);
		m_rcVideoArea.top += 24;
		m_rcVideoArea.bottom -= 72;	
	}
	else{
		m_bFullScreen = TRUE;
		nShowMode = SW_HIDE;
		ShowWindow(SW_MAXIMIZE);

		GetClientRect(&m_rcVideoArea);
	}

	m_btnMin.ShowWindow(nShowMode);
	m_btnRst.ShowWindow(nShowMode);
	m_btnClose.ShowWindow(nShowMode);

    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	
	m_btnShow.ShowWindow(nShowMode);

	switch (m_nScreenMode)
	{
	case SCREEN_VIDEO1:
		ShowVideo1();
		break;
	case SCREEN_VIDEO4:
		ShowVideo4();
		break;
	case SCREEN_VIDEOMULTI:
		ShowMulti();
		break;
	default:
		break;
	}

	Invalidate(TRUE);
}

void CVideoDlg::OnBnClickedScreenshare()
{
	m_dlgDesktopCapture.SaveScreen(NULL);
	m_dlgDesktopCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::OnBnClickedWindowshare()
{
	m_dlgScreenCapture.RefreashWndInfo();
	m_dlgScreenCapture.SaveScreen(NULL);

	m_dlgScreenCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::ShowControlButton(BOOL bShow)
{
	int nShowMode = 0;
	if (bShow)
		nShowMode = SW_SHOW;
	else
		nShowMode = SW_HIDE;

	m_btnScrCap.ShowWindow(nShowMode);
    m_btnMessage.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
    m_btnMore.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnShow.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
}

void CVideoDlg::OnBnClickedBtntip()
{
	for (int nIndex = 0; nIndex < 4; nIndex++)
        m_wndVideo[nIndex].ShowVideoInfo(!m_bShowInfo);

    m_bShowInfo = !m_bShowInfo;
}

void CVideoDlg::OnBnClickedBtnScreenCapture()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
	
	if (lpAgora->IsScreenCaptureEnabled()) {
		lpAgora->EnableScreenCapture(NULL, 0, NULL, FALSE);
		m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
        CAgoraObject::GetAgoraObject()->EnableLocalRender(TRUE);
	}
	else
		CreateScreenShareMenu();

	Invalidate();
}

void CVideoDlg::OnBnClickedBtnMore()
{
    CreateMoreMenu();
}

void CVideoDlg::CreateScreenShareMenu()
{
	CRect				rcButton;
	CMenu				Menu;
	MENUITEMINFO		mii;

	Menu.LoadMenu(IDR_MENU_SCRSHARE);

	CMenu *pMenuPopup = Menu.GetSubMenu(0);
	
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_SCRSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_SCRSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
	
	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_WNDSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_WNDSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
	m_btnScrCap.GetWindowRect(&rcButton);

	int x = rcButton.left - 60;
	int y = rcButton.top - 50;

	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
	pMenuPopup->DestroyMenu();
}


void CVideoDlg::CreateMoreMenu()
{
    CRect				rcButton;
    CMenu				Menu;
    MENUITEMINFO		mii;
    UINT                nFlag = 0;

    Menu.LoadMenu(IDR_MENU_MORE);

    CMenu *pMenuPopup = Menu.GetSubMenu(0);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING;

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREINFO");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREINFO")) + 1;
    pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_VIDEOINFO, MF_BYCOMMAND, &m_bitMenuInfo, &m_bitMenuInfo);
    if (m_bShowInfo)
        nFlag = MF_BYCOMMAND | MF_CHECKED;
    else
        nFlag = MF_BYCOMMAND | MF_UNCHECKED;
    pMenuPopup->CheckMenuItem(ID_IDR_VIDEOINFO, nFlag);

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREDEVICE");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREDEVICE")) + 1;
    pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_DEVICE, MF_BYCOMMAND, &m_bitMenuDevice, &m_bitMenuDevice);

    mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREFILTER");
    mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREFILTER")) + 1;
    pMenuPopup->SetMenuItemInfo(2, &mii, FALSE);
    pMenuPopup->SetMenuItemBitmaps(ID_IDR_FILTER, MF_BYCOMMAND, &m_bitMenuFilter, &m_bitMenuFilter);
    if (m_bFilter)
        nFlag = MF_BYCOMMAND | MF_CHECKED;
    else
        nFlag = MF_BYCOMMAND | MF_UNCHECKED;
    pMenuPopup->CheckMenuItem(ID_IDR_FILTER, nFlag);
    
    
    m_btnMore.GetWindowRect(&rcButton);

    int x = rcButton.left - 60;
    int y = rcButton.top - 75;

    pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
    pMenuPopup->DestroyMenu();
}

void CVideoDlg::OnBnClickedBtnsetup()
{
	m_dlgDevice.ShowWindow(SW_SHOW);
	m_dlgDevice.CenterWindow();
}

void CVideoDlg::OnBnClickedBtnfilter()
{
    IRtcEngine *lpEngine = CAgoraObject::GetEngine();

    if (!m_bFilter){
        load_preprocessing_plugin(lpEngine);
    }
    else{
        unload_preprocessing_plugin(lpEngine);
    }

    m_bFilter = !m_bFilter;
}

void CVideoDlg::OnBnClickedBtnmessage()
{
    BOOL bIsShowing = m_dlgChat.IsWindowVisible();
    CRect   rcChatArea;

    rcChatArea.CopyRect(&m_rcVideoArea);
    rcChatArea.top = rcChatArea.bottom - 150;
    ClientToScreen(&rcChatArea);

    if (bIsShowing) {
        m_dlgChat.ShowWindow(SW_HIDE);
        m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
    }
    else {
        m_dlgChat.ShowWindow(SW_SHOW);
        m_dlgChat.MoveWindow(&rcChatArea, TRUE);
        m_btnMessage.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
    }
}

void CVideoDlg::OnBnClickedBtnmode()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();
    IRtcEngine *lpRtcEngine = CAgoraObject::GetEngine();

	if (!lpAgora->IsLocalVideoMuted()) {
		lpAgora->MuteLocalVideo(TRUE);
        m_btnMode.SetBackImage(IDB_BTNVIDEO_VIDEO);
        
        CAgoraObject::GetAgoraObject()->EnableLocalRender(FALSE);
        m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96);
        m_wndLocal.ShowBackground(TRUE);
	}
	else {
        lpAgora->EnableVideo(TRUE);
		lpAgora->MuteLocalVideo(FALSE);
        CAgoraObject::GetAgoraObject()->EnableLocalRender(TRUE);
        m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO);
        m_wndLocal.ShowBackground(FALSE);
	}
    
    m_wndLocal.Invalidate(TRUE);
    m_wndLocal.UpdateWindow();
    
    Invalidate(TRUE);
    UpdateWindow();
}

void CVideoDlg::OnBnClickedBtnaudio()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();

	if (lpAgora->IsLocalAudioMuted()) {
		lpAgora->MuteLocalAudio(FALSE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	}
	else {
		lpAgora->MuteLocalAudio(TRUE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	}
}

LRESULT CVideoDlg::OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)wParam;

	m_listWndInfo.RemoveAll();
	CAgoraObject::GetAgoraObject()->SetSelfUID(lpData->uid);

	delete lpData;
	return 0;
}

LRESULT CVideoDlg::OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REJOINCHANNEL_SUCCESS lpData = (LPAGE_REJOINCHANNEL_SUCCESS)wParam;

	m_listWndInfo.RemoveAll();
	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = (LPAGE_FIRST_LOCAL_VIDEO_FRAME)wParam;

	if (m_listWndInfo.GetCount() == 0)
		ShowVideo1();

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;
	BOOL bFound = FALSE;

	POSITION pos = m_listWndInfo.GetHeadPosition();
	while (pos != NULL) {
		AGVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
		if (agvWndInfo.nUID == lpData->uid) {
			bFound = TRUE;
			break;
		}
	}

	if (!bFound) {
		AGVIDEO_WNDINFO agvWndInfo;
		memset(&agvWndInfo, 0, sizeof(AGVIDEO_WNDINFO));
		agvWndInfo.nUID = lpData->uid;
		agvWndInfo.nWidth = lpData->width;
		agvWndInfo.nHeight = lpData->height;

		m_listWndInfo.AddTail(agvWndInfo);
	}

	RebindVideoWnd();

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDUserJoined(WPARAM wParam, LPARAM lParam)
{
    CString str;
    LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;

    str.Format(_T("%d joined the channel"), lpData->uid);
    MessageBox(str);
	return 0;
}

LRESULT CVideoDlg::OnEIDUserOffline(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)wParam;

	POSITION pos = m_listWndInfo.GetHeadPosition();
	while (pos != NULL){
		if (m_listWndInfo.GetAt(pos).nUID == lpData->uid) {
			m_listWndInfo.RemoveAt(pos);
			RebindVideoWnd();
			break;
		}

		m_listWndInfo.GetNext(pos);
	}

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDConnectionLost(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnRemoteVideoStat(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)wParam;

	POSITION posNext = m_listWndInfo.GetHeadPosition();

	while (posNext != NULL) {
		AGVIDEO_WNDINFO &rWndInfo = m_listWndInfo.GetNext(posNext);

		if (rWndInfo.nUID == lpData->uid) {
			rWndInfo.nFramerate = lpData->receivedFrameRate;
			rWndInfo.nBitrate = lpData->receivedBitrate;
			rWndInfo.nWidth = lpData->width;
			rWndInfo.nHeight = lpData->height;
			m_wndVideo[rWndInfo.nIndex].SetFrameRateInfo(rWndInfo.nFramerate);
			m_wndVideo[rWndInfo.nIndex].SetBitrateInfo(rWndInfo.nBitrate);
			m_wndVideo[rWndInfo.nIndex].SetVideoResolution(rWndInfo.nWidth, rWndInfo.nHeight);
			break;
		}
	}

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnStartRecordingService(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT CVideoDlg::OnStopRecordingService(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnStreamMessage(WPARAM wParam, LPARAM lParam)
{
    LPAGE_STREAM_MESSAGE lpData = (LPAGE_STREAM_MESSAGE)wParam;
    TCHAR szMessage[256];

    int nUTF8Len = lpData->length;
    
    memset(szMessage, 0, 256 * sizeof(TCHAR));
#ifdef UNICODE
    ::MultiByteToWideChar(CP_UTF8, 0, lpData->data, lpData->length, szMessage, 256);
#else
    _tcscpy_s(szMessage, 256, lpData->data);
#endif

    m_dlgChat.AddChatMessage(lpData->uid, szMessage);

    delete[] lpData->data;
    delete lpData;

    return 0;
}

void CVideoDlg::DrawHead(CDC *pDC)
{
	CRect rcTitle;
	CString strTitle;

	if (m_bFullScreen)
		return;

	GetWindowText(strTitle);
	GetClientRect(&rcTitle);
	CBrush *lpDefaultBrush = pDC->SelectObject(&m_brHead);

	rcTitle.bottom = 24;
	pDC->FillRect(&rcTitle, &m_brHead);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->DrawText(strTitle, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->SelectObject(lpDefaultBrush);
}

void CVideoDlg::InitCtrls()
{
	CRect	rcClient;
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	
	GetClientRect(&rcClient);

    m_btnMessage.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMESSAGE_VIDEO);
    m_btnMode.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMODE_VIDEO);
	m_btnAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNAUDIO_VIDEO);
	m_btnEndCall.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNENDCALL_VIDEO);
	m_btnScrCap.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCRCAP_VIDEO);
    m_btnMore.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMORE_VIDEO);

	m_btnShow.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCR_VIDEO);
	
	for (int nIndex = 0; nIndex < 4; nIndex++){
		m_wndVideo[nIndex].Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + nIndex);
		m_wndVideo[nIndex].SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
		m_wndVideo[nIndex].SetFaceColor(RGB(0x58, 0x58, 0x58));
	}

	m_wndLocal.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
	m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndLocal.SetFaceColor(RGB(0x58, 0x58, 0x58));
	m_wndLocal.SetUID(0);

	m_btnMin.MoveWindow(rcClient.Width() - 72, 1, 22, 22, TRUE);
	m_btnRst.MoveWindow(rcClient.Width() - 48, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(rcClient.Width() - 24, 1, 22, 22, TRUE);

    m_btnMessage.MoveWindow(rcClient.Width() / 2 - 144, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnMode.MoveWindow(rcClient.Width() / 2 - 24, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnAudio.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnShow.MoveWindow(rcClient.Width() - 72, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnMore.MoveWindow(rcClient.Width() / 2 + 264, rcClient.Height() - 84, 48, 48, TRUE);
    m_btnEndCall.MoveWindow(rcClient.Width() - 120, rcClient.Height() - 84, 48, 48, TRUE);
    
	m_wndVideo[0].MoveWindow(0, 24, rcClient.Width(), rcClient.Height() - 96, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0x00, 0xFF));
	m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0x00, 0xFF));

	m_btnScrCap.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnScrCap.EnableFrameEffect(FALSE);
	m_btnScrCap.SetBackImage(IDB_BTNSCRCAP_VIDEO, RGB(0x26, 0x26, 0x26));

    m_btnMessage.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
    m_btnMessage.EnableFrameEffect(FALSE);
    m_btnMessage.SetBackImage(IDB_BTNMSG_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnMode.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnMode.EnableFrameEffect(FALSE);
	m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnAudio.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnAudio.EnableFrameEffect(FALSE);
	m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	
	m_btnShow.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnShow.EnableFrameEffect(FALSE);
	m_btnShow.SetBackImage(IDB_BTNFULLSCR_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnEndCall.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnEndCall.EnableFrameEffect(FALSE);
	m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));

    m_btnMore.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
    m_btnMore.EnableFrameEffect(FALSE);
    m_btnMore.SetBackImage(IDB_BTNMORE_VIDEO, RGB(0x26, 0x26, 0x26));
}

void CVideoDlg::ShowVideo1()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
	}

	if (m_listWndInfo.GetCount() == 0) {
		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
		m_wndLocal.SetParent(this);
		m_wndLocal.SetBigShowFlag(TRUE);
		m_wndLocal.ShowWindow(SW_SHOW);
	}
	else {
		m_wndVideo[0].MoveWindow(&m_rcVideoArea, FALSE);
		m_wndVideo[0].ShowWindow(SW_SHOW);
		m_wndVideo[0].SetBigShowFlag(TRUE);
		m_wndVideo[0].SetParent(this);

		m_wndLocal.MoveWindow(&m_rcChildVideoArea, FALSE);
		m_wndLocal.SetParent(&m_wndVideo[0]);
		m_wndLocal.ShowWindow(SW_SHOW);
		m_wndLocal.SetParent(&m_wndVideo[0]);
	}

	m_nScreenMode = SCREEN_VIDEO1;

	ShowButtonsNormal();
}

void CVideoDlg::ShowVideo4()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_SHOW);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
		m_wndVideo[nIndex].SetParent(this);
	}

	m_wndVideo[0].MoveWindow(0, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[1].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[2].MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	m_wndVideo[3].MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);

	CRect	rcLocal;
	int		nRemoteCount = m_listWndInfo.GetCount();
	
	if (nRemoteCount == 2)
		m_wndLocal.MoveWindow(0, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);
	else
		m_wndLocal.MoveWindow(m_rcVideoArea.Width() / 2, m_rcVideoArea.top + m_rcVideoArea.Height() / 2, m_rcVideoArea.Width() / 2, m_rcVideoArea.Height() / 2, FALSE);

	m_wndVideo[nRemoteCount].ShowWindow(SW_HIDE);
	m_wndLocal.SetParent(this);
	m_wndLocal.ShowWindow(SW_SHOW);

	m_nScreenMode = SCREEN_VIDEO4;

	ShowButtonsNormal();
	
}

void CVideoDlg::ShowMulti()
{
	int nLocalIndex = 0;

	m_wndLocal.ShowWindow(TRUE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
	}
	
	if (m_lpBigShowed == NULL)
		m_lpBigShowed = &m_wndVideo[0];

	m_lpBigShowed->ShowWindow(SW_SHOW);
	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);
	m_lpBigShowed->SetParent(this);
	m_lpBigShowed->SetBigShowFlag(TRUE);
	
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.top + 16;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, 192, 144, TRUE);
				m_wndVideo[nIndex].ShowWindow(SW_SHOW);
				m_wndVideo[nIndex].SetParent(m_lpBigShowed);
				nLocalIndex++;
			}
		}
		else{
			m_wndLocal.MoveWindow(nXPos, nYPos, 192, 144, TRUE);
			m_wndLocal.ShowWindow(SW_SHOW);
			m_wndLocal.SetParent(m_lpBigShowed);
			nLocalIndex++;
		}
	}

	m_nScreenMode = SCREEN_VIDEOMULTI;

	ShowButtonsNormal();
}

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_dlgDevice.Create(CDeviceDlg::IDD, this);
	m_dlgDevice.EnableDeviceTest(FALSE);

	m_dlgScreenCapture.Create(CAGScreenCaptureDlg::IDD, this);
	m_dlgDesktopCapture.Create(CAGDesktopCaptureDlg::IDD, this);
    m_dlgChat.Create(CChatDlg::IDD, this);
	InitCtrls();
    
    m_bitMenuInfo.LoadBitmap(IDB_MENU_INFO);
    m_bitMenuDevice.LoadBitmap(IDB_MENU_DEVICE);
    m_bitMenuFilter.LoadBitmap(IDB_MENU_FILTER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


HWND CVideoDlg::GetRemoteVideoWnd(int nIndex)
{
	if (nIndex < 0 || nIndex > 3)
		return NULL;

	return m_wndVideo[nIndex].GetSafeHwnd();
}


void CVideoDlg::RebindVideoWnd()
{
	if (m_wndVideo[0].GetSafeHwnd() == NULL || m_wndLocal.GetSafeHwnd() == NULL)
		return;

	VideoCanvas canvas;

	canvas.renderMode = RENDER_MODE_FIT;

	POSITION pos = m_listWndInfo.GetHeadPosition();
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		if (pos != NULL) {
			AGVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
			canvas.uid = agvWndInfo.nUID;
			canvas.view = m_wndVideo[nIndex].GetSafeHwnd();
			agvWndInfo.nIndex = nIndex;

			CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
			m_wndVideo[nIndex].SetUID(canvas.uid);
			m_wndVideo[nIndex].SetVideoResolution(agvWndInfo.nWidth, agvWndInfo.nHeight);
			m_wndVideo[nIndex].SetFrameRateInfo(agvWndInfo.nFramerate);
			m_wndVideo[nIndex].SetBitrateInfo(agvWndInfo.nBitrate);
		}
		else
			m_wndVideo[nIndex].SetUID(0);
	}

	if (m_nScreenMode != SCREEN_VIDEOMULTI) {
		if (m_listWndInfo.GetCount() <= 1)
			ShowVideo1();
		else if (m_listWndInfo.GetCount() > 1 && m_listWndInfo.GetCount() < 4)
			ShowVideo4();
		else
			ShowMulti();
	}
	else
		ShowMulti();
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			if (m_bFullScreen)
				OnBnClickedBtnfullscr();
			
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CVideoDlg::OnShowModeChanged(WPARAM wParam, LPARAM lParam)
{
	if (m_nScreenMode == SCREEN_VIDEOMULTI) {
		if (m_listWndInfo.GetCount() <= 1)
			ShowVideo1();
		else if (m_listWndInfo.GetCount() > 1 && m_listWndInfo.GetCount() < 4)
			ShowVideo4();
	}
	else {
		m_lpBigShowed = (CAGVideoWnd *)wParam;
		ShowMulti();
	}
	
	return 0;
}

LRESULT CVideoDlg::OnShowBig(WPARAM wParam, LPARAM lParam)
{
	CRect rcNextBig;
	CRect rcCurBig;

	if (m_nScreenMode != SCREEN_VIDEOMULTI)
		return 0;

	CAGVideoWnd *lpNextBigWnd = (CAGVideoWnd *)wParam;
	if (lpNextBigWnd == m_lpBigShowed)
		return 0;

	ShowMulti();
	m_lpBigShowed = lpNextBigWnd;

	return 0;
}

LRESULT CVideoDlg::OnWindowShareStart(WPARAM wParam, LPARAM lParam)
{
    HWND hShareWnd = (HWND)wParam;
    if (hShareWnd == GetSafeHwnd()) {
        CAgoraObject::GetAgoraObject()->EnableLocalRender(FALSE);
        m_wndLocal.Invalidate(TRUE);
    }

	CAgoraObject::GetAgoraObject()->EnableScreenCapture((HWND)wParam, 15, NULL, TRUE);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}

LRESULT CVideoDlg::OnDesktopShareStart(WPARAM wParam, LPARAM lParam)
{
	LPDESKTOP_SHARE_PARAM	lpDesktopShareParam = (LPDESKTOP_SHARE_PARAM)(wParam);
	CRect rcRegion;

	CAgoraObject::GetAgoraObject()->EnableVideo();

	rcRegion.left = lpDesktopShareParam->nX;
	rcRegion.top = lpDesktopShareParam->nY;
	rcRegion.right = rcRegion.left + lpDesktopShareParam->nWidth;
	rcRegion.bottom = rcRegion.top + lpDesktopShareParam->nHeight;

	CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, lpDesktopShareParam->nFPS, &rcRegion, TRUE);
	//	Sleep(1000);
	//	CAgoraObject::GetAgoraObject()->SetVideoProfileEx(lpDesktopShareParam->nWidth, lpDesktopShareParam->nHeight, lpDesktopShareParam->nFPS, lpDesktopShareParam->nBitrate);

	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	return 0;
}

void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码

	if (bShow && GetSafeHwnd() != NULL)
		RebindVideoWnd();
}


void CVideoDlg::OnMove(int x, int y)
{
    CDialogEx::OnMove(x, y);

    // TODO:  在此处添加消息处理程序代码
    CRect rcChatBox;

    rcChatBox.SetRect(x, y + m_rcVideoArea.Height() - 126, x + m_rcVideoArea.Width(), y + m_rcVideoArea.Height() + 24);

    if (::IsWindow(m_dlgChat.GetSafeHwnd()) && m_dlgChat.IsWindowVisible()){
        m_dlgChat.MoveWindow(&rcChatBox);
    }
}
