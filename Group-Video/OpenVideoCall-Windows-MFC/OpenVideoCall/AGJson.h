#pragma once
#include <map>
#include "json/json.h"
class CAGJson
{
public:
    CAGJson();
    ~CAGJson();
    BOOL GetParameters(std::map<std::string, std::string>& mapStringParameters,
        std::map<std::string, bool>& mapBoolParameters,
        std::map<std::string, int>& mapIntParameters,
        std::map<std::string, std::string>& mapObjectParameters);
private:
    Json::Value jsonParameters;
    bool bSuccess = false;

};

