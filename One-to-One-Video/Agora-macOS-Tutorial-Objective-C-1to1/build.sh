
BUILD_DATE=`date +%Y-%m-%d-%H.%M.%S`
ArchivePath=Agora-Mac-Tutorial-${BUILD_DATE}.xcarchive

xcodebuild clean -project "Agora-Mac-Tutorial-Objective-C.xcodeproj" -scheme "Agora-Mac-Tutorial-Objective-C" -configuration Release
xcodebuild CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO -project "Agora-Mac-Tutorial-Objective-C.xcodeproj" -scheme "Agora-Mac-Tutorial-Objective-C" -archivePath ${ArchivePath} archive
xcodebuild CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO -exportArchive -exportOptionsPlist exportPlist.plist -archivePath ${ArchivePath} -exportPath .
