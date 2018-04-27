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

    char *DataPos = map_Data;
    map_main *map_main_ptr = Win::GetMainWin();

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket(this)) {
        this->isThreadRunning = false;
        return;
    }

    unsigned char str[50] = {0};
    unsigned char ack[800] = {0};
    P_HEAD *head = (P_HEAD*)(str+1);
    MAP_PACKAGE_POP *map_pop = (MAP_PACKAGE_POP*)(str+1+16);
    P_HEAD *ackhead = (P_HEAD*)(ack+1);
    MAP_PACKAGE_ACK *map_ack = (MAP_PACKAGE_ACK*)(ack+1+16);
    head->funcId = PACK_MAP;
    head->msg_code = 0;
    head->size = 16+8;
    map_pop->package_num = 0;
    map_pop->package_sum = 0;
    str[0] = 0xAA;
    str[1+16+8+1]=0xAB;

    do{
        p_socket->write((char*)str,head->size+9);
        if(!p_socket->waitForBytesWritten()){
            printf("send() error\n");
            break;
        }

        int ret = 0;
        bool result = false;
        result = p_socket->waitForReadyRead(5000);

        if(!result || (ret=p_socket->read((char*)ack,sizeof(MAP_PACKAGE_ACK)+19)) == -1) {
            printf("recv() error\n");
            break;
        }
        if(ackhead->funcId == PACK_MAP )
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

            DataPos += 512;
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

