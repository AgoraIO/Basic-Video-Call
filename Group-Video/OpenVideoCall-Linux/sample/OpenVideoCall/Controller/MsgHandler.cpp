#include"MsgHandler.h"
#include"EngineController.h"

MsgHandler::MsgHandler() {
}

MsgHandler::~MsgHandler() {
}

void MsgHandler::registerHandler(int msg, handler_ptr ptr) {
    m_handlers[msg] = ptr;
}

int MsgHandler::onMsg(int msgUri, void* msg) {
    MsgHandlerMap_t::iterator it = m_handlers.find(msgUri);
    if(it != m_handlers.end()) {
        handler_ptr ptr = it->second; 
        return (this->*ptr)(msg);
    }

    return 0;
}

int MsgHandler::setController(EngineController* controller) {
    m_controller = controller;
}
