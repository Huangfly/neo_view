#include "TcpSocket.h"

using namespace MapTcp;

static pthread_mutex_t *mutex_socket_ptr = NULL;

TcpSocket::TcpSocket(QObject *parent):
    QTcpSocket(parent)
{
    this->sysIP = "127.0.0.1";
    this->sysPort = "8888";
    m_bServerConnected = false;
    //this->setPeerPort();
    if(mutex_socket_ptr == NULL){
        mutex_socket_ptr = new pthread_mutex_t;
        pthread_mutex_init(mutex_socket_ptr,NULL);
    }
    connect(this, SIGNAL(connected()), this, SLOT(OnSocketConnected()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnSocketDisconnected()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));
}

void TcpSocket::OnSocketConnected()
{
    m_bServerConnected = true;
}

void TcpSocket::OnSocketDisconnected()
{
    m_bServerConnected = false;
}

void TcpSocket::OnSocketError(QAbstractSocket::SocketError error)
{
    printf("%s\n",this->errorString().toStdString().c_str());
}


bool TcpSocket::closeSocket()
{
    pthread_mutex_unlock(mutex_socket_ptr);
    if(m_bServerConnected){
        this->disconnectFromHost();
    }
    return true;
}

bool TcpSocket::connectSocket()
{
    pthread_mutex_lock(mutex_socket_ptr);
    if(!m_bServerConnected){
        this->connectToHost(sysIP,sysPort.toInt(),QTcpSocket::ReadWrite);
    }
    return true;

}

int TcpSocket::SendSockPackage(char *buf, int ack_len, char *ack, unsigned int time_out)
{
    if(buf == NULL || ack == NULL || ack_len<=0)return -1;
    Neo_Packet::HEAD *head = (Neo_Packet::HEAD*)(buf+1);
    Neo_Packet::HEAD *ackhead = (Neo_Packet::HEAD*)(ack+1);

    this->write((char*)buf,head->size+3);
    if(!this->waitForBytesWritten()){
        printf("send() error\n");
        return -1;
    }

    //printf("wait ack...\n");
    int ret = 0;
    bool result = false;
    result = this->waitForReadyRead(time_out);

    if(!result || (ret=this->read((char*)ack,(1+sizeof(Neo_Packet::HEAD)+ack_len+1+1))) == -1) {
        printf("recv() error\n");
        return -1;
    }
    return ackhead->function_id;
}

