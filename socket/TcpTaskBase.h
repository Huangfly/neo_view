#ifndef TCPTASKBASE_H
#define TCPTASKBASE_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "TcpSocket.h"

namespace MapTcp {
    class TcpTaskBase : public QThread
    {
        Q_OBJECT
    public:
        explicit TcpTaskBase(QObject *parent = 0);
        virtual ~TcpTaskBase();
        void InitPacket(unsigned char mode, unsigned int sendBodySize, unsigned int recvBodySize,unsigned int drive_id = 0);
        TcpSocket *p_socket;
        int tcpDes;
        QString sysIP;
        QString sysPort;

        unsigned int PacketSendSize;
        unsigned int PacketRecvSize;
        char *packet_send;
        char *packet_recv;
        P_HEAD *PacketSend_Head;
        void *PacketSend_Body;
        P_HEAD *PacketRecv_Head;
        void *PacketRecv_Body;
    };
}
#endif // TCPTASKBASE_H
