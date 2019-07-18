#!/usr/bin/python
# -*- coding: UTF-8 -*-
import re
import os

def main():
    f = open("./app/build.gradle")
    content = f.read()
    x = re.sub(
        r'(dependencies {$)(.*)', r"\1\n    implementation 'io.agora.rtc:full-sdk:2.4.1'\2", content, flags=re.M)
    print(x)
    f.close()

    appId = ""
    if "AGORA_APP_ID" in os.environ:
      appId = os.environ["AGORA_APP_ID"]
    token = ""

    f = open("./app/src/main/res/values/strings_config.xml")
    content = f.read()
    contentNew = re.sub(r'<#YOUR APP ID#>', appId, content)
    contentNew = re.sub(r'#YOUR ACCESS TOKEN#', token, contentNew)
    print(contentNew)
    f.close()


if __name__ == "__main__":
  main()
