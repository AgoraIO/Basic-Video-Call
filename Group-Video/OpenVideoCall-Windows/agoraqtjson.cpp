#include "agoraqtjson.h"
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QJsonParseError>
#include <QMessageBox>
#include <QJsonObject>
/////////////////////////////////////////////
/// \brief AgoraQtJson::GetAgoraQtJson
////////////////////////////////////////////

AgoraQtJson::AgoraQtJson(QObject *parent)
    : QObject(parent)
    , bSuccess(false)
    , isArray(false)
{
    QString path = QApplication::applicationDirPath();
    jsonFilePath = path + QString("/Parameters.json");

     QDir jsonDir(path);
     if(jsonDir.exists()){
         QFile jsonFile(jsonFilePath);
         if(jsonFile.open(QIODevice::ReadOnly)){
             QByteArray allData = jsonFile.readAll();
             jsonFile.close();
             QJsonParseError json_error;
             QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
             if(json_error.error == QJsonParseError::NoError){
               if(isArray = jsonDoc.isArray()){
                   jsParameters = jsonDoc.array();
                   bSuccess = true;
               }
             }
         }

     }
}

AgoraQtJson::~AgoraQtJson()
{

}

bool AgoraQtJson::GetParameters(std::map<std::string, std::string>& mapStringParameters,
       std::map<std::string, bool>& mapBoolParameters,
       std::map<std::string, int>& mapIntParameters,
       std::map<std::string, std::string>& mapObjectParameters)
{
    if (!bSuccess)
        return false;

    if(isArray){
         int size = jsParameters.size();
         for(int i=0; i<size; ++i){
             QJsonValue js = jsParameters[i];
             QJsonObject jsObj = js.toObject();
             QStringList keys = jsObj.keys();
             for(int j =0 ; j<keys.size(); ++j){
                 QString key = keys[j];
                 if(jsObj[key].isBool()){
                     bool b = jsObj[key].toBool();
                     mapBoolParameters.insert(std::make_pair(key.toUtf8().data(), b));
                 } else if(jsObj[key].isString()){
                     QString s = jsObj[key].toString();
                     mapStringParameters.insert(std::make_pair(key.toUtf8().data(), s.toStdString()));
                 }  else if (jsObj[key].isObject()){
                     mapObjectParameters.insert(std::make_pair(key.toUtf8().data(), jsObj[key].toString().toUtf8().data() ));
                 }
                 else {
                     int value = jsObj[key].toInt();
                     mapIntParameters.insert(std::make_pair(key.toUtf8().data(), value));
                 }
             }
         }
         return true;
    }else{
        QMessageBox::critical(nullptr, ("AgoraOpenLive"), ("Parameters file format"));
    }
    return false;
}
