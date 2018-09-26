#include "AgoraDefs.h"
#include "OpenVideoCallApp.h"

#include "View/CommandLineView.h"
#include "Controller/EngineController.h"
#include "EngineModel/AGEngineModel.h"

OpenVideoCallApp::OpenVideoCallApp() {
    m_view = new CommandLineView();  
    m_controller = new EngineController();

    m_controller->setView(m_view);
    m_controller->setModel(AGEngineModel::Get());
}

OpenVideoCallApp::~OpenVideoCallApp() {
    if(m_view) {
        delete m_view;
        m_view = NULL;
    }

    if(m_controller) {
        m_controller->setView(NULL);
        m_controller->setModel(NULL);
        delete m_controller;
        m_controller = NULL;
    }
}

void OpenVideoCallApp::loadConfig(const AppConfig& cfg) {
    m_view->configure(cfg);
}

void OpenVideoCallApp::run(bool open) {
    AGEngineModel::Get()->initialize();
    m_view->run(open);
}

