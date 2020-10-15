#include "stdafx.h"
#include "LanguageSet.h"

CLanguageSet *CLanguageSet::m_lpLanguageSet = NULL;
TCHAR CLanguageSet::m_szSelfPath[MAX_PATH];

CLanguageSet::CLanguageSet()
{
}


CLanguageSet::~CLanguageSet()
{
}

CLanguageSet *CLanguageSet::GetInstance()
{
	if (CLanguageSet::m_lpLanguageSet == NULL)
		CLanguageSet::m_lpLanguageSet = new CLanguageSet();

    ::GetModuleFileName(NULL, CLanguageSet::m_szSelfPath, MAX_PATH);
    LPTSTR lpLastSlash = _tcsrchr(CLanguageSet::m_szSelfPath, _T('\\')) + 1;
    _tcscpy_s(lpLastSlash, MAX_PATH, _T("Language\\"));

	CString strLanguageName = CLanguageSet::m_lpLanguageSet->GetCurrentLanguage();

    CLanguageSet::m_lpLanguageSet->LoadLanguageModule(strLanguageName);

	return CLanguageSet::m_lpLanguageSet;
}

void CLanguageSet::CloseInstance()
{
	CLanguageSet::m_lpLanguageSet->UnloadLanguageModule();
	delete CLanguageSet::m_lpLanguageSet;

	CLanguageSet::m_lpLanguageSet = NULL;
}

CString CLanguageSet::GetCurrentLanguage()
{
	CString	strCurrent;
	TCHAR	szProfilePath[MAX_PATH];

	_tcscpy_s(szProfilePath, MAX_PATH, CLanguageSet::m_szSelfPath);
	_tcscat_s(szProfilePath, MAX_PATH, _T("Language.ini"));

	::GetPrivateProfileString(_T("language"), _T("current"), _T("english.dll"), strCurrent.GetBuffer(MAX_PATH), MAX_PATH, szProfilePath);

	strCurrent.ReleaseBuffer();

	return strCurrent;
}

BOOL CLanguageSet::SetCurrentLanguage(LPCTSTR lpLanguageName)
{
	TCHAR	szProfilePath[MAX_PATH];
	TCHAR	szModulePath[MAX_PATH];

	_tcscpy_s(szProfilePath, MAX_PATH, CLanguageSet::m_szSelfPath);
	_tcscat_s(szProfilePath, _T("Language.ini"));

	_tcscpy_s(szModulePath, MAX_PATH, CLanguageSet::m_szSelfPath);
	_tcscat_s(szModulePath, MAX_PATH, lpLanguageName);
	_tcscat_s(szModulePath, MAX_PATH, _T(".dll"));
	if (::GetFileAttributes(szModulePath) == INVALID_FILE_ATTRIBUTES)
		return FALSE;

	return ::WritePrivateProfileString(_T("language"), _T("current"), lpLanguageName, szProfilePath);
}

LPCTSTR CLanguageSet::GetString(LPCSTR lpStringID)
{
	ASSERT(m_hLanguageModule != NULL);

	LPCTSTR *lpStrAddr = (LPCTSTR *)::GetProcAddress(m_hLanguageModule, lpStringID);

	return *lpStrAddr;// ::GetProcAddress(m_hLanguageModule, lpStringID);
}

BOOL CLanguageSet::LoadLanguageModule(LPCTSTR lpLanguageName)
{
	TCHAR	szModulePath[MAX_PATH];
	LPTSTR	lpLastSlash = NULL;

	_tcscpy_s(szModulePath, MAX_PATH, CLanguageSet::m_szSelfPath);

	if (lpLanguageName == NULL)
		_tcscat_s(szModulePath, MAX_PATH, GetCurrentLanguage());
	else
		_tcscat_s(szModulePath, MAX_PATH, lpLanguageName);

//	_tcscat_s(szModulePath, MAX_PATH, _T(".dll"));
	if (::GetFileAttributes(szModulePath) == INVALID_FILE_ATTRIBUTES)
		return FALSE;

	m_hLanguageModule = ::LoadLibrary(szModulePath);

	return m_hLanguageModule != NULL ? TRUE : FALSE;
}

void CLanguageSet::UnloadLanguageModule()
{
	if (m_hLanguageModule != NULL)
		::FreeLibrary(m_hLanguageModule);

	m_hLanguageModule = NULL;
}