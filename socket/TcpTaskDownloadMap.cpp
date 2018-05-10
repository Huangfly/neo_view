#include "TcpTaskDownloadMap.h"
#include "src/main.h"
#include "src/map_main.h"

using namespace MapTcp;
using namespace Win;

TcpTaskDownloadMap::TcpTaskDownloadMap(QObject *parent):
    TcpTaskBase(parent)
{
    isThreadRunning = false;
    map_Data = NULL;
    map_Data_len = 0;
    MapDatas.clear();
}

TcpTaskDownloadMap::~TcpTaskDownloadMap()
{

}

void TcpTaskDownloadMap::run()
{
    map_main *map_main_ptr = Win::GetMainWin();

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket()) {
        this->isThreadRunning = false;
        return;
    }

    this->InitPacket(Neo_Packet::PacketType::MAPDATAS, sizeof(Neo_Packet::MAP_PACKAGE_POP), sizeof(Neo_Packet::MAP_PACKAGE_ACK));
    Neo_Packet::MAP_PACKAGE_POP *map_pop = (Neo_Packet::MAP_PACKAGE_POP*)this->PacketSend_Body;
    Neo_Packet::MAP_PACKAGE_ACK *map_ack = (Neo_Packet::MAP_PACKAGE_ACK*)this->PacketRecv_Body;

    map_pop->package_num = map_pop->package_sum = 0;

    do{

        if(this->p_socket->SendSockPackage(this->packet_send, sizeof(Neo_Packet::MAP_PACKAGE_ACK), this->packet_recv) == Neo_Packet::PacketType::MAPDATAS )
        {

            //printf("ack sum:%d num:%d\n",map_ack->package_sum,map_ack->package_num);
            map_pop->package_sum = map_ack->package_sum;
            map_pop->package_num = map_ack->package_num;
            //num = 0;
            if(map_pop->package_num == 1){
                MapDatas.clear();
            }
            for(unsigned int i = 0; i<512; i++){
                MapDatas.push_back(map_ack->data[i]);
            }

            if (map_pop->package_sum <= map_pop->package_num)
            {
                break;
            }
        }
        else
        {
            printf("ack fail.\n");
            break;
        }
    }while(map_pop->package_sum != 0 && map_pop->package_sum >= map_ack->package_num);

    //printf("map ack w:%d h:%d size:%d recv:%d\n",map_ack->width,map_ack->hight,map_ack->size,(int)(DataPos-map_Data));
    //printf("ack x:%f y:%f \n",map_ack->x,map_ack->y);

    this->p_socket->closeSocket();
    this->isThreadRunning = false;

    if(map_ack->package_sum != 0 && map_ack->package_sum == map_ack->package_num)
    {
        map_main_ptr->m_MapViewCtl.updateMap(MapDatas,map_ack->width,map_ack->hight,map_ack->resolution,*(ST_POSE*)(&map_ack->x));
        map_main_ptr->m_robot_status.updateMap = 0;
    }
}

