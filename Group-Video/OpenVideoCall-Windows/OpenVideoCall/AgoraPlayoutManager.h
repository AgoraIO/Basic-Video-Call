#pragma once
class CAgoraPlayoutManager
{
public:
	CAgoraPlayoutManager();
	~CAgoraPlayoutManager();

	BOOL Create(IRtcEngine *lpRtcEngine);
	void Close();

	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	CString GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);

	BOOL GetDevice(UINT nIndex, CString &rDeviceName, CString &rDeviceID);
	void TestPlaybackDevice(UINT nWavID, BOOL bTestOn = TRUE);

	BOOL IsTesting() { return m_bTestingOn; };
private:
	AAudioDeviceManager			*m_ptrDeviceManager;
	IAudioDeviceCollection		*m_lpCollection;

private:
	BOOL m_bTestingOn;
};

