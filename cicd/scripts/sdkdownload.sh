TARGET_LIBS_ZIP="agora_sdk.zip"
TARGET_INTERNAL_FOLDER="agora_sdk"
TARGET_INTERNAL_STATIC_FOLDER="agora_static_sdk"

FILE_PATH=`pwd`

echo ${FILE_PATH}

SDK_URL=$1
COPY_TO=$2

echo "url" ${SDK_URL}

#download
wget -q ${SDK_URL} -O ${TARGET_LIBS_ZIP}

#unzip
unzip -q ${TARGET_LIBS_ZIP} -d ${TARGET_INTERNAL_FOLDER}

cd ${TARGET_INTERNAL_FOLDER}

FIRST_UNZIP_FOLDER=""

for i in `ls`
do
    FIRST_UNZIP_FOLDER=$i
    echo $i
done

cd ${FIRST_UNZIP_FOLDER}

DYNAMIC_SDK_TAG="Dynamic"
STATIC_SDK=""

for i in `ls`
do
    if [[ $i =~ ${DYNAMIC_SDK_TAG} ]]
    then
    echo "Dynamic" $i
    else
    STATIC_SDK=$i
    echo "Static" ${STATIC_SDK}
fi
done

unzip -q ${STATIC_SDK} -d ${TARGET_INTERNAL_STATIC_FOLDER}

cd ${TARGET_INTERNAL_STATIC_FOLDER}

SECOND_UNZIP_FOLDER=""

for i in `ls`
do
    SECOND_UNZIP_FOLDER=$i
    echo ${SECOND_UNZIP_FOLDER}
done

echo `pwd`
echo SECOND_UNZIP_FOLDER ${SECOND_UNZIP_FOLDER}

#copy
DESTINATION=${FILE_PATH}/../../${COPY_TO}

echo DESTINATION ${DESTINATION}

cp -af ${SECOND_UNZIP_FOLDER}/libs/* ${DESTINATION}

echo pwd `pwd`
