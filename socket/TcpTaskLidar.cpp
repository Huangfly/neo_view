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
    //char *DataPos = map_Data;
    map_main *ctl = Win::GetMainWin();

    this->isThreadRunning = true;
    if(!this->p_socket->connectSocket(this)) {
        this->isThreadRunning = false;
        return;
    }

    unsigned char str[50] = {0};
    unsigned char ack[2200] = {0};
    P_HEAD *head = (P_HEAD*)(str+1);
    LIDAR_PACKAGE_POP *lidar_pop = (LIDAR_PACKAGE_POP*)(str+1+sizeof(P_HEAD));
    P_HEAD *ackhead = (P_HEAD*)(ack+1);
    LIDAR_PACKAGE_ACK *lidar_ack = (LIDAR_PACKAGE_ACK*)(ack+1+sizeof(P_HEAD));
    head->funcId = PACK_LIDAR;
    head->msg_code = 0;
    head->size = sizeof(P_HEAD)+sizeof(LIDAR_PACKAGE_POP);
    //map_pop->package_num = 0;
    //map_pop->package_sum = 0;
    str[0] = 0xAA;
    str[ 1 + head->size + 1 ]=0xAB;

    do{

        p_socket->write((char*)str,head->size+9);
        if(!p_socket->waitForBytesWritten()){
            printf("lidar send() error\n");
            break;
        }

        //printf("wait ack...\n");
        int ret = 0;
        bool result = false;
        result = p_socket->waitForReadyRead(5000);

        if(!result || (ret=p_socket->read((char*)ack,sizeof(LIDAR_PACKAGE_ACK)+19)) == -1) {
            printf("lidar recv() error\n");
            break;
        }
        if(ackhead->funcId == PACK_LIDAR )
        {

            //printf("lidar ack sum:%d num:%d\n",lidar_ack->package_sum,lidar_ack->package_num);
            lidar_pop->serial_num = lidar_ack->serial_num;
            lidar_pop->package_sum = lidar_ack->package_sum;
            lidar_pop->package_num = lidar_ack->package_num;
            //num = 0;
            if(lidar_ack->package_num == 1){
                lidar_Data.clear();
            }
            for(unsigned int i = 0;i<lidar_ack->this_rangs_size;i++){
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
        ctl->m_MapViewCtl.updateLidarData(lidar_Data,lidar_ack->angle_min,lidar_ack->angle_min,lidar_ack->angle_increment,lidar_ack->pose);
    }

    lidar_Data.clear();
}
