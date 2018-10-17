#pragma once
class CAgoraAudInputManager
{
public:
	CAgoraAudInputManager();
	~CAgoraAudInputManager();

	BOOL Create(IRtcEngine *lpRtcEngine);
	void Close();

	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	CString GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);

	BOOL GetDevice(UINT nIndex, CString &rDeviceName, CString &rDeviceID);
	
	void TestAudInputDevice(HWND hMsgWnd, BOOL bTestOn);

	BOOL IsTesting() { return m_bTestingOn; };
private:
	BOOL						m_bTestingOn;
	HWND						m_hOldMsgWnd;
	AAudioDeviceManager			*m_ptrDeviceManager;
	IAudioDeviceCollection		*m_lpCollection;
};

