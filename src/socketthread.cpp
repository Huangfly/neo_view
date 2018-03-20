#include "socketthread.h"
#include "map_main.h"
#include "map_view.h"
#include "socket/CTcpSocket.h"

QTcpSocket *pt_socket = NULL;
QString sysIP = "192.168.1.128";
QString sysPort = "8888";
bool socketThread::isRunOnRobotStatus = false;
bool socketThread::isRunOnDownloadMap = false;
bool socketThread::isRunOnSendGoal = false;
bool socketThread::isRunOnCancelGoal = false;
bool socketThread::isRunOnActivateNode = false;

char *map_Data = NULL;
unsigned long map_Data_len = 0;
pthread_mutex_t mutex_socket;
pthread_mutex_t mutex_rosbotstatus;
pthread_mutex_t mutex_nodeaction;

int SendSockPackage(QTcpSocket *p_socket,char *buf, int ack_len, char *ack)
{
    if(buf == NULL || ack == NULL || ack_len<=0)return -1;
    P_HEAD *head = (P_HEAD*)(buf+1);
    P_HEAD *ackhead = (P_HEAD*)(ack+1);

    p_socket->write((char*)buf,head->size+3);
    if(!p_socket->waitForBytesWritten()){
        printf("send() error\n");
        return -1;
    }

    //printf("wait ack...\n");
    int ret = 0;
    bool result = false;
    result = p_socket->waitForReadyRead(5000);

    if(!result || (ret=p_socket->read((char*)ack,(1+sizeof(P_HEAD)+ack_len+1+1))) == -1) {
        printf("recv() error\n");
        return -1;
    }
    return ackhead->funcId;
}

bool connectsocket(QTcpSocket *p_socket)
{
    bool connected = false;
    QAbstractSocket::SocketState states = p_socket->state();
    if(states == QAbstractSocket::UnconnectedState)
    {
MAINWINDOWS_CONNECTSOCKET:

        p_socket->connectToHost(sysIP,sysPort.toInt(),QTcpSocket::ReadWrite);
        connected = p_socket->waitForConnected(2000);
        if(connected == false)
        {
            //QMessageBox::information(NULL, "Connect fail.", "Enter ok to reset IP.          ");
            return false;
        }
        return true;
    }
    else if(states == QAbstractSocket::ConnectedState)
    {
        return true;
    }
    else if(states == QAbstractSocket::ConnectingState)
    {
        connected = p_socket->waitForConnected();
        if(connected == false)return false;
        return true;
    }
    else if(states == QAbstractSocket::ClosingState)
    {
        connected = p_socket->waitForDisconnected(3000);
        if(connected == false)return false;
        goto MAINWINDOWS_CONNECTSOCKET;
    }
    else
    {
        return false;
    }
}

bool closesocket(QTcpSocket *p_socket)
{
    bool connected = false;
    QAbstractSocket::SocketState states = p_socket->state();
    if(states == QAbstractSocket::ConnectedState)
    {
MAINWINDOWS_CLOSESOCKET:
        p_socket->disconnectFromHost();
        //int timercount =
        while(1)
        {
            connected = false;
            if(p_socket->state() == QAbstractSocket::UnconnectedState)
            {
                connected = true;
                break;
            }
        }
        if(connected == false)return false;
        return true;
    }
    else if(states == QAbstractSocket::UnconnectedState)
    {
        return true;
    }
    else if(states == QAbstractSocket::ClosingState)
    {
        connected = p_socket->waitForDisconnected(3000);
        if(connected == false)return false;
        return true;
    }
    else if(states == QAbstractSocket::ConnectingState)
    {
        connected = p_socket->waitForConnected(3000);
        if(connected == false)return false;
        goto MAINWINDOWS_CLOSESOCKET;
    }
    else
    {
        return false;
    }
}

socketThread::socketThread()
{
    //if(p_socket == NULL)
    //    p_socket = new QTcpSocket(this);// the end of process was delete
    isRunOnRobotStatus = false;
    //this->map_Data = NULL;
    //map_Data_len = 0;
    pthread_mutex_init(&mutex_rosbotstatus,NULL);
    pthread_mutex_init(&mutex_nodeaction,NULL);
    pthread_mutex_init(&mutex_socket,NULL);
}

void socketThread::OnDownloadMap(void *data)
{
    pthread_t id;
    bool ret;
    ret = pthread_create(&id,NULL,DownloadMap,data);
    if(!ret)
    {
        printf("succeed!\n");

    }
    else
    {
        printf("Fail to Create Thread.\n");
    }
}

void socketThread::OnRobotStatus(void *data)
{
    pthread_t id;
    bool ret;

    ret = pthread_create(&id,NULL,RobotStatus,data);
    if(!ret)
    {
        //printf("succeed!\n");

    }
    else
    {
        printf("Fail to Create Thread.\n");
    }
}

void socketThread::OnSendGoal(void *data)
{
    pthread_t id;
    bool ret;

    ret = pthread_create(&id,NULL,SendGoal,data);
    if(!ret)
    {
        //printf("succeed!\n");

    }
    else
    {
        printf("Fail to Create Thread.\n");
    }
}

void *socketThread::SendGoal(void *data)
{
pthread_detach(pthread_self());
    char pop_buf[80];
    char ack_buf[80];
    map_main *p_map_main = (map_main *)data;
    map_view *p_map_view = (map_view *)(p_map_main->m_map_view_ctl);
    QTcpSocket socket;
    QTcpSocket *p_socket = &socket;
    isRunOnSendGoal = true;
    if(!connectsocket(p_socket)){
        closesocket(p_socket);
        isRunOnSendGoal = false;
        return NULL;
    }
    P_HEAD *head = (P_HEAD*)(pop_buf+1);
    GOAL_PACKAGE_POP *pop_package = (GOAL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
    //P_HEAD *ackhead = (P_HEAD*)(ack+1);
    //GOAL_PACKAGE_ACK *ack_package = (GOAL_PACKAGE_ACK*)(ack_buf+1+sizeof(P_HEAD));
    head->funcId = PACK_GOAL;
    head->size = sizeof(P_HEAD)+sizeof(GOAL_PACKAGE_POP);

    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(P_HEAD)+sizeof(GOAL_PACKAGE_POP)+1]=0xAB;
    if(p_map_view->getGoalsFromList((ST_POSE*)pop_package,0) > 0)
    {
        //printf("%f %f %f %f\n",pop_package->Quaternion[0],pop_package->Quaternion[1],pop_package->Quaternion[2],pop_package->Quaternion[3]);
        if(SendSockPackage(p_socket,pop_buf,sizeof(GOAL_PACKAGE_ACK),ack_buf) == head->funcId)
        {
            printf("Goal send success...\n");
        }
    }
    closesocket(p_socket);
    isRunOnSendGoal = false;
    return NULL;
}

void socketThread::OnCancelGoal(void *data)
{
    pthread_t id;
    bool ret;

    ret = pthread_create(&id,NULL,CancelGoal,data);
    if(!ret)
    {
        //printf("succeed!\n");

    }
    else
    {
        printf("Fail to Create Thread.\n");
    }
}

void *socketThread::CancelGoal(void *data)
{
    pthread_detach(pthread_self());
    char pop_buf[80];
    char ack_buf[80];

    //map_main *p_map_main = (map_main *)data;
    //map_view *p_map_view = (map_view *)(p_map_main->m_map_view_ctl);
    QTcpSocket socket;
    QTcpSocket *p_socket = &socket;
    isRunOnCancelGoal = true;
    if(!connectsocket(p_socket)){
        closesocket(p_socket);
        isRunOnCancelGoal = false;
        return NULL;
    }
    P_HEAD *head = (P_HEAD*)(pop_buf+1);
    CANCELGOAL_PACKAGE_POP *pop_package = (CANCELGOAL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
    head->funcId = PACK_CANCELGOAL;
    head->size = sizeof(P_HEAD)+sizeof(CANCELGOAL_PACKAGE_POP);
    pop_package->isAck = 1;

    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(P_HEAD)+sizeof(CANCELGOAL_PACKAGE_POP)+1]=0xAB;
    if(SendSockPackage(p_socket,pop_buf,sizeof(CANCELGOAL_PACKAGE_ACK),ack_buf) == head->funcId)
    {
        printf("Goal cancel success...\n");
    }
    closesocket(p_socket);
    isRunOnCancelGoal = false;
    return NULL;
}

void socketThread::OnActivateNode(std::string str,char enable)
{
    pthread_t id;
    bool ret;
    NODECTL_PACKAGE_POP in_Data;
    memset(in_Data.node_name,0,sizeof(in_Data.node_name));
    in_Data.enable = enable;
    if(str.size()>19) return;
    memcpy(in_Data.node_name,str.c_str(),str.size());
    printf("%s\n",in_Data.node_name);
    pthread_mutex_lock(&mutex_nodeaction);
    ret = pthread_create(&id,NULL,ActivateNode,&in_Data);
    if(!ret)
    {
        //printf("succeed!\n");

    }
    else
    {
        printf("Fail to Create Thread.\n");
    }
    pthread_mutex_lock(&mutex_nodeaction);
    pthread_mutex_unlock(&mutex_nodeaction);
}

void *socketThread::ActivateNode(void *data)
{
    NODECTL_PACKAGE_POP in_Data;

    memcpy(&in_Data,data,sizeof(NODECTL_PACKAGE_POP));
    pthread_mutex_unlock(&mutex_nodeaction);

    pthread_detach(pthread_self());
    char pop_buf[80];
    char ack_buf[80];
    //map_main *p_map_main = (map_main *)data;
    //map_view *p_map_view = (map_view *)(p_map_main->m_map_view_ctl);
    QTcpSocket socket;
    QTcpSocket *p_socket = &socket;

    isRunOnActivateNode = true;
    if(!connectsocket(p_socket)){
        closesocket(p_socket);
        isRunOnActivateNode = false;
        return NULL;
    }
    P_HEAD *head = (P_HEAD*)(pop_buf+1);
    NODECTL_PACKAGE_POP *pop_package = (NODECTL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
    head->funcId = PACK_NODECTL;
    head->size = sizeof(P_HEAD)+sizeof(NODECTL_PACKAGE_POP);
    memcpy(pop_package,&in_Data,sizeof(NODECTL_PACKAGE_POP));

    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(P_HEAD)+sizeof(NODECTL_PACKAGE_POP)+1]=0xAB;
    if(SendSockPackage(p_socket,pop_buf,sizeof(NODECTL_PACKAGE_ACK),ack_buf) == head->funcId)
    {
        printf("Goal cancel success...\n");
    }
    closesocket(p_socket);
    isRunOnActivateNode = false;
    return NULL;
}

void socketThread::OnCmdVel(float x, float y, float z)
{
    pthread_t id;
    bool ret;
    CMDVEL_PACKAGE_POP *p = new CMDVEL_PACKAGE_POP();
    p->x = x;
    p->y = y;
    p->z = z;
    ret = pthread_create(&id,NULL,CmdVel,p);
    if(!ret)
    {
        //printf("succeed!\n");

    }
    else
    {
        printf("Fail to Create Thread.\n");
    }
}

void *socketThread::CmdVel(void *data)
{
    CMDVEL_PACKAGE_POP in_Data;

    memcpy(&in_Data,data,sizeof(CMDVEL_PACKAGE_POP));
    delete data;
    //pthread_mutex_unlock(&mutex_nodeaction);

    pthread_detach(pthread_self());
    char pop_buf[80];
    char ack_buf[80];
    //map_main *p_map_main = (map_main *)data;
    //map_view *p_map_view = (map_view *)(p_map_main->m_map_view_ctl);
    QTcpSocket socket;
    QTcpSocket *p_socket = &socket;

    isRunOnActivateNode = true;
    if(!connectsocket(p_socket)){
        closesocket(p_socket);
        isRunOnActivateNode = false;
        return NULL;
    }
    P_HEAD *head = (P_HEAD*)(pop_buf+1);
    CMDVEL_PACKAGE_POP *pop_package = (CMDVEL_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
    head->funcId = PACK_CMDVEL;
    head->size = sizeof(P_HEAD)+sizeof(CMDVEL_PACKAGE_POP);
    memcpy(pop_package,&in_Data,sizeof(CMDVEL_PACKAGE_POP));

    pop_buf[0] = 0xAA;
    pop_buf[1+sizeof(P_HEAD)+sizeof(CMDVEL_PACKAGE_POP)+1]=0xAB;
    if(SendSockPackage(p_socket,pop_buf,sizeof(CMDVEL_PACKAGE_ACK),ack_buf) == head->funcId)
    {
        printf("Goal cancel success...\n");
    }
    closesocket(p_socket);
    isRunOnActivateNode = false;
    return NULL;
}

void* socketThread::RobotStatus(void *data)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&mutex_rosbotstatus);
    map_main *p_map_main = (map_main *)data;
    map_view *p_map_view = (map_view *)(p_map_main->m_map_view_ctl);
    do{
        QTcpSocket socket;
        QTcpSocket *p_socket = &socket;
        isRunOnRobotStatus = true;
        if(!connectsocket(p_socket)){
            closesocket(p_socket);
            break;
        }
        /*p_socket->connectToHost(sysIP,sysPort.toInt(),QTcpSocket::ReadWrite);
        if(!p_socket->waitForConnected(2000)){
            isRunOnRobotStatus = false;
            closesocket(p_socket);
            return NULL;
        }*/

        char pop_buf[50] = {0};
        char ack_buf[600] = {0};
        P_HEAD *head = (P_HEAD*)(pop_buf+1);
        STATUS_PACKAGE_POP *pop_package = (STATUS_PACKAGE_POP*)(pop_buf+1+sizeof(P_HEAD));
        P_HEAD *ackhead = (P_HEAD*)(ack_buf+1);
        STATUS_PACKAGE_ACK *ack_package = (STATUS_PACKAGE_ACK*)(ack_buf+1+sizeof(P_HEAD));
        head->funcId = PACK_HEARD;
        head->size = sizeof(P_HEAD)+sizeof(STATUS_PACKAGE_POP);
        pop_package->isAck = 1;
        pop_buf[0] = 0xAA;
        pop_buf[1+sizeof(P_HEAD)+sizeof(STATUS_PACKAGE_POP)+1]=0xAB;

       /* int ret = 0;
        if((ret = SendSockPackage(p_socket,pop_buf,sizeof(STATUS_PACKAGE_ACK),ack_buf)) == head->funcId)
        {
            printf("Goal cancel success...\n");
        }*/

        //sleep(1);
        //printf("send : \n id:%d  size:%d\n",head->funcId,head->size);
        //printf("sum:%d num:%d\n",map_pop->package_sum,map_pop->package_num);

        p_socket->write((char*)pop_buf,head->size+3);
        if(!p_socket->waitForBytesWritten()){
            printf("send() error\n");
            isRunOnRobotStatus = false;
            closesocket(p_socket);
            break;
        }
        //sleep(1);
        //printf("wait ack...\n");
        int ret = 0;
        bool result = false;
        result = p_socket->waitForReadyRead(2000);

        if(!result || (ret=p_socket->read((char*)ack_buf,(1+sizeof(P_HEAD)+sizeof(STATUS_PACKAGE_ACK)+1+1))) == -1) {
            printf("recv(%d) error\n",ack_package->updateMap);
            isRunOnRobotStatus = false;
            closesocket(p_socket);
            break;
        }
        closesocket(p_socket);
        if(ackhead->funcId == PACK_HEARD && ret>=0)
        {
            printf("recv(%d) success\n",ack_package->updateMap);
            memcpy(&p_map_main->m_robot_status,ack_package,sizeof(STATUS_PACKAGE_ACK));
            p_map_view->updateRobotPose(p_map_main->m_robot_status);
            p_map_view->emitUpdateUI();
        }
        else
        {
            printf("ack fail.\n");
            //return NULL;
        }
    }while(0);
    isRunOnRobotStatus = false;
    pthread_mutex_unlock(&mutex_rosbotstatus);
    return NULL;
}




void* socketThread::DownloadMap(void *data)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&mutex_socket);
    //char *p = NULL;
    char *pp = map_Data;
    map_main *p_map_main = (map_main *)data;
    map_view *p_map_view = (map_view *)(p_map_main->m_map_view_ctl);
    QTcpSocket socket;
    QTcpSocket *p_socket = &socket;
    if(isRunOnDownloadMap)return NULL;
    isRunOnDownloadMap = true;
    if(!connectsocket(p_socket)){
        closesocket(p_socket);
        isRunOnDownloadMap = false;
        return NULL;
    }
    unsigned char str[50] = {0};
    unsigned char ack[800] = {0};
    P_HEAD *head = (P_HEAD*)(str+1);
    MAP_PACKAGE_POP *map_pop = (MAP_PACKAGE_POP*)(str+1+16);
    P_HEAD *ackhead = (P_HEAD*)(ack+1);
    MAP_PACKAGE_ACK *map_ack = (MAP_PACKAGE_ACK*)(ack+1+16);
    head->funcId = PACK_MAP;
    head->size = 16+8;
    map_pop->package_num = 0;
    map_pop->package_sum = 0;
    str[0] = 0xAA;
    str[1+16+8+1]=0xAB;

    do{
        //sleep(1);
        //printf("send : \n id:%d  size:%d\n",head->funcId,head->size);
        //printf("sum:%d num:%d\n",map_pop->package_sum,map_pop->package_num);

        p_socket->write((char*)str,head->size+9);
        if(!p_socket->waitForBytesWritten()){
            printf("send() error\n");
            break;
        }

        //printf("wait ack...\n");
        int ret = 0;
        bool result = false;
        result = p_socket->waitForReadyRead(5000);

        if(!result || (ret=p_socket->read((char*)ack,sizeof(MAP_PACKAGE_ACK)+19)) == -1) {
            printf("recv() error\n");
            break;
        }
        if(ackhead->funcId == PACK_MAP )
        {

            printf("ack sum:%d num:%d\n",map_ack->package_sum,map_ack->package_num);
            map_pop->package_sum = map_ack->package_sum;
            map_pop->package_num = map_ack->package_num;
            //num = 0;

            if( map_Data_len <  (map_ack->package_sum*512) ){
                if(map_Data != NULL) delete [] map_Data;
                pp = map_Data = new char[map_ack->package_sum*512+10];
                map_Data_len = map_ack->package_sum*512;
            }
            memcpy(pp,map_ack->data,512);
            pp += 512;
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
        //closesocket(p_socket);
    }while(map_pop->package_sum != 0 && map_pop->package_sum >= map_ack->package_num);
    printf("ack w:%d h:%d size:%d recv:%d\n",map_ack->width,map_ack->hight,map_ack->size,(int)(pp-map_Data));
    printf("ack x:%f y:%f \n",map_ack->x,map_ack->y);

    closesocket(p_socket);
    pthread_mutex_unlock(&mutex_socket);
    if(map_ack->package_sum != 0 && map_ack->package_sum == map_ack->package_num)
    {

        p_map_view->updateMap(map_Data,map_ack->width,map_ack->hight,map_ack->resolution,*(ST_POSE*)(&map_ack->x));
        p_map_view->emitUpdateUI();
        p_map_main->m_robot_status.updateMap = 0;
        //ui->_map_view->update();
        //QMessageBox::about(this,"Msg","receive success!");
    }
    //if(p != NULL)delete [] p;
    isRunOnDownloadMap = false;
    return NULL;
}

