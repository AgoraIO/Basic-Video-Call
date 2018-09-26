#pragma once
#include<cstdint>
#include<string>

namespace agora {
    namespace rtc {
        class IRtcEngine;
        class AVideoDeviceManager;
        class IVideoDeviceCollection;
    }
}

using namespace agora::rtc;
using std::string;

class AGCameraManager 
{
    public:
        AGCameraManager();
        ~AGCameraManager();

        bool create(IRtcEngine* rtcEngine);
        void close();

        uint8_t getDeviceCount();

        bool getCurDeviceId(string& id);
        bool setCurDevice(const char* deviceId);

        bool getDevice(uint8_t index, string& deviceName, string& deviceId);
    private:
        AVideoDeviceManager			*m_deviceManager;
        IVideoDeviceCollection		*m_collection;
};

