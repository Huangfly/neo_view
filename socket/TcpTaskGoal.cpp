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

    if( !this->p_socket->connectSocket() ){
        return;
    }
    if(this->isSendGoal){
        map_main *ctl = Win::GetMainWin();
        this->InitPacket(Neo_Packet::PacketType::SENDGOAL, sizeof(Neo_Packet::GOAL_PACKAGE_POP), sizeof(Neo_Packet::GOAL_PACKAGE_ACK));
        if(ctl->m_MapViewCtl.GetGoal((Neo_Packet::GOAL_PACKAGE_POP*)this->PacketSend_Body))
        {
            if(this->p_socket->SendSockPackage(this->packet_send,sizeof(Neo_Packet::GOAL_PACKAGE_ACK),this->packet_recv) == this->PacketSend_Head->function_id)
            {
                printf("Goal send success...\n");
            }
        }
    }else{

        this->InitPacket(Neo_Packet::PacketType::CANCELGOAL, sizeof(Neo_Packet::CANCELGOAL_PACKAGE_POP), sizeof(Neo_Packet::CANCELGOAL_PACKAGE_ACK));

        if(this->p_socket->SendSockPackage(this->packet_send,sizeof(Neo_Packet::CANCELGOAL_PACKAGE_ACK),this->packet_recv) == this->PacketSend_Head->function_id)
        {
            printf("Goal cancel success...\n");
        }
    }
    this->ReleasePacket();
    this->p_socket->closeSocket();
}
