#pragma once
class CAgoraCameraManager
{
public:
	CAgoraCameraManager();
	~CAgoraCameraManager();

	BOOL Create(IRtcEngine *lpRtcEngine);
	void Close();

	UINT GetDeviceCount();

	CString GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);

	BOOL GetDevice(UINT nIndex, CString &rDeviceName, CString &rDeviceID);
	void TestCameraDevice(HWND hVideoWnd, BOOL bTestOn = TRUE);

	BOOL IsTesting() { return m_bTestingOn; };

private:
	AVideoDeviceManager			*m_ptrDeviceManager;
	IVideoDeviceCollection		*m_lpCollection;
	BOOL						m_bTestingOn;
};

