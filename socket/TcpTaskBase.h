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
        explicit TcpTaskBase(QString ip,QString port,QObject *parent = 0);
        virtual ~TcpTaskBase();
        TcpSocket *p_socket;
        int tcpDes;
        QString sysIP;
        QString sysPort;
    };
}
#endif // TCPTASKBASE_H
