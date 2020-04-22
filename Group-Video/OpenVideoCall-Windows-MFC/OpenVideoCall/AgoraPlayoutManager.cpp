#include "stdafx.h"
#include "AgoraPlayoutManager.h"
#include "AGResourceVisitor.h"

CAgoraPlayoutManager::CAgoraPlayoutManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
{
}


CAgoraPlayoutManager::~CAgoraPlayoutManager()
{
	Close();
}

BOOL CAgoraPlayoutManager::Create(IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new AAudioDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return FALSE;

	m_lpCollection = (*m_ptrDeviceManager)->enumeratePlaybackDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void CAgoraPlayoutManager::Close()
{
	if (m_lpCollection != NULL){
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}
}

UINT CAgoraPlayoutManager::GetVolume()
{
	int nVol = 0;

	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return 0;

	(*m_ptrDeviceManager)->getPlaybackDeviceVolume(&nVol);

	return (UINT)nVol;
}

BOOL CAgoraPlayoutManager::SetVolume(UINT nVol)
{
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return 0;

	int nRet = (*m_ptrDeviceManager)->setPlaybackDeviceVolume((int)nVol);

	return nRet == 0 ? TRUE : FALSE;
}

UINT CAgoraPlayoutManager::GetDeviceCount()
{
	if (m_lpCollection == NULL)
		return 0;

	return (UINT)m_lpCollection->getCount();
}

BOOL CAgoraPlayoutManager::GetDevice(UINT nIndex, CString &rDeviceName, CString &rDeviceID)
{
	CHAR szDeviceName[MAX_DEVICE_ID_LENGTH];
	CHAR szDeviceID[MAX_DEVICE_ID_LENGTH];

	ASSERT(nIndex < GetDeviceCount());
	if (nIndex >= GetDeviceCount())
		return FALSE;

	int nRet = m_lpCollection->getDevice(nIndex, szDeviceName, szDeviceID);
	if (nRet != 0)
		return FALSE;

#ifdef UNICODE
	::MultiByteToWideChar(CP_UTF8, 0, szDeviceName, -1, rDeviceName.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);
	::MultiByteToWideChar(CP_UTF8, 0, szDeviceID, -1, rDeviceID.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);

	rDeviceName.ReleaseBuffer();
	rDeviceID.ReleaseBuffer();
#else
	strDeviceName = szDeviceName;
	strDeviceID = szDeviceID;
#endif

	return TRUE;
}

CString CAgoraPlayoutManager::GetCurDeviceID()
{
	CString		str;
	CHAR		szDeviceID[MAX_DEVICE_ID_LENGTH];
	
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return str;

	(*m_ptrDeviceManager)->getPlaybackDevice(szDeviceID);

#ifdef UNICODE
	::MultiByteToWideChar(CP_UTF8, 0, szDeviceID, -1, str.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);
	str.ReleaseBuffer();
#else
	strDeviceName = szDeviceID;
#endif

	return str;
}

BOOL CAgoraPlayoutManager::SetCurDevice(LPCTSTR lpDeviceID)
{
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return FALSE;

#ifdef UNICODE
	CHAR szDeviceID[128];
	::WideCharToMultiByte(CP_ACP, 0, lpDeviceID, -1, szDeviceID, 128, NULL, NULL);
	int nRet = (*m_ptrDeviceManager)->setPlaybackDevice(szDeviceID);
#else
	int nRet = (*m_ptrDeviceManager)->setPlaybackDevice(lpDeviceID);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

void CAgoraPlayoutManager::TestPlaybackDevice(UINT nWavID, BOOL bTestOn)
{
	TCHAR	szWavPath[MAX_PATH];

	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return;

	::GetModuleFileName(NULL, szWavPath, MAX_PATH);
	LPTSTR lpLastSlash = (LPTSTR)_tcsrchr(szWavPath, _T('\\')) + 1;
	_tcscpy_s(lpLastSlash, 16, _T("test.wav"));

	if (bTestOn && !m_bTestingOn) {
		CAGResourceVisitor::SaveResourceToFile(_T("WAVE"), nWavID, szWavPath);

#ifdef UNICODE
		CHAR szWavPathA[MAX_PATH];

		::WideCharToMultiByte(CP_ACP, 0, szWavPath, -1,szWavPathA , MAX_PATH, NULL, NULL);
		(*m_ptrDeviceManager)->startPlaybackDeviceTest(szWavPathA);
#else
		(*m_ptrDeviceManager)->startPlaybackDeviceTest(szWavPathA);
#endif
	}
	else if (!bTestOn && m_bTestingOn)
		(*m_ptrDeviceManager)->stopPlaybackDeviceTest();

	m_bTestingOn = bTestOn;

}