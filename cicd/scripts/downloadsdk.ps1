param($SDKURL)
Invoke-WebRequest -Uri "$SDKURL" -OutFile .\AgoraSDK.zip -TimeoutSec 10;