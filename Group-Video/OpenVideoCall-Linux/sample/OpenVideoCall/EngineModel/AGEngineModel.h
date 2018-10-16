#pragma once

#include"Controller/MsgHandler.h"
#include"IAGEventReceiver.h"
#include"AGEngineEventHandler.h"
#include"AgoraDefs.h"

class EngineController;
class AGEngine;
class AGCameraManager;

class AGEngineModel : public MsgHandler
                      ,public IAGEventReceiver
{
    public:
        static AGEngineModel* Get();

        //event callback handler
        void onEvent(int id, void* pData);

        void initialize();

    private:
        AGEngineModel();
        ~AGEngineModel();

        //message handler
        bool onOpenMsg(void* msg);
        bool onCloseMsg(void* msg);
        bool onEnableVideoMsg(void* msg);
        bool onEnableAudioMsg(void* msg);
        bool onMuteLocalVideoMsg(void* msg);
        bool onMuteLocalAudioMsg(void* msg);
        bool onEnableLocalVideoMsg(void* msg);
        bool onConfigureMsg(void* msg);
        bool onPrintDeviceInfoMsg(void* msg);
        bool onSetCurCameraMsg(void* msg); 
        bool onExitMsg(void* msg); 
        bool onMuteRemote(void *msg);

        void release(); 
    private:
        AGEngineEventHandler m_engineEventHandler;
        AGEngine*   m_engine;
        AGCameraManager*  m_cameraMgr;

        AppConfig m_cfg;

    private:
        static AGEngineModel m_model; 
};

