#include "TcpTaskCmdVel.h"

using namespace MapTcp;

TcpTaskCmdVel::TcpTaskCmdVel(QObject *parent):
    TcpTaskBase(parent)
{
    isThreadRunning = false;
}

TcpTaskCmdVel::~TcpTaskCmdVel()
{

}


void MapTcp::TcpTaskCmdVel::run()
{

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket()) {
        this->isThreadRunning = false;
        return;
    }

    this->InitPacket(Neo_Packet::PacketType::CMDVEL, sizeof(Neo_Packet::CMDVEL_PACKAGE_POP), sizeof(Neo_Packet::CMDVEL_PACKAGE_ACK));


    if(this->p_socket->SendSockPackage(this->packet_send, sizeof(Neo_Packet::CMDVEL_PACKAGE_ACK), this->packet_recv) == this->PacketSend_Head->function_id)
    {
        printf("Goal cancel success...\n");
    }
    this->p_socket->closeSocket();
    this->isThreadRunning = false;
}
