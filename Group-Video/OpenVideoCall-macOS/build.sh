PROJECT_NAME="OpenVideoCall.xcodeproj"
SCHEME_NAME="OpenVideoCall"
BUILD_DATE=`date +%Y-%m-%d-%H.%M.%S`

ArchivePath=${SCHEME_NAME}-${BUILD_DATE}.xcarchive
BUILD_DATE=`date +%Y-%m-%d-%H.%M.%S`
ArchivePath=${SCHEME_NAME}-${BUILD_DATE}.xcarchive

xcodebuild clean -project ${PROJECT_NAME} -scheme ${SCHEME_NAME} -configuration Release
xcodebuild CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO -project ${PROJECT_NAME} -scheme ${SCHEME_NAME} -archivePath ${ArchivePath} archive
xcodebuild CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO CODE_SIGNING_ALLOWED=NO -exportArchive -exportOptionsPlist exportPlist.plist -archivePath ${ArchivePath} -exportPath .
