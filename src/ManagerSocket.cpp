#include "ManagerSocket.h"
#include "map_main.h"
#include "Map_View.h"

using namespace MapTcp;

QTcpSocket *pt_socket = NULL;
QString ManagerSocket::sysIP = "192.168.1.125";
QString ManagerSocket::sysPort = "8888";

ManagerSocket::ManagerSocket()
{
    setIpPort(sysIP,sysPort);
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

void ManagerSocket::OnLoadMap()
{
    this->ThreadLoadMap.startThread();
}

void ManagerSocket::OnGlobalPath()
{
    this->ThreadGlobalPath.startThread();
}

void ManagerSocket::setIpPort(QString ip, QString port)
{
    this->sysIP = ip;
    this->sysPort = port;
    this->ThreadActionNode.p_socket->setIpPort(ip,port);
    this->ThreadCmdVel.p_socket->setIpPort(ip,port);
    this->ThreadDownloadMap.p_socket->setIpPort(ip,port);
    this->ThreadGoal.p_socket->setIpPort(ip,port);
    this->ThreadLidar.p_socket->setIpPort(ip,port);
    this->ThreadRobotStatus.p_socket->setIpPort(ip,port);
    this->ThreadLoadMap.p_socket->setIpPort(ip,port);
    this->ThreadGlobalPath.p_socket->setIpPort(ip,port);
}



