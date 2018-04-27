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
    char pop_buf[80];
    char ack_buf[80];

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket(this)) {
        this->isThreadRunning = false;
        return;
    }

    P_HEAD *head = (P_HEAD*)(pop_buf+1);
    CMDVEL_PACKAGE_POP *pop_package = (CMDVEL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
    head->funcId = PACK_CMDVEL;
    head->msg_code = 0;
    head->size = sizeof(P_HEAD)+sizeof(CMDVEL_PACKAGE_POP);
    memcpy(pop_package,&pop_body,sizeof(CMDVEL_PACKAGE_POP));

    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(P_HEAD)+sizeof(CMDVEL_PACKAGE_POP)+1]=0xAB;
    if(this->p_socket->SendSockPackage(pop_buf,sizeof(CMDVEL_PACKAGE_ACK),ack_buf) == head->funcId)
    {
        printf("Goal cancel success...\n");
    }
    this->p_socket->closeSocket();
    this->isThreadRunning = false;
}
