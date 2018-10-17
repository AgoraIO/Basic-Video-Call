#include<stdlib.h>

#include"EngineController.h"
#include"MsgHandler.h"

EngineController::EngineController():m_model(NULL), m_view(NULL){
}

EngineController::~EngineController(){
    m_model = NULL;
    m_view = NULL;
}

void EngineController::setModel(MsgHandler* model) {
    m_model = model;
    if(m_model)
        m_model->setController(this);
}

void EngineController::setView(MsgHandler* view) {
    m_view = view;
    if(m_view)
        m_view->setController(this);
}

bool EngineController::sendMsg(MsgHandler* sender, int msgUri, void* msg) {
    if((sender == m_model) && m_view) {
        return m_view->onMsg(msgUri,msg);
    } else if((sender == m_view) && m_model) {
        return m_model->onMsg(msgUri,msg);
    }

    return false;
}

