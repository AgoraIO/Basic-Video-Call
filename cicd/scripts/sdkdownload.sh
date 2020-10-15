TARGET_LIBS_ZIP="agora_sdk.zip"
TARGET_INTERNAL_FOLDER="agora_sdk"

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

FIRST_FOLDER=""

for i in `ls`
do
    FIRST_FOLDER=$i
    echo $i
done

#copy
DESTINATION=${FILE_PATH}/../../${COPY_TO}

echo DESTINATION ${DESTINATION}

cp -af ${FIRST_FOLDER}/libs/* ${DESTINATION}

echo pwd `pwd`
