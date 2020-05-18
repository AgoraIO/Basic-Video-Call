#pragma once
class CAGConfig
{
public:
	CAGConfig();
	~CAGConfig();

	int GetCodec();
	BOOL SetCodec(int nCodec);

/*	int GetResolution();
	BOOL SetResolution(int nResolution);

	int GetFPS();
	BOOL SetFPS(int nPFS);

	int GetMaxRate();
	BOOL SetMaxRate(int nMaxRate);
*/
	BOOL EnableAutoSave(BOOL bEnable);
	BOOL IsAutoSaveEnabled();

    BOOL SetSolution(int nIndex);
    int GetSolution();

    BOOL IsCustomFPS();
    BOOL IsCustomBitrate();
    BOOL IsCustomRsolution();

    void SetCustomFPS(int fps);
    int  GetCustomFPS();

    void SetResolution(int w, int h);
    BOOL GetResolution(int& w, int& h);

    void SetCustomBitrate(int bitrate);
    int  GetCustomBitrate();
private:
	TCHAR m_szConfigFile[MAX_PATH];
};

