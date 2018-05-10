#include "TcpTaskRobotStatus.h"
#include "src/main.h"
#include "src/map_main.h"

using namespace MapTcp;
//bool TcpTaskRobotStatus::isRunOnRobotStatus = false;

TcpTaskRobotStatus::TcpTaskRobotStatus(QObject *parent):
    TcpTaskBase(parent)
{
    isThreadRunning = false;
    //connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
}

TcpTaskRobotStatus::~TcpTaskRobotStatus()
{
    printf("close.\n");
}

void TcpTaskRobotStatus::run()
{
    map_main *main_ptr = Win::GetMainWin();

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket()) {
        this->isThreadRunning = false;
        return;
    }



    /*char pop_buf[50] = {0};
    char ack_buf[600] = {0};
    Neo_Packet::HEAD *head = (Neo_Packet::HEAD*)(pop_buf+1);
    STATUS_PACKAGE_POP *pop_package = (STATUS_PACKAGE_POP*)(pop_buf+1+sizeof(Neo_Packet::HEAD));
    Neo_Packet::HEAD *ackhead = (Neo_Packet::HEAD*)(ack_buf+1);
    STATUS_PACKAGE_ACK *ack_package = (STATUS_PACKAGE_ACK*)(ack_buf+1+sizeof(Neo_Packet::HEAD));
    head->function_id = Neo_Packet::PacketType::ROBOTSTATUS;
    head->device_id = 0;
    head->size = sizeof(Neo_Packet::HEAD)+sizeof(STATUS_PACKAGE_POP);
    pop_package->isAck = 1;
    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(Neo_Packet::HEAD)+sizeof(STATUS_PACKAGE_POP)+1]=0xAB;*/

   /* p_socket->write((char*)pop_buf,head->size+3);
    if(!p_socket->waitForBytesWritten(2000)){
        printf("send() error\n");
        break;
    }
    int ret = 0;
    bool result = false;
    result = p_socket->waitForReadyRead(2000);

    if(!result || (ret=p_socket->read((char*)ack_buf,(1+sizeof(Neo_Packet::HEAD)+sizeof(STATUS_PACKAGE_ACK)+1+1))) == -1) {
        printf("recv(%d) error\n",ack_package->updateMap);
        break;
    }*/

    this->InitPacket(Neo_Packet::PacketType::ROBOTSTATUS, sizeof(Neo_Packet::STATUS_PACKAGE_POP), sizeof(Neo_Packet::STATUS_PACKAGE_ACK));

    if(this->p_socket->SendSockPackage(this->packet_send, sizeof(Neo_Packet::STATUS_PACKAGE_ACK), this->packet_recv) == this->PacketSend_Head->function_id)
    {
        //printf("recv(%d) success\n",ack_package->updateMap);
        memcpy(&main_ptr->m_robot_status,this->PacketRecv_Body,sizeof(Neo_Packet::STATUS_PACKAGE_ACK));
        main_ptr->m_MapViewCtl.updateRobotPose(main_ptr->m_robot_status);
    }
    else
    {
        //printf("TcpTaskRobotStatus fail.\n");
    }

    this->p_socket->closeSocket();
    this->isThreadRunning = false;
}
