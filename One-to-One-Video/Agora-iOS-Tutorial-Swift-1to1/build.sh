
BUILD_DATE=`date +%Y-%m-%d-%H.%M.%S`
ArchivePath=Agora-iOS-Tutorial-${BUILD_DATE}.xcarchive

xcodebuild clean -project "Agora-iOS-Tutorial.xcodeproj" -scheme "Agora-iOS-Tutorial" -configuration Release
xcodebuild -project "Agora-iOS-Tutorial.xcodeproj" -scheme "Agora-iOS-Tutorial" -archivePath ${ArchivePath} archive
xcodebuild -exportArchive -exportOptionsPlist exportPlist.plist -archivePath ${ArchivePath} -exportPath .