#include "TcpTaskLidar.h"
#include "src/main.h"
#include "src/map_main.h"

using namespace Win;
using namespace MapTcp;

MapTcp::TcpTaskLidar::TcpTaskLidar(QObject *parent):
    TcpTaskBase(parent)
{
    isThreadRunning = false;
    lidar_Data.clear();
}

TcpTaskLidar::~TcpTaskLidar()
{
lidar_Data.clear();
}

void TcpTaskLidar::run()
{
    map_main *ctl = Win::GetMainWin();

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket()) {
        this->isThreadRunning = false;
        return;
    }

    this->InitPacket(Neo_Packet::PacketType::LIDARDATAS, sizeof(Neo_Packet::LIDAR_PACKAGE_POP), sizeof(Neo_Packet::LIDAR_PACKAGE_ACK));
    Neo_Packet::LIDAR_PACKAGE_POP *lidar_pop = (Neo_Packet::LIDAR_PACKAGE_POP*)this->PacketSend_Body;
    Neo_Packet::LIDAR_PACKAGE_ACK *lidar_ack = (Neo_Packet::LIDAR_PACKAGE_ACK*)this->PacketRecv_Body;
    lidar_pop->package_num = lidar_pop->package_sum = 0;

    do{
        if(this->p_socket->SendSockPackage(this->packet_send, sizeof(Neo_Packet::LIDAR_PACKAGE_ACK), this->packet_recv) == this->PacketSend_Head->function_id )
        {

            //printf("lidar ack sum:%d num:%d\n",lidar_ack->package_sum,lidar_ack->package_num);
            lidar_pop->serial_num = lidar_ack->serial_num;
            lidar_pop->package_sum = lidar_ack->package_sum;
            lidar_pop->package_num = lidar_ack->package_num;
            //num = 0;
            if(lidar_ack->package_num == 1){
                lidar_Data.clear();
            }
            for(unsigned int i = 0;i<lidar_ack->rangs_size;i++){
                lidar_Data.push_back(lidar_ack->rangs[i]);
            }

            if (lidar_pop->package_sum <= lidar_pop->package_num)
            {
                break;
            }
        }
        else
        {
            printf("lidar ack fail.\n");
            break;
        }
    }while(lidar_pop->package_sum != 0 && lidar_pop->package_sum >= lidar_ack->package_num);

    this->p_socket->closeSocket();
    this->isThreadRunning = false;

    if(lidar_ack->package_sum != 0 && lidar_ack->package_sum == lidar_ack->package_num){
        ctl->m_MapViewCtl.updateLidarData(lidar_Data,lidar_ack->angle_min,lidar_ack->angle_min,lidar_ack->angle_increment,*((ST_POSE*)&lidar_ack->pose));
    }

    lidar_Data.clear();
}
