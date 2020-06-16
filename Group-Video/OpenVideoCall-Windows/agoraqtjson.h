#ifndef CAGORAQTJSON_H
#define CAGORAQTJSON_H

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <map>

class AgoraQtJson : public QObject
{
    Q_OBJECT
public:
    AgoraQtJson(QObject *parent = NULL);
    ~AgoraQtJson();

    bool GetParameters(std::map<std::string, std::string>& mapStringParameters,
           std::map<std::string, bool>& mapBoolParameters,
           std::map<std::string, int>& mapIntParameters,
           std::map<std::string, std::string>& mapObjectParameters);
private:
    QString jsonFilePath;
    QJsonArray jsParameters;
    bool  bSuccess;
    bool isArray;
};
#endif // CAGORACONFIG_H
