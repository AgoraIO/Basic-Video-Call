
#include "stdafx.h"
#include "AgoraTutorial.h"
#include "AgoraTutorialDlg.h"
#include "afxdialogex.h"

#include "AGEventDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CAgoraTutorialDlg::CAgoraTutorialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgoraTutorialDlg::IDD, pParent)
	, m_strChannelName(_T("AgoraTest"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAgoraTutorialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDCHANNEL, m_strChannelName);
	DDX_Control(pDX, IDC_STATIC_REMOTE, m_wndRemote);
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_wndLocal);
}

BEGIN_MESSAGE_MAP(CAgoraTutorialDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNJOIN, &CAgoraTutorialDlg::OnBnClickedBtnjoin)
	ON_BN_CLICKED(IDC_BTNLEAVE, &CAgoraTutorialDlg::OnBnClickedBtnleave)
	ON_BN_CLICKED(IDC_BTNDISVID, &CAgoraTutorialDlg::OnBnClickedBtndisvid)
	ON_BN_CLICKED(IDC_BTNMUTELOCAUD, &CAgoraTutorialDlg::OnBnClickedBtnmutelocaud)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CAgoraTutorialDlg::OnFirstRemoteVideoDecoded)
	ON_MESSAGE(WM_MSGID(EID_USER_OFFLINE), &CAgoraTutorialDlg::OnUserOffline)
END_MESSAGE_MAP()

BOOL CAgoraTutorialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	if (_tcslen(APP_ID) == 0) {
		MessageBox(_T("Please apply your own App ID to macro APP_ID"), _T("Notice"), MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	m_lpAgoraObject = CAgoraObject::GetAgoraObject(APP_ID);
	m_lpAgoraObject->SetMsgHandlerWnd(GetSafeHwnd());

	m_lpAgoraObject->GetEngine()->enableVideo();

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_wndLocal.GetSafeHwnd();
	vc.renderMode = agora::media::base::RENDER_MODE_TYPE::RENDER_MODE_FIT;

	m_lpAgoraObject->GetEngine()->setupLocalVideo(vc);

	return TRUE;
}

void CAgoraTutorialDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAgoraTutorialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CAgoraTutorialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAgoraTutorialDlg::OnBnClickedBtnjoin()
{
	UpdateData(TRUE);

	if (m_strChannelName.IsEmpty())
		MessageBox(_T("Channel name must not be empty"), _T("Notice"), MB_OK | MB_ICONERROR);
	else {
		m_lpAgoraObject->JoinChannel(m_strChannelName, 0, MEDIA_TOKEN);
	}
		
}


void CAgoraTutorialDlg::OnBnClickedBtnleave()
{
	m_lpAgoraObject->LeaveChannel();
	m_remoteUid = 0;
}

void CAgoraTutorialDlg::OnBnClickedBtnmutelocaud()
{
	BOOL bStatValue = !m_lpAgoraObject->IsLocalAudioMuted();
	m_lpAgoraObject->MuteLocalAudio(bStatValue);
}

void CAgoraTutorialDlg::OnBnClickedBtndisvid()
{
	BOOL bStatValue = !m_lpAgoraObject->IsLocalVideoMuted();
	m_lpAgoraObject->MuteLocalVideo(bStatValue);
}

LRESULT CAgoraTutorialDlg::OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;

	if (m_remoteUid != 0) {
		return 0;
	}

	m_remoteUid = lpData->uid;

	VideoCanvas vc;

	vc.renderMode = agora::media::base::RENDER_MODE_TYPE::RENDER_MODE_FIT;
	vc.uid = lpData->uid;
	vc.view = m_wndRemote.GetSafeHwnd();

	m_lpAgoraObject->GetEngine()->setupRemoteVideo(vc);

	delete lpData;

	return 0;
}

LRESULT CAgoraTutorialDlg::OnUserOffline(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;

	if (m_remoteUid != lpData->uid) {
		return 0;
	}

	VideoCanvas vc;

	vc.uid = lpData->uid;
	vc.view = NULL;

	m_lpAgoraObject->GetEngine()->setupRemoteVideo(vc);
	m_remoteUid = 0;

	delete lpData;

	return 0;
}

