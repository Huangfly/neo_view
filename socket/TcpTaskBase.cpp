#include "TcpTaskBase.h"

using namespace MapTcp;

TcpTaskBase::TcpTaskBase(QObject *parent):
    QThread(parent)
{

    this->p_socket = new TcpSocket();
    this->p_socket->moveToThread(this);

    this->packet_send = NULL;
    this->packet_recv = NULL;
    PacketSend_Head = NULL;
    PacketSend_Body = NULL;
    PacketRecv_Head = NULL;
    PacketRecv_Body = NULL;
    //this->p_socket->setSocketDescriptor(tcpDes);

}

TcpTaskBase::~TcpTaskBase()
{
    if(this->packet_send == NULL){
        delete [] this->packet_send;
    }

    if(this->packet_recv == NULL){
        delete [] this->packet_recv;
    }
}

void TcpTaskBase::InitPacket(unsigned char mode, unsigned int sendBodySize, unsigned int recvBodySize,unsigned int drive_id)
{

    this->PacketSendSize = sendBodySize+sizeof(P_HEAD)+3;
    if(this->packet_send == NULL){
        this->packet_send = new char[PacketSendSize+5];
    }

    this->PacketRecvSize = recvBodySize+sizeof(P_HEAD)+3;
    if(this->packet_recv == NULL){
        this->packet_recv = new char[PacketRecvSize+5];
    }

    this->PacketRecv_Head = (P_HEAD*)(this->packet_recv + 1);
    this->PacketRecv_Body = this->packet_recv + 1 + sizeof(P_HEAD);
    this->PacketSend_Head = (P_HEAD*)(this->packet_send + 1);
    this->PacketSend_Body = this->packet_send + 1 + sizeof(P_HEAD);

    this->packet_send[0] = 0xAA;
    this->PacketSend_Head->funcId = mode;
    this->PacketSend_Head->msg_code = drive_id;
    this->PacketSend_Head->size = sendBodySize+sizeof(P_HEAD);
    this->packet_send[sendBodySize+sizeof(P_HEAD)+2] = 0xAB;

}

