#include "TcpTaskActionNode.h"

using namespace MapTcp;


MapTcp::TcpTaskActionNode::TcpTaskActionNode(QString ip, QString port, QObject *parent):
    TcpTaskBase(ip,port,parent)
{
    isThreadRunning = false;
}

TcpTaskActionNode::~TcpTaskActionNode()
{

}

void TcpTaskActionNode::run()
{

    char pop_buf[80];
    char ack_buf[80];

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket(this)) return;

    P_HEAD *head = (P_HEAD*)(pop_buf+1);
    NODECTL_PACKAGE_POP *pop_package = (NODECTL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
    head->funcId = PACK_NODECTL;
    head->size = sizeof(P_HEAD)+sizeof(NODECTL_PACKAGE_POP);
    memcpy(pop_package,&pop_body,sizeof(NODECTL_PACKAGE_POP));

    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(P_HEAD)+sizeof(NODECTL_PACKAGE_POP)+1]=0xAB;
    if(this->p_socket->SendSockPackage(pop_buf,sizeof(NODECTL_PACKAGE_ACK),ack_buf) == head->funcId)
    {
        printf("Node success...\n");
    }
    this->p_socket->closeSocket();
    this->isThreadRunning = false;
}
