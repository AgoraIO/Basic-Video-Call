#include "AGCameraManager.h"
#include "IAgoraRtcEngine.h"
#include <memory.h>

AGCameraManager::AGCameraManager()
    : m_deviceManager(NULL)
      , m_collection(NULL) {
}

AGCameraManager::~AGCameraManager() {
    close();
}

bool AGCameraManager::create(IRtcEngine *rtcEngine) {
    m_deviceManager = new AVideoDeviceManager(rtcEngine);
    if (m_deviceManager == NULL || m_deviceManager->get() == NULL)
        return false;

    m_collection = (*m_deviceManager)->enumerateVideoDevices();
    if (m_collection == NULL) {
        delete m_deviceManager;
        m_deviceManager = NULL;
    }

    return m_collection != NULL ? true : false;
}

void AGCameraManager::close() {
    if (m_collection != NULL){
        m_collection->release();
        m_collection = NULL;
    }

    if (m_deviceManager != NULL) {
        delete m_deviceManager;
        m_deviceManager = NULL;
    }
}

uint8_t AGCameraManager::getDeviceCount() {
    if (m_collection == NULL)
        return 0;

    return (uint8_t)m_collection->getCount();
}

bool AGCameraManager::getCurDeviceId(string& id) {
    char deviceId[MAX_DEVICE_ID_LENGTH];

    if (m_deviceManager == NULL || m_deviceManager->get() == NULL)
        return false;

    memset(deviceId, 0x00, MAX_DEVICE_ID_LENGTH);
    if (m_deviceManager != NULL && m_deviceManager->get() != NULL)
        (*m_deviceManager)->getDevice(deviceId);

    id.assign(deviceId);

    return true;
}

bool AGCameraManager::getDevice(uint8_t index, string& deviceName, string& deviceId) {
    char name[MAX_DEVICE_ID_LENGTH];
    char id[MAX_DEVICE_ID_LENGTH];

    if (index >= getDeviceCount())
        return false;

    if(m_collection == NULL)
        return false;

    int ret = m_collection->getDevice(index, name, id);
    if (ret != 0)
        return false;

    deviceName.assign(name);
    deviceId.assign(id);

    return true;
}

bool AGCameraManager::setCurDevice(const char* deviceId)
{
    if (m_deviceManager == NULL || *m_deviceManager == NULL)
        return false;

    int ret = (*m_deviceManager)->setDevice(deviceId);

    return ret == 0 ? true : false;
}
