//
//  Agora SDK
//
//  Created by Sting Feng in 2017-11.
//  Copyright (c) 2017 Agora.io. All rights reserved.
//

#ifndef AGORA_SERVICE_H
#define AGORA_SERVICE_H
#include "AgoraBase.h"

namespace agora {
    namespace rtc {
        class IRtcEngine;
    }
    namespace signaling {
        class ISignalingEngine;
    }
namespace base {

struct AgoraServiceContext
{
};


class IAgoraService
{
public:
    virtual void release() = 0;
	
	/**
    * initialize the engine
    * @param [in] context
    *        the RTC engine context
    * @return return 0 if success or an error code
    */
    virtual int initialize(const AgoraServiceContext& context) = 0;

    /**
    * get the version information of the SDK
    * @param [in, out] build
    *        the build number
    * @return return the version number string in char format
    */
    virtual const char* getVersion(int* build) = 0;

    virtual rtc::IRtcEngine* createRtcEngine() = 0;
    virtual signaling::ISignalingEngine* createSignalingEngine() = 0;
};

} //namespace base
} // namespace agora

/**
* to get the version number of the SDK
* @param [in, out] build
*        the build number of Agora SDK
* @return returns the string of the version of the SDK
*/
AGORA_API const char* AGORA_CALL getAgoraSdkVersion(int* build);

/**
* create the RTC engine object and return the pointer
* @param [in] err
*        the error code
* @return returns the description of the error code
*/
AGORA_API const char* AGORA_CALL getAgoraSdkErrorDescription(int err);

/**
* create the Agora Service object and return the pointer
* @return returns pointer of the Agora Service object
*/
AGORA_API agora::base::IAgoraService* AGORA_CALL createAgoraService();

AGORA_API int AGORA_CALL setAgoraSdkExternalSymbolLoader(void* (*func)(const char* symname));

#endif
