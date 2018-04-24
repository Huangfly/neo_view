#include "TcpTaskLoadMap.h"
#include "src/Map_Model.h"
#include "src/main.h"
#include "src/map_main.h"

using namespace MapWin;
using namespace MapTcp;

MapTcp::TcpTaskLoadMap::TcpTaskLoadMap(QObject *parent):
    TcpTaskBase(parent)
{
}

MapTcp::TcpTaskLoadMap::~TcpTaskLoadMap()
{
}

void MapTcp::TcpTaskLoadMap::run()
{
    //char *DataPos = map_Data;
    map_main *map_main_ptr = Win::GetMainWin();

    printf("LoadMap Tcp\n");
    if(!this->p_socket->connectSocket(this)) {
        return;
    }

    this->InitPacket(PACK_LOADMAP,sizeof(LOADMAP_PACKAGE_POP),sizeof(LOADMAP_PACKAGE_ACK));

    LOADMAP_PACKAGE_POP *send_body = (LOADMAP_PACKAGE_POP *)this->PacketSend_Body;
    LOADMAP_PACKAGE_ACK *recv_body = (LOADMAP_PACKAGE_ACK *)this->PacketRecv_Body;

    MapWin::MapVector MapData;
    map_main_ptr->m_MapViewModle.GetMap(MapData);

    send_body->height = MapData.height;
    send_body->width = MapData.width;
    send_body->resolution = MapData.resolution;
    MapData.pose.ToPOSE(&send_body->map_pose);
    send_body->package_sum = ( MapData.data.size()%LOADMAP_PACKAGE_SIZE == 0 ? MapData.data.size()/LOADMAP_PACKAGE_SIZE : MapData.data.size()/LOADMAP_PACKAGE_SIZE+1 );
    send_body->package_num = 1;

    printf("size:%d\n",send_body->height*send_body->width);
    do{
        send_body->data_size = ( (MapData.data.size()%LOADMAP_PACKAGE_SIZE == 0 || send_body->package_num != send_body->package_sum) ?
                                     LOADMAP_PACKAGE_SIZE : MapData.data.size()%LOADMAP_PACKAGE_SIZE );
        //printf("bag size:%d\n",send_body->data_size);
        MapData.copyToBuf((char*)send_body->data
                          , (send_body->package_num-1) * LOADMAP_PACKAGE_SIZE
                          , ( (send_body->package_num-1) * LOADMAP_PACKAGE_SIZE + send_body->data_size ));

        p_socket->write(this->packet_send,this->PacketSendSize);
        if(!p_socket->waitForBytesWritten()){
            printf("send() error\n");
            break;
        }

        int ret = 0;
        bool result = false;
        result = p_socket->waitForReadyRead(5000);

        if(!result || ( ret=p_socket->read(this->packet_recv,this->PacketRecvSize) ) == -1) {
            printf("recv() error\n");
            break;
        }

        if( this->PacketRecv_Head->funcId == PACK_LOADMAP )
        {
            printf("ack sum:%d num:%d\n",recv_body->package_sum,recv_body->package_num);
            //send_body->package_sum = recv_body->package_sum;
            send_body->package_num = recv_body->package_num;
            //num = 0;
            if (send_body->package_sum < send_body->package_num
                    || recv_body->package_sum == 0)
            {
                break;
            }
        }
        else
        {
            printf("ack fail.\n");
            break;
        }
    }while(send_body->package_sum != 0 && recv_body->package_sum >= recv_body->package_num);

    this->p_socket->closeSocket();
}

