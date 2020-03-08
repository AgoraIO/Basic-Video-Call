#include "agorawindowmanager.h"

AgoraWindowManager* AgoraWindowManager::m_pInstance = Q_NULLPTR;
std::mutex  AgoraWindowManager::m_mutexMgr;

AgoraWindowManager* AgoraWindowManager::getInstance()
{
    std::lock_guard<std::mutex> autolock(m_mutexMgr);
    if(Q_NULLPTR == m_pInstance)
        m_pInstance = new AgoraWindowManager;

    return m_pInstance;
}

void AgoraWindowManager::releaseInstance()
{
    std::lock_guard<std::mutex> autoLock(m_mutexMgr);
    if(Q_NULLPTR != m_pInstance)
        delete m_pInstance;

    m_pInstance = Q_NULLPTR;
}

AgoraWindowManager::AgoraWindowManager()
{

}

AgoraWindowManager::~AgoraWindowManager()
{
}

void AgoraWindowManager::insertItem(eQMainWndType type,QMainWindow* pWnd)
{
    std::lock_guard<std::mutex> autolock(m_mutexMgr);
    m_qmapeq[type] = pWnd;
}

QMainWindow* AgoraWindowManager::getQWnd(eQMainWndType type)
{
    std::lock_guard<std::mutex> autolock(m_mutexMgr);
    QMainWindow* pWndTemp = Q_NULLPTR;
    if(m_qmapeq.contains(type))
        pWndTemp = m_qmapeq.value(type);

    return pWndTemp;
}
