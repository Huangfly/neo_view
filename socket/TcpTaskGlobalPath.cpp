#include "TcpTaskGlobalPath.h"
#include "src/main.h"
#include "src/map_main.h"

using namespace Win;
using namespace MapTcp;

MapTcp::TcpTaskGlobalPath::TcpTaskGlobalPath(QObject *parent):
    TcpTaskBase(parent)
{

}

MapTcp::TcpTaskGlobalPath::~TcpTaskGlobalPath()
{

}

void MapTcp::TcpTaskGlobalPath::run()
{

    if(!this->p_socket->connectSocket(this)) {
        return;
    }


    map_main *ctl = Win::GetMainWin();
    this->InitPacket(PACK_GLOBALPATH,sizeof(GLOBALPATH_PACKAGE_POP),sizeof(GLOBALPATH_PACKAGE_ACK));

    GLOBALPATH_PACKAGE_ACK *recv_body = (GLOBALPATH_PACKAGE_ACK *)this->PacketRecv_Body;

    if(this->p_socket->SendSockPackage(this->packet_send,sizeof(GLOBALPATH_PACKAGE_ACK),this->packet_recv) == this->PacketSend_Head->funcId)
    {
        printf("GlobalPath send success...%d\n",recv_body->path_size);
        paths.clear();
        QVector2D point;
        for(int i = 0; i<recv_body->path_size; i++){
            point.setX(recv_body->path[i].x);
            point.setY(recv_body->path[i].y);
            paths.push_back(point);
        }
        ctl->m_MapViewCtl.updateGlobalPath(paths);
    }

    this->p_socket->closeSocket();
}
