#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include "src/map_socket_type.h"

namespace MapTcp {
    class TcpSocket : public QTcpSocket
    {
        Q_OBJECT
    public:
        TcpSocket(QObject *parent = 0);
        bool closeSocket();
        bool connectSocket(QThread *thread);
        int SendSockPackage(char *buf, int ack_len, char *ack);
        void setIpPort(QString ip,QString port){ sysIP = ip; sysPort = port; }
    public slots:
        void OnSocketConnected();
        void OnSocketDisconnected();
        void OnSocketError(QAbstractSocket::SocketError error);
    private:
        QString sysIP;
        QString sysPort;
        bool m_bServerConnected;

    };
}

#endif // TCPSOCKET_H
