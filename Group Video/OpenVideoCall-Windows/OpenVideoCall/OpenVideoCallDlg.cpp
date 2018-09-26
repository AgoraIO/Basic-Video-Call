
// AgoraVideoCallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "OpenVideoCallDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAgoraVideoCallDlg dialog



COpenVideoCallDlg::COpenVideoCallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenVideoCallDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nVideoSolution = 0;

	m_lpAgoraObject = NULL;
	m_lpRtcEngine = NULL;

	m_nLastmileQuality = 0;
}

void COpenVideoCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNMIN, m_btnMin);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnClose);

	DDX_Control(pDX, IDC_LINKAGORA, m_linkAgora);
}

BEGIN_MESSAGE_MAP(COpenVideoCallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_GOBACK, &COpenVideoCallDlg::OnBackPage)
	ON_MESSAGE(WM_GONEXT, &COpenVideoCallDlg::OnNextPage)
	ON_MESSAGE(WM_JOINCHANNEL, &COpenVideoCallDlg::OnJoinChannel)
	ON_MESSAGE(WM_LEAVECHANNEL, &COpenVideoCallDlg::OnLeaveChannel)
	
	ON_BN_CLICKED(IDC_BTNMIN, &COpenVideoCallDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE, &COpenVideoCallDlg::OnBnClickedBtnclose)

    ON_MESSAGE(WM_MSGID(EID_LASTMILE_QUALITY), &COpenVideoCallDlg::OnLastmileQuality)

END_MESSAGE_MAP()


// CAgoraVideoCallDlg message handlers
BOOL COpenVideoCallDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_ESCAPE:
		case VK_RETURN:
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL COpenVideoCallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_ftTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftLink.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftDes.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftPhone.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	CString strAppID = CAgoraObject::LoadAppID();

	if (_tcslen(strAppID) == 0) {
		MessageBox(_T("Please define your own APP_ID in source code"), _T("information"), MB_OK | MB_ICONINFORMATION);
		::PostQuitMessage(0);
	}

	m_lpAgoraObject = CAgoraObject::GetAgoraObject(strAppID);
	m_lpRtcEngine = CAgoraObject::GetEngine();
    m_lpAgoraObject->EnableVideo(TRUE);
    m_lpAgoraObject->SetLogFilePath(NULL);
	m_lpAgoraObject->EnableNetworkTest(TRUE);
	m_lpAgoraObject->SetMsgHandlerWnd(GetSafeHwnd());

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
	InitCtrls();
	InitChildDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenVideoCallDlg::InitCtrls()
{
	CRect ClientRect;
	CBitmap	bmpNetQuality;

	MoveWindow(0, 0, 720, 600, 1);
	GetClientRect(&ClientRect);

	bmpNetQuality.LoadBitmap(IDB_NETWORK_QUALITY);

	m_imgNetQuality.Create(32, 32, ILC_COLOR24 | ILC_MASK, 6, 1);
	m_imgNetQuality.Add(&bmpNetQuality, RGB(0xFF, 0, 0xFF));

	m_btnMin.MoveWindow(ClientRect.Width() - 46, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(ClientRect.Width() - 23, 1, 22, 22, TRUE);
	m_linkAgora.MoveWindow(ClientRect.Width() / 2 - 110, ClientRect.Height()-55, 80, 20, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0, 0xFF));

	m_linkAgora.SetFont(&m_ftLink);
	m_linkAgora.SetURL(_T("http://www.agora.io"));
	m_linkAgora.SetWindowText(LANG_STR("IDS_LOGO_AGORAWEB"));
	CMFCButton::EnableWindowsTheming(FALSE);
}

void COpenVideoCallDlg::InitChildDialog()
{
	CString str;

	m_dlgEnterChannel.Create(CEnterChannelDlg::IDD, this);
	m_dlgSetup.Create(CSetupDlg::IDD, this);
	m_dlgVideo.Create(CVideoDlg::IDD, this);

	m_dlgEnterChannel.MoveWindow(110, 70, 500, 450, TRUE);
	m_dlgSetup.MoveWindow(110, 70, 500, 450, TRUE);

	m_dlgEnterChannel.ShowWindow(SW_SHOW);
	m_lpCurDialog = &m_dlgEnterChannel;

    m_dlgEnterChannel.SetVideoString(m_dlgSetup.GetVideoSolutionDes());

//	str.Format(_T("%d*%d,%dfps,%dKbps"), m_nVideoWidth, m_nVideoHeight, m_nFrameRate, m_nMaxRate);
//	m_dlgEnterChannel.SetVideoString(str);
}

void COpenVideoCallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpenVideoCallDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawClient(&dc);
//		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpenVideoCallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT COpenVideoCallDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
			lResult = HTCAPTION;
		
	return lResult;
}

void COpenVideoCallDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;
	CFont* defFont = lpDC->SelectObject(&m_ftTitle);

	m_imgNetQuality.Draw(lpDC, m_nLastmileQuality, CPoint(16, 40), ILD_NORMAL);

	GetClientRect(&rcClient);
    lpDC->FillSolidRect(0, 0, 720, 24, RGB(0, 161, 230));
	lpDC->SetBkColor(RGB(0x00, 0x9E, 0xEB));
	lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = LANG_STR("IDS_TITLE");
	lpDC->TextOut(rcClient.Width()/2 - 80, 3, lpString, _tcslen(lpString));
	
    lpDC->SelectObject(&m_ftDes);
    lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
    lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
    lpString = LANG_STR("IDS_CONTACT");
    lpDC->TextOut(rcClient.Width() / 2-50, rcClient.Height() - 55, lpString, _tcslen(lpString));

    lpDC->SelectObject(&m_ftPhone);
    lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
    lpString = LANG_STR("IDS_PHONENUMBER");
    lpDC->TextOut(rcClient.Width() / 2 + 45, rcClient.Height() - 55, lpString, _tcslen(lpString));

    lpDC->SelectObject(&m_ftDes);
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = AVC_VER;// _T("v1.3");
	CString strVersion = CAgoraObject::GetSDKVersionEx();

	rcText.SetRect(0, rcClient.Height() - 30, rcClient.Width(), rcClient.Height() - 5);
	lpDC->DrawText(strVersion, _tcslen(strVersion), &rcText, DT_CENTER | DT_SINGLELINE);
	lpDC->SelectObject(defFont);
}

void COpenVideoCallDlg::OnBnClickedBtnmin()
{
	// TODO:  在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void COpenVideoCallDlg::OnBnClickedBtnclose()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

LRESULT COpenVideoCallDlg::OnBackPage(WPARAM wParam, LPARAM lParam)
{
	if (m_lpCurDialog == &m_dlgSetup) {
		m_lpCurDialog->ShowWindow(SW_HIDE);
		m_lpCurDialog = &m_dlgEnterChannel;
	}

	CString str;

	m_dlgSetup.GetVideoSolution();
    m_dlgEnterChannel.SetVideoString(m_dlgSetup.GetVideoSolutionDes());

	m_lpCurDialog->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT COpenVideoCallDlg::OnNextPage(WPARAM wParam, LPARAM lParam)
{
	m_lpCurDialog->ShowWindow(SW_HIDE);
	if (m_lpCurDialog == &m_dlgEnterChannel)
			m_lpCurDialog = &m_dlgSetup;

	m_lpCurDialog->ShowWindow(SW_SHOW);

	return 0;
}

LRESULT COpenVideoCallDlg::OnJoinChannel(WPARAM wParam, LPARAM lParam)
{
	IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	CString strChannelName = m_dlgEnterChannel.GetChannelName();
    if (strChannelName.GetLength() == 0)
        return 0;

	m_dlgVideo.MoveWindow(0, 0, 960, 720, 1);
	m_dlgVideo.ShowWindow(SW_SHOW);
	m_dlgVideo.CenterWindow();

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_dlgVideo.GetLocalVideoWnd();
	vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;

	//cancel setVideoProfile bitrate since version 2.1.0
    m_nVideoSolution = m_dlgSetup.GetVideoSolution();
    lpRtcEngine->setVideoProfile((VIDEO_PROFILE_TYPE)m_nVideoSolution, m_dlgSetup.IsWHSwap());
    lpAgoraObject->EnableVideo(TRUE);

	m_dlgVideo.SetWindowText(strChannelName);
	lpRtcEngine->setupLocalVideo(vc);
	lpRtcEngine->startPreview();

	lpAgoraObject->JoinChannel(strChannelName);
	lpAgoraObject->SetMsgHandlerWnd(m_dlgVideo.GetSafeHwnd());

	return 0;
}

LRESULT COpenVideoCallDlg::OnLeaveChannel(WPARAM wParam, LPARAM lParam)
{
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	lpAgoraObject->LeaveCahnnel();
    m_dlgEnterChannel.CleanEncryptionSecret();

	return 0;
}

LRESULT COpenVideoCallDlg::OnLastmileQuality(WPARAM wParam, LPARAM lParam)
{
    LPAGE_LASTMILE_QUALITY lpData = (LPAGE_LASTMILE_QUALITY)wParam;

	if (m_nLastmileQuality != lpData->quality) {
		m_nLastmileQuality = lpData->quality;
		InvalidateRect(CRect(16, 40, 48, 72), TRUE);
	}

	delete lpData;
	return 0;
}