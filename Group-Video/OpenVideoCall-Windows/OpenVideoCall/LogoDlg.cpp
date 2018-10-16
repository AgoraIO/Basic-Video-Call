// LogoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "LogoDlg.h"
#include "afxdialogex.h"


// CLogoDlg 对话框

IMPLEMENT_DYNAMIC(CLogoDlg, CDialogEx)

CLogoDlg::CLogoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bmpLogo.LoadBitmap(IDB_LOGO);
}

CLogoDlg::~CLogoDlg()
{
}

void CLogoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNMIN, m_btnMin);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CLogoDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTNMIN, &CLogoDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE, &CLogoDlg::OnBnClickedBtnclose)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLogoDlg 消息处理程序

BOOL CLogoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    m_ftTitle.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftDescp.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
	InitCtrls();

	AnimateWindow(500, AW_BLEND | AW_SLIDE);
	m_nTimerID = SetTimer(1, 1000, NULL);

	Invalidate(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLogoDlg::InitCtrls()
{
	CRect ClientRect;

	MoveWindow(0, 0, 720, 600, 1);
	GetClientRect(&ClientRect);
	CenterWindow();

	m_btnMin.MoveWindow(ClientRect.Width() - 46, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(ClientRect.Width() - 23, 1, 22, 22, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0, 0xFF));

	CMFCButton::EnableWindowsTheming(FALSE);
}


LRESULT CLogoDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CLogoDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
    CDC     memDC;

	LPCTSTR lpString = NULL;
	CFont* defFont = lpDC->SelectObject(&m_ftTitle);

    memDC.CreateCompatibleDC(lpDC);
    memDC.SelectObject(&m_bmpLogo);
	lpString = LANG_STR("IDS_TITLE");
    lpDC->FillSolidRect(0, 0, 720, 24, RGB(0, 161, 230));
	lpDC->SetBkColor(RGB(0x00, 0x9E, 0xEB));
	lpDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	rcText.SetRect(5, 5, 20, 400);
	lpDC->TextOut(320, 3, lpString, _tcslen(lpString));

	lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
	lpDC->SetTextColor(RGB(0x91, 0x96, 0xA0));
	lpDC->TextOut(305, 316, lpString, _tcslen(lpString));
	
    lpString = LANG_STR("IDS_LOGO_DESC");
	lpDC->SelectObject(&m_ftDescp);
	lpDC->SetTextColor(RGB(0x00, 0x9D, 0xEB));
	lpDC->TextOut(255, 348, lpString, _tcslen(lpString));

    lpDC->BitBlt(294, 224, 132, 72, &memDC, 0, 0, SRCCOPY);

	lpDC->SelectObject(defFont);
}

void CLogoDlg::OnBnClickedBtnmin()
{
	// TODO:  在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void CLogoDlg::OnBnClickedBtnclose()
{
	// TODO:  在此添加控件通知处理程序代码
#ifdef DEBUG
	AnimateWindow(3000, AW_BLEND | AW_HIDE | AW_SLIDE);
#else
    AnimateWindow(1000, AW_BLEND | AW_HIDE | AW_SLIDE);
#endif
	CDialogEx::OnCancel();
}



void CLogoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1){
		PostMessage(WM_COMMAND, (WPARAM)IDCANCEL, NULL);
		KillTimer(1);
	}
		
	CDialogEx::OnTimer(nIDEvent);
}


void CLogoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	DrawClient(&dc);
}
