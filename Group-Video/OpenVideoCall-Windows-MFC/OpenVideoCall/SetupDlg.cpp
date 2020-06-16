// SetupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenVideoCall.h"
#include "SetupDlg.h"
#include "afxdialogex.h"


// CSetupDlg 对话框

IMPLEMENT_DYNAMIC(CSetupDlg, CDialogEx)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupDlg::IDD, pParent)
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNCONFIRM_SETUP, m_btnConfirm);
    DDX_Control(pDX, IDC_BTNCANCEL_SETUP, m_btnCancel);
	DDX_Control(pDX, IDC_CKSAVESETTING_SETUP, m_ckSaveSettings);
    DDX_Control(pDX, IDC_CKSWPWH_SETUP, m_ckSwapWH);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNCONFIRM_SETUP, &CSetupDlg::OnBnClickedBtnconfirmSetup)
    ON_BN_CLICKED(IDC_BTNCANCEL_SETUP, &CSetupDlg::OnBnClickedBtncancelSetup)
//	ON_CBN_SELCHANGE(IDC_CMBRES_SETUP, &CSetupDlg::OnCbnSelchangeCmbresSetup)
//	ON_CBN_SELCHANGE(IDC_CMBCODEC_SETUP, &CSetupDlg::OnCbnSelchangeCmbresSetup)

	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()

END_MESSAGE_MAP()


// CSetupDlg 消息处理程序


BOOL CSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
    m_ftHead.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_ftDes.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_penFrame.CreatePen(PS_SOLID, 1, RGB(0xD8, 0xD8, 0xD8));

	m_cbxVideoProfile.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(0, 0, 300, 40), this, IDC_CMBVDOPRF_SETUP);
    m_cbxFPS.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(0, 0, 120, 40), this, IDC_CMBFPS_SETUP);
    m_cbxBitrate.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(0, 0, 120, 40), this, IDC_CMBBITRATE_SETUP);

    SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF), TRUE);
    InitData2();
    InitCtrls();

	if (m_agConfig.IsAutoSaveEnabled())
		m_ckSaveSettings.SetCheck(TRUE);
	else
		m_ckSaveSettings.SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CSetupDlg::InitCtrls()
{
	CRect ClientRect;

	MoveWindow(0, 0, 320, 450, 1);
	GetClientRect(&ClientRect);

	int nResolutionIndex = 0;
	CString str;
	
    m_cbxVideoProfile.MoveWindow(90, 125, 140, 22, TRUE);//210,80,200,22
    m_cbxVideoProfile.SetFont(&m_ftDes);
    m_cbxVideoProfile.SetButtonImage(IDB_CMBBTN, 12, 12, RGB(0xFF, 0x00, 0xFF));
    m_cbxVideoProfile.SetFaceColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));
    m_cbxVideoProfile.SetListMaxHeight(600);

    ::SendMessage(m_cbxVideoProfile.GetSafeHwnd(), CB_SETDROPPEDWIDTH, 140, 0);

    m_cbxFPS.MoveWindow(225, 125, 100, 22, TRUE);
    m_cbxFPS.SetFont(&m_ftDes);
    m_cbxFPS.SetButtonImage(IDB_CMBBTN, 12, 12, RGB(0xFF, 0x00, 0xFF));
    m_cbxFPS.SetFaceColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));

    m_cbxBitrate.MoveWindow(330, 125, 130, 22, TRUE);
    m_cbxBitrate.SetFont(&m_ftDes);
    m_cbxBitrate.SetButtonImage(IDB_CMBBTN, 12, 12, RGB(0xFF, 0x00, 0xFF));
    m_cbxBitrate.SetFaceColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));


    m_ckSwapWH.MoveWindow(75, 160, 20, 20, TRUE);
    m_ckSaveSettings.MoveWindow(185, ClientRect.Height() - 120, 20, 20, TRUE);

    m_btnCancel.MoveWindow(ClientRect.Width() / 2 - 93, ClientRect.Height() - 58, 174, 36, TRUE);
    m_btnConfirm.MoveWindow(ClientRect.Width() / 2 + 93, ClientRect.Height() - 58, 174, 36, TRUE);

    for (int nIndex = 0; nIndex < RESOLUTION_COUNT; nIndex++) {
        m_cbxVideoProfile.InsertString(nIndex, m_szProfileDes[nIndex]);
        m_cbxVideoProfile.SetItemData(nIndex, (DWORD_PTR)&m_mapResolition[nIndex]);
    }

    for (int i = 0; i < FPS_COUNT; ++i) {
        m_cbxFPS.InsertString(i, m_szFPS[i]);
        m_cbxFPS.SetItemData(i, (DWORD_PTR)m_nFPS[i]);
    }

    for (int i = 0; i < 3; ++i) {
        m_cbxBitrate.InsertString(i, m_szBitrate[i]);
        m_cbxBitrate.SetItemData(i, (DWORD_PTR)m_nBitrate[i]);
    }

    m_btnCancel.SetBorderColor(RGB(0xD8, 0xD8, 0xD8), RGB(0x00, 0xA0, 0xE9), RGB(0x00, 0xA0, 0xE9), RGB(0xCC, 0xCC, 0xCC));
    m_btnCancel.SetBackColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));
    m_btnCancel.SetTextColor(RGB(0x55, 0x58, 0x5A), RGB(0x00, 0xA0, 0xE9), RGB(0x00, 0xA0, 0xE9), RGB(0xCC, 0xCC, 0xCC));

	m_btnConfirm.SetBackColor(RGB(0, 160, 239), RGB(0, 160, 239), RGB(0, 160, 239), RGB(192, 192, 192));
	m_btnConfirm.SetFont(&m_ftBtn);
	m_btnConfirm.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xC8, 0x64), RGB(0xFF, 0xC8, 0x64), RGB(0xCC, 0xCC, 0xCC));
	m_btnConfirm.SetWindowText(LANG_STR("IDS_SET_BTCONFIRM"));

    if (m_agConfig.IsAutoSaveEnabled())
        nResolutionIndex = m_agConfig.GetSolution();
    else
        nResolutionIndex = 15;
    
    m_cbxVideoProfile.SetCurSel(nResolutionIndex);
    m_cbxFPS.SetCurSel(4);//24fps
    m_cbxBitrate.SetCurSel(0);//standard bitrate
}

void CSetupDlg::InitData()
{
    m_szProfileDes[0] = _T("160x120 15fps"); //65kbps
    m_nProfileValue[0] = 0;
    m_szProfileDes[1] = _T("120x120 15fps"); //50kbps
    m_nProfileValue[1] = 2;

    m_szProfileDes[2] = _T("320x180 15fps"); //140kbps
    m_nProfileValue[2] = 10;
    m_szProfileDes[3] = _T("180x180 15fps"); //100kbps
    m_nProfileValue[3] = 12;
    m_szProfileDes[4] = _T("240x180 15fps"); //120kbps
    m_nProfileValue[4] = 13;

    m_szProfileDes[5] = _T("320x240 15fps"); //200kbps
    m_nProfileValue[5] = 20;
    m_szProfileDes[6] = _T("240x240 15fps"); //140kbps
    m_nProfileValue[6] = 22;
    m_szProfileDes[7] = _T("424x240 15fps"); //220kbps
    m_nProfileValue[7] = 23;

    m_szProfileDes[8] = _T("640x360 15fps"); //400kbps
    m_nProfileValue[8] = 30;
    m_szProfileDes[9] = _T("360x360 15fps"); //260kbps
    m_nProfileValue[9] = 32;
    m_szProfileDes[10] = _T("640x360 30fps"); //600kbps
    m_nProfileValue[10] = 33;
    m_szProfileDes[11] = _T("360x360 30fps"); //400kbps
    m_nProfileValue[11] = 35;
    m_szProfileDes[12] = _T("480x360 15fps"); //320kbps
    m_nProfileValue[12] = 36;
    m_szProfileDes[13] = _T("480x360 30fps"); //490kbps
    m_nProfileValue[13] = 37;
    m_szProfileDes[14] = _T("640x360 15fps"); //800kbps
    m_nProfileValue[14] = 38;

    m_szProfileDes[15] = _T("640x480 15fps"); //500kbps
    m_nProfileValue[15] = 40;
    m_szProfileDes[16] = _T("480x480 15fps"); //400kbps
    m_nProfileValue[16] = 42;
    m_szProfileDes[17] = _T("640x480 30fps"); //750kbps
    m_nProfileValue[17] = 43;
    m_szProfileDes[18] = _T("480x480 30fps"); //600kbps
    m_nProfileValue[18] = 44;
    m_szProfileDes[19] = _T("848x480 15fps"); //610kbps
    m_nProfileValue[19] = 47;
    m_szProfileDes[20] = _T("848x480 30fps"); //930kbps
    m_nProfileValue[20] = 48;

    m_szProfileDes[21] = _T("1280x720 15fps"); //1130kbps
    m_nProfileValue[21] = 50;
    m_szProfileDes[22] = _T("1280x720 30fps"); //1710kbps
    m_nProfileValue[22] = 52;
    m_szProfileDes[23] = _T("960x720 15fps"); //910kbps
    m_nProfileValue[23] = 54;
    m_szProfileDes[24] = _T("960x720 30fps"); //1380kbps
    m_nProfileValue[24] = 55;

    m_szProfileDes[25] = _T("1920x1080 15fps"); //2080kbps
    m_nProfileValue[25] = 60;
    m_szProfileDes[26] = _T("1920x1080 30fps"); //3150kbps
    m_nProfileValue[26] = 62;
    m_szProfileDes[27] = _T("1920x1080 60fps"); //4780kbps
    m_nProfileValue[27] = 64;
    m_szProfileDes[28] = _T("2560x1440 30fps"); //4850kbps
    m_nProfileValue[28] = 66;
    m_szProfileDes[29] = _T("3560x1440 60fps"); //7350kbps
    m_nProfileValue[29] = 67;

    m_szProfileDes[30] = _T("3840x2160 30fps"); //8190kbps
    m_nProfileValue[30] = 70;
    m_szProfileDes[31] = _T("3840x2160 60fps"); //13500kbps
    m_nProfileValue[31] = 72;

}


#include <utility>
void CSetupDlg::InitData2()
{
    int i = 0;
    m_szProfileDes[i] = _T("160x120");//0
    SIZE sz = { 160, 120 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("120x120");
    sz = { 120, 120 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("320x180");//2
    sz = { 320, 180 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("180x180");
    sz = { 180, 180 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("240x180");
    sz = { 240, 180 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("320x240");//5
    sz = { 320, 240 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("240x240");
    sz = { 240, 240 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("424x240");
    sz = { 424, 240 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("640x360");//8
    sz = { 640, 360 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("360x360");
    sz = { 360, 360 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("480x360");
    sz = { 480, 360 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("640x480");//11
    sz = { 640, 480 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("480x480");
    sz = { 480, 480 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("848x480");
    sz = { 848, 480 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("1280x720");//14
    sz = { 1280, 720 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("960x720");
    sz = { 960, 720 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("1920x1080");//16
    sz = { 1920, 1080 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("2560x1440");
    sz = { 2560, 1440 };
    m_mapResolition.insert(std::make_pair(i, sz));

    i++;
    m_szProfileDes[i] = _T("3840x2160");//18
    sz = { 3840, 2160 };
    m_mapResolition.insert(std::make_pair(i, sz));

    //fps
    i = 0;
    m_szFPS[i] = _T("1fps");
    m_nFPS[i++] = 1;

    m_szFPS[i] = _T("7fps");
    m_nFPS[i++] = 7;

    m_szFPS[i] = _T("10fps");
    m_nFPS[i++] = 10;

    m_szFPS[i] = _T("15fps");
    m_nFPS[i++] = 15;

    m_szFPS[i] = _T("24fps");
    m_nFPS[i++] = 24;

    m_szFPS[i] = _T("30fps");
    m_nFPS[i++] = 30;

    m_szFPS[i] = _T("60fps");
    m_nFPS[i++] = 60;

    //
    m_szBitrate[0] = _T("Standard");
    m_nBitrate[0] = 0;
    m_szBitrate[1] = _T("Compatible");
    m_nBitrate[1] = -1;
    m_szBitrate[2] = _T("Default");
    m_nBitrate[2] = 1;
}

void CSetupDlg::DrawClient(CDC *lpDC)
{
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;

	GetClientRect(&rcClient);

	CFont* defFont = lpDC->SelectObject(&m_ftHead);

    lpDC->SelectObject(&m_penFrame);
    rcText.SetRect(rcClient.Width() / 2 - 188, 120, rcClient.Width() / 2 + 172, 152);
    lpDC->RoundRect(&rcText, CPoint(32, 32));

    lpDC->SetBkColor(RGB(0xFF, 0xFF, 0xFF));
    lpDC->SetTextColor(RGB(0xD8, 0xD8, 0xD8));
	lpString = LANG_STR("IDS_SET_RESOLUTION");
	lpDC->TextOut(75, 128, lpString, _tcslen(lpString));

    lpDC->SetTextColor(RGB(0x44, 0x45, 0x46));
    lpString = LANG_STR("IDS_SET_SWAPWH");
    lpDC->TextOut(100, 160, lpString, _tcslen(lpString));

    lpString = LANG_STR("IDS_SET_SAVESETTING");
	lpDC->TextOut(205, rcClient.Height() - 118, lpString, _tcslen(lpString));

	lpDC->SelectObject(defFont);
}

void CSetupDlg::OnBnClickedBtnconfirmSetup()
{
	// TODO:  在此添加控件通知处理程序代码
	GetParent()->SendMessage(WM_GOBACK, 0, 0);

	if (m_ckSaveSettings.GetCheck() == TRUE) {
		m_agConfig.EnableAutoSave(TRUE);
        m_agConfig.SetSolution(m_cbxVideoProfile.GetCurSel());
	}
	else
		m_agConfig.EnableAutoSave(FALSE);
}

void CSetupDlg::OnBnClickedBtncancelSetup()
{
    GetParent()->SendMessage(WM_GOBACK, 0, 0);
}

void CSetupDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawClient(&dc);
}

int CSetupDlg::GetVideoSolution()
{
    int nIndex = m_cbxVideoProfile.GetCurSel();

    return (int)m_cbxVideoProfile.GetItemData(nIndex);
}

void CSetupDlg::SetVideoSolution(int nIndex)
{
	m_cbxVideoProfile.SetCurSel(nIndex);
}

void CSetupDlg::SetWHSwap(BOOL bSwap)
{
    m_ckSwapWH.SetCheck(bSwap);
}

BOOL CSetupDlg::IsWHSwap()
{
    return (BOOL)m_ckSwapWH.GetCheck();
}


int CSetupDlg::GetFPS()
{
    if (m_agConfig.IsCustomFPS()) {
        return m_agConfig.GetCustomFPS();
    }
    return m_nFPS[m_cbxFPS.GetCurSel()];
}

int CSetupDlg::GetBirate()
{
    if (m_agConfig.IsCustomBitrate()) {
        return m_agConfig.GetCustomBitrate();
    }
    return m_nBitrate[m_cbxBitrate.GetCurSel()];
}

CString CSetupDlg::GetVideoSolutionDes()
{
    int nIndex = m_cbxVideoProfile.GetCurSel();
    if (nIndex == -1)
        nIndex = 0;
    CString strDes;
    strDes.Format(_T("%s %s %s Bitrate"), m_szProfileDes[nIndex], m_szFPS[m_cbxFPS.GetCurSel()], m_szBitrate[m_cbxBitrate.GetCurSel()]);
    return (CString)strDes;
}


SIZE CSetupDlg::GetVideoResolution()
{
    if (m_agConfig.IsCustomRsolution()) {
        int w = 0, h = 0;
        m_agConfig.GetResolution(w, h);
        SIZE sz = { w, h };
        return sz;
    }

    return m_mapResolition[m_cbxVideoProfile.GetCurSel()];
}