#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QObject>
#include <QThread>
#include "map_socket_type.h"
#include "socket/TcpTaskRobotStatus.h"
#include "socket/TcpTaskDownloadMap.h"
#include "socket/TcpTaskCmdVel.h"
#include "socket/TcpTaskActionNode.h"
#include "socket/TcpTaskGoal.h"
#include "socket/TcpTaskLidar.h"
#include "socket/TcpTaskLoadMap.h"
#include "socket/TcpTaskGlobalPath.h"

using namespace MapTcp;
namespace MapTcp {
    class ManagerSocket : public QObject
    {
        Q_OBJECT
    public:
        explicit ManagerSocket();
        void OnDownloadMap();
        void OnRobotStatus();
        void OnSendGoal();
        void OnCancelGoal();
        void OnActivateNode(QString str,bool enable);
        void OnCmdVel(float x,float y,float z);
        void OnDownloadLidarData();
        void OnLoadMap();
        void OnGlobalPath();

        void setIpPort(QString ip, QString port);


        static QString sysIP;
        static QString sysPort;
    signals:

    public slots:
    private:
        MapTcp::TcpTaskRobotStatus ThreadRobotStatus;
        MapTcp::TcpTaskDownloadMap ThreadDownloadMap;
        MapTcp::TcpTaskActionNode ThreadActionNode;
        MapTcp::TcpTaskGoal ThreadGoal;
        MapTcp::TcpTaskLidar ThreadLidar;
        MapTcp::TcpTaskCmdVel ThreadCmdVel;
        MapTcp::TcpTaskLoadMap ThreadLoadMap;
        MapTcp::TcpTaskGlobalPath ThreadGlobalPath;
    };
}
#endif // SOCKETTHREAD_H
