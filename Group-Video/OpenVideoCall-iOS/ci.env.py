#!/usr/bin/python
# -*- coding: UTF-8 -*-
import re
import os

def main():
    SDK_URL = ""
    if "SDK_URL" in os.environ:
       SDK_URL = os.environ["SDK_URL"]
        
    TARGET_LIBS_ZIP = "agora_sdk.zip"
    TARGET_INTERNAL_FOLDER = "agora_sdk"

    #if need reset
    ZIP_STRUCTURE_FOLDER = "Agora_Native_SDK_for_iOS_FULL/libs"
    CRY_FRAMEWORK_NAME = "AgoraRtcCryptoLoader.framework"
    CRY_LIB_NAME = "libcrypto.a"
    FRAMEWORK_NAME = "AgoraRtcEngineKit.framework"
    APP_NAME = "OpenVideoCall"

    wget = "wget -q " + SDK_URL + " -O " + TARGET_LIBS_ZIP
    os.system(wget)
    
    unzip = "unzip -q " + TARGET_LIBS_ZIP + " -d " + TARGET_INTERNAL_FOLDER
    os.system(unzip)
    
    mv_rtc = "mv -f " + TARGET_INTERNAL_FOLDER + "/" + ZIP_STRUCTURE_FOLDER + "/" + FRAMEWORK_NAME + " \"" + APP_NAME +"\""
    os.system(mv_rtc)

    mv_ecy = "mv -f " + TARGET_INTERNAL_FOLDER + "/" + ZIP_STRUCTURE_FOLDER + "/" + CRY_FRAMEWORK_NAME + " \"" + APP_NAME +"\""
    os.system(mv_ecy)

    mv_lib = "mv -f " + TARGET_INTERNAL_FOLDER + "/" + ZIP_STRUCTURE_FOLDER + "/" + CRY_LIB_NAME + " \"" + APP_NAME +"\""
    os.system(mv_lib)

    appId = ""
    if "AGORA_APP_ID" in os.environ:
        appId = os.environ["AGORA_APP_ID"]
    token = ""

    #if need reset
    f = open("./OpenVideoCall/KeyCenter.swift", 'r+')
    content = f.read()

    #if need reset
    appString = "\"" + appId + "\""
    tokenString = "\"" + token + "\""
    contentNew = re.sub(r'<#Your App Id#>', appString, content)
    contentNew = re.sub(r'<#Temp Access Token#>', tokenString, contentNew)

    f.seek(0)
    f.write(contentNew)
    f.truncate()


if __name__ == "__main__":
    main()
