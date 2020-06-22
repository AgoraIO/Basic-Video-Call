#include "stdafx.h"
#include "AGJson.h"
#include <fstream>
CAGJson::CAGJson()
{
     TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
    LPTSTR lpLastSlash = _tcsrchr(szFilePath, _T('\\')) + 1;
    szFilePath[lpLastSlash - szFilePath] = 0;
    CString strPath = szFilePath;
    strPath += _T("Parameters.json");

    if (PathFileExists(strPath)) {
        char szFile[MAX_PATH] = { 0 };
        WideCharToMultiByte(CP_ACP, 0, strPath, -1, szFile, MAX_PATH, NULL, NULL);
        std::ifstream ifs(szFile);
        Json::CharReaderBuilder readerBuilder;
        JSONCPP_STRING errs;
        bSuccess = Json::parseFromStream(readerBuilder, ifs, &jsonParameters, &errs);
    }
}

CAGJson::~CAGJson()
{
}

BOOL CAGJson::GetParameters(std::map<std::string, std::string>& mapStringParameters,
    std::map<std::string, bool>& mapBoolParameters,
    std::map<std::string, int>& mapIntParameters,
    std::map<std::string, std::string>& mapObjectParameters)
{
    if (!bSuccess) {
        return FALSE;
    }

    if (jsonParameters.isArray()) {
        int size = jsonParameters.size();
        for (size_t i = 0; i < size; i++) {
            Json::Value js = jsonParameters[(int)i];
            std::vector<std::string> keys = js.getMemberNames();
            if (js[keys[0].c_str()].isBool()) {
                bool b = js[keys[0].c_str()].asBool();
                mapBoolParameters.insert(std::make_pair(keys[0], b));
            }
            else if(js[keys[0].c_str()].isInt())
                mapIntParameters.insert(std::make_pair(keys[0], js[keys[0].c_str()].asInt()));
            else if (js[keys[0].c_str()].isString())
                mapStringParameters.insert(std::make_pair(keys[0], js[keys[0].c_str()].asString()));
            else if (js[keys[0].c_str()].isObject())
                mapObjectParameters.insert(std::make_pair(keys[0], js[keys[0].c_str()].toStyledString()));

        }
        return TRUE;
    }
    else {
        AfxMessageBox(_T("json string must be array"));
    }

    return FALSE;
}
