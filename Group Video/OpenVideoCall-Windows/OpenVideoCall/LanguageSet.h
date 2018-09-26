#pragma once

//#define LANGUAGE_SET_MSG _T("SNOWWALKER_LANGSET_MSG")

#define LANG_STR(lpStringID) (CLanguageSet::GetInstance()->GetString(lpStringID))

class CLanguageSet
{
protected:
	CLanguageSet();
	~CLanguageSet();

public:
	static CLanguageSet *GetInstance();
	static void CloseInstance();

	CString GetCurrentLanguage();
	BOOL	SetCurrentLanguage(LPCTSTR lpLanguageName);
	LPCTSTR GetString(LPCSTR lpStringID);

	BOOL LoadLanguageModule(LPCTSTR lpLanguageName = NULL);
	void UnloadLanguageModule();

private:
	HMODULE				m_hLanguageModule;
	static CLanguageSet *m_lpLanguageSet;
	static TCHAR		m_szSelfPath[MAX_PATH];
};

