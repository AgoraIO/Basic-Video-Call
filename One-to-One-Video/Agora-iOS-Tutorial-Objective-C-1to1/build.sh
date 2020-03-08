
BUILD_DATE=`date +%Y-%m-%d-%H.%M.%S`
ArchivePath=Agora-iOS-Tutorial-${BUILD_DATE}.xcarchive

xcodebuild clean -project "Agora-iOS-Tutorial-Objective-C.xcodeproj" -scheme "Agora-iOS-Tutorial-Objective-C" -configuration Release
xcodebuild -project "Agora-iOS-Tutorial-Objective-C.xcodeproj" -scheme "Agora-iOS-Tutorial-Objective-C" -archivePath ${ArchivePath} archive
xcodebuild -exportArchive -exportOptionsPlist exportPlist.plist -archivePath ${ArchivePath} -exportPath .
