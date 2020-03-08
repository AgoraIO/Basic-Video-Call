#ifndef AGORAWINDOWMANAGER_H
#define AGORAWINDOWMANAGER_H

#include <QMainWindow>
#include <QVariant>
#include <mutex>

enum eQMainWndType {
    qWndType_NULL = -1,
    qWndType_OpenVideoCall,
    qWndType_AvDevice,
    qWndType_NetTesting,
    qWndType_NetTestResult,
    qWndType_NetTestDetail,
    qWndType_InRoom,
    qWndType_EnterRoom,

    qWndType_UNKNOWN = 0xff,
};

typedef QMap<eQMainWndType,QMainWindow*> qMapeq;
class AgoraWindowManager
{
public:
    static AgoraWindowManager* getInstance();
    static void releaseInstance();
    void insertItem(eQMainWndType type,QMainWindow* pWnd);
    QMainWindow* getQWnd(eQMainWndType type);

private:
    AgoraWindowManager();
    ~AgoraWindowManager();
    static AgoraWindowManager* m_pInstance;
    qMapeq  m_qmapeq;
    static std::mutex  m_mutexMgr;
};

#endif // AGORAWINDOWMANAGER_H
