#!/usr/bin/python
# -*- coding: UTF-8 -*-
import re
import os

def main():

    appId = ""
    if "AGORA_APP_ID" in os.environ:
        appId = os.environ["AGORA_APP_ID"]
    token = ""

    #if need reset
    f = open("./OpenVideoCall/KeyCenter.m", 'r+')
    content = f.read()

    #if need reset
    appString = "@\"" + appId + "\""
    tokenString = "@\"" + token + "\""
    contentNew = re.sub(r'<#Your App Id#>', appString, content)
    contentNew = re.sub(r'<#Temp Access Token#>', tokenString, contentNew)

    f.seek(0)
    f.write(contentNew)
    f.truncate()


if __name__ == "__main__":
    main()
