#include "openvideocall.h"
#include "avdevice.h"
#include "nettesting.h"
#include "nettestresult.h"
#include "inroom.h"
#include "enterroom.h"
#include <QApplication>
#include <IAgoraRtcEngine.h>
using namespace agora::rtc;

Q_DECLARE_METATYPE(USER_OFFLINE_REASON_TYPE)
Q_DECLARE_METATYPE(LocalVideoStats)
Q_DECLARE_METATYPE(RemoteVideoStats)
Q_DECLARE_METATYPE(RtcStats)
Q_DECLARE_METATYPE(LastmileProbeResult)

// Start of the application
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("openvideocall.ico"));

    qRegisterMetaType<USER_OFFLINE_REASON_TYPE>();
    qRegisterMetaType<LocalVideoStats>();
    qRegisterMetaType<RemoteVideoStats>();
    qRegisterMetaType<RtcStats>();
    qRegisterMetaType<LastmileProbeResult>();

    OpenVideoCall w;
    w.show();

    return a.exec();
}
