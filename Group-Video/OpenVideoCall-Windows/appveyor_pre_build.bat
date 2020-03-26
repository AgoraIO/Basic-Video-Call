set SDKVersion=%~1
set FolderVersion=%~2
set Machine=%~3

curl -fsSL -o AgoraSDK.zip https://download.agora.io/sdk/release/Agora_Native_SDK_for_Windows(%Machine%)_v3_0_0_FULL.zip
7z x AgoraSDK.zip -oAgoraSDK
mkdir sdk
xcopy /S /I AgoraSDK\Agora_Native_SDK_for_Windows_v3.0.0_FULL\sdk sdk
dir
call appveyor_build_release.bat %FolderVersion% %Machine%