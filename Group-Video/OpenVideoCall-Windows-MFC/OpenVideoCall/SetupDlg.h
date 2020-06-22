#pragma once
#include "AGButton.h"
#include "AGComboBox.h"
#include "AGSliderCtrl.h"
#include "afxwin.h"

#include "AGConfig.h"
#include <unordered_map>
#include "AGConfig.h"

#define RESOLUTION_COUNT 19
#define FPS_COUNT 7
// CSetupDlg 对话框

class CSetupDlg : public CDialogEx
{

	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetupDlg();

    void SetVideoSolution(int nIndex);
    int GetVideoSolution();
    CString GetVideoSolutionDes();
    SIZE GetVideoResolution();
    int GetFPS();
    int GetBirate();
   
    void SetWHSwap(BOOL bSwap);
    BOOL IsWHSwap();

// 对话框数据
	enum { IDD = IDD_SETUP_DIALOG };

	typedef struct _AGVIDEO_PARAM
	{
		int nWidth;
		int nHeight;
		int nMinKPB;
		int nMaxKPB;

	} AGNET_RATE_RANGE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnconfirmSetup();
    afx_msg void OnBnClickedBtncancelSetup();
	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void InitData();
	void DrawClient(CDC *lpDC);
    void InitData2();
private:
    CAGButton       m_btnCancel;
	CAGButton		m_btnConfirm;
    CButton			m_ckSwapWH;
    CButton			m_ckSaveSettings;

	CAGComboBox		m_cbxVideoProfile;
    CAGComboBox     m_cbxFPS;
    CAGComboBox     m_cbxBitrate;

	CFont			m_ftHead;		// title
	CFont			m_ftDes;		// text in ctrl
	CFont			m_ftBtn;		// button
    CPen            m_penFrame;
	
    LPTSTR			    m_szProfileDes[64];
    int					m_nProfileValue[64];
    AGNET_RATE_RANGE	m_agRateRange[13];

    LPTSTR              m_szFPS[7];//1 7 10 15 24 30 60
    int                 m_nFPS[7];

    LPTSTR              m_szBitrate[3];
    int                 m_nBitrate[3];

    std::unordered_map<int, SIZE> m_mapResolition;

private:
	CAGConfig		m_agConfig;
};
