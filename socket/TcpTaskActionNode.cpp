#include "TcpTaskActionNode.h"

using namespace MapTcp;


MapTcp::TcpTaskActionNode::TcpTaskActionNode(QObject *parent):
    TcpTaskBase(parent)
{
    isThreadRunning = false;
}

TcpTaskActionNode::~TcpTaskActionNode()
{

}

void TcpTaskActionNode::run()
{
    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket()) {
        this->isThreadRunning = false;
        return;
    }

    this->InitPacket(Neo_Packet::PacketType::ROSCONTROL,sizeof(Neo_Packet::NODECTL_PACKAGE_POP),sizeof(Neo_Packet::NODECTL_PACKAGE_ACK),0);

    Neo_Packet::NODECTL_PACKAGE_POP *send_body = (Neo_Packet::NODECTL_PACKAGE_POP *)this->PacketSend_Body;

    send_body->enable = pop_body.enable;
    memcpy(send_body->node_name,this->pop_body.node_name,sizeof(this->pop_body.node_name));

    if(this->p_socket->SendSockPackage(this->packet_send,sizeof(Neo_Packet::NODECTL_PACKAGE_ACK),this->packet_recv, 20000) == this->PacketRecv_Head->function_id)
    {
        printf("%s %d Node success...\n",send_body->node_name,send_body->enable);
    }
    this->p_socket->closeSocket();
    this->isThreadRunning = false;
}
