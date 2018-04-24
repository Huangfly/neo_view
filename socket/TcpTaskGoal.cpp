#include "TcpTaskGoal.h"
#include "src/main.h"
#include "src/map_main.h"

using namespace MapTcp;

MapTcp::TcpTaskGoal::TcpTaskGoal(QObject *parent):
    TcpTaskBase(parent)
{
}

TcpTaskGoal::~TcpTaskGoal()
{

}

void TcpTaskGoal::startSendGoalThread(){
    if( !this->isRunning() ) {
        this->isSendGoal = true;
        this->start();
    }
}

void TcpTaskGoal::startCancelGoalThread(){
    if( !this->isRunning() ) {
        this->isSendGoal = false;
        this->start();
    }
}

void TcpTaskGoal::run()
{
    char pop_buf[80];
    char ack_buf[80];

    if( !this->p_socket->connectSocket(this) ){
        return;
    }
    if(this->isSendGoal){
        map_main *ctl = Win::GetMainWin();
        P_HEAD *head = (P_HEAD*)(pop_buf+1);
        GOAL_PACKAGE_POP *pop_package = (GOAL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
        //P_HEAD *ackhead = (P_HEAD*)(ack+1);
        //GOAL_PACKAGE_ACK *ack_package = (GOAL_PACKAGE_ACK*)(ack_buf+1+sizeof(P_HEAD));
        head->funcId = PACK_GOAL;
        head->size = sizeof(P_HEAD)+sizeof(GOAL_PACKAGE_POP);

        pop_buf[0] = 0xAA;
        pop_buf[1+sizeof(P_HEAD)+sizeof(GOAL_PACKAGE_POP)+1]=0xAB;
        if(ctl->m_MapViewCtl.GetGoal(pop_package))
        {
            if(this->p_socket->SendSockPackage(pop_buf,sizeof(GOAL_PACKAGE_ACK),ack_buf) == head->funcId)
            {
                printf("Goal send success...\n");
            }
        }
    }else{
        P_HEAD *head = (P_HEAD*)(pop_buf+1);
        CANCELGOAL_PACKAGE_POP *pop_package = (CANCELGOAL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
        head->funcId = PACK_CANCELGOAL;
        head->size = sizeof(P_HEAD)+sizeof(CANCELGOAL_PACKAGE_POP);
        pop_package->isAck = 1;

        pop_buf[0] = 0xAA;
        pop_buf[1+sizeof(P_HEAD)+sizeof(CANCELGOAL_PACKAGE_POP)+1]=0xAB;
        if(this->p_socket->SendSockPackage(pop_buf,sizeof(CANCELGOAL_PACKAGE_ACK),ack_buf) == head->funcId)
        {
            printf("Goal cancel success...\n");
        }
    }

    this->p_socket->closeSocket();
}
