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
    if(!this->p_socket->connectSocket(this)) {
        this->isThreadRunning = false;
        return;
    }

    this->InitPacket(PACK_NODECTL,sizeof(NODECTL_PACKAGE_POP),sizeof(NODECTL_PACKAGE_ACK),0);

    NODECTL_PACKAGE_POP *send_body = (NODECTL_PACKAGE_POP *)this->PacketSend_Body;

    send_body->enable = pop_body.enable;
    memcpy(send_body->node_name,this->pop_body.node_name,sizeof(this->pop_body.node_name));

    if(this->p_socket->SendSockPackage(this->packet_send,sizeof(NODECTL_PACKAGE_ACK),this->packet_recv) == this->PacketRecv_Head->funcId)
    {
        printf("%s %d Node success...\n",send_body->node_name,send_body->enable);
    }
    this->p_socket->closeSocket();
    this->isThreadRunning = false;
}
