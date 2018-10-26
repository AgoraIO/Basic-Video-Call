#include "stdafx.h"
#include "AgoraCameraManager.h"


CAgoraCameraManager::CAgoraCameraManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
{
}


CAgoraCameraManager::~CAgoraCameraManager()
{
	Close();
}

BOOL CAgoraCameraManager::Create(IRtcEngine *lpRtcEngine)
{
//	lpRtcEngine->enableVideo();

	m_ptrDeviceManager = new AVideoDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return FALSE;

	m_lpCollection = (*m_ptrDeviceManager)->enumerateVideoDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void CAgoraCameraManager::Close()
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

UINT CAgoraCameraManager::GetDeviceCount()
{
	if (m_lpCollection == NULL)
		return 0;

	return (UINT)m_lpCollection->getCount();
}

CString CAgoraCameraManager::GetCurDeviceID()
{
	CString		str;
	CHAR		szDeviceID[MAX_DEVICE_ID_LENGTH];

	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return str;

	memset(szDeviceID, 0x00, MAX_DEVICE_ID_LENGTH);
	if (m_ptrDeviceManager != NULL && m_ptrDeviceManager->get() != NULL)
		(*m_ptrDeviceManager)->getDevice(szDeviceID);

#ifdef UNICODE
	::MultiByteToWideChar(CP_UTF8, 0, szDeviceID, -1, str.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);
	str.ReleaseBuffer();
#else
	strDeviceName = szDeviceID;
#endif

	return str;
}

BOOL CAgoraCameraManager::GetDevice(UINT nIndex, CString &rDeviceName, CString &rDeviceID)
{
	CHAR szDeviceName[MAX_DEVICE_ID_LENGTH];
	CHAR szDeviceID[MAX_DEVICE_ID_LENGTH];

	ASSERT(nIndex < GetDeviceCount());
	if (nIndex >= GetDeviceCount())
		return FALSE;

	if(m_lpCollection == NULL)
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

BOOL CAgoraCameraManager::SetCurDevice(LPCTSTR lpDeviceID)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL)
		return FALSE;

#ifdef UNICODE
	CHAR szDeviceID[128];
	::WideCharToMultiByte(CP_ACP, 0, lpDeviceID, -1, szDeviceID, 128, NULL, NULL);
	int nRet = (*m_ptrDeviceManager)->setDevice(szDeviceID);
#else
	int nRet = (*m_ptrDeviceManager)->setDevice(lpDeviceID);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

void CAgoraCameraManager::TestCameraDevice(HWND hVideoWnd, BOOL bTestOn)
{
	if (bTestOn && !m_bTestingOn) {
		ASSERT(hVideoWnd != NULL);
		CAgoraObject::GetAgoraObject()->LocalVideoPreview(hVideoWnd, TRUE);
	}
	else if(!bTestOn && m_bTestingOn){
		CAgoraObject::GetAgoraObject()->LocalVideoPreview(NULL, FALSE);
	}

	m_bTestingOn = bTestOn;
}