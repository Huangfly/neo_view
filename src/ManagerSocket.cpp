#include "ManagerSocket.h"
#include "map_main.h"
#include "Map_View.h"
#include "socket/CTcpSocket.h"

using namespace MapTcp;

QTcpSocket *pt_socket = NULL;
QString sysIP = "192.168.1.128";
QString sysPort = "8888";

ManagerSocket::ManagerSocket():
    ThreadRobotStatus(sysIP,sysPort),
    ThreadDownloadMap(sysIP,sysPort),
    ThreadActionNode(sysIP,sysPort),
    ThreadGoal(sysIP,sysPort),
    ThreadLidar(sysIP,sysPort),
    ThreadCmdVel(sysIP,sysPort)
{
}

void ManagerSocket::OnDownloadMap()
{
    this->ThreadDownloadMap.startThread();
}

void ManagerSocket::OnRobotStatus()
{
    this->ThreadRobotStatus.startThread();
}

void ManagerSocket::OnSendGoal()
{
    this->ThreadGoal.startSendGoalThread();
}

void ManagerSocket::OnCancelGoal()
{
    this->ThreadGoal.startCancelGoalThread();
}

void ManagerSocket::OnActivateNode(QString str,bool enable)
{
    this->ThreadActionNode.startThread(str,enable);
}


void ManagerSocket::OnCmdVel(float x, float y, float z)
{
    this->ThreadCmdVel.startThread(x,y,z);
}

void ManagerSocket::OnDownloadLidarData()
{
    this->ThreadLidar.startThread();
}


