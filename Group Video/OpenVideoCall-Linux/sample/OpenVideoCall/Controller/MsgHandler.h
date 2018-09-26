#pragma once
#include <unordered_map>

class EngineController;

class MsgHandler {
    public:
        typedef bool (MsgHandler::*handler_ptr)(void*); 
        typedef std::unordered_map<int , handler_ptr> MsgHandlerMap_t;

        MsgHandler();
        ~MsgHandler();

        int onMsg(int msgUri, void* msg);
        int setController(EngineController* controller); 

    protected:
        void registerHandler(int msgUri, handler_ptr ptr);

    protected:
        MsgHandlerMap_t m_handlers;
        EngineController* m_controller;
};



