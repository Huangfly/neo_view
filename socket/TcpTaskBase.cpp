#include "TcpTaskBase.h"

using namespace MapTcp;

TcpTaskBase::TcpTaskBase(QString ip,QString port,QObject *parent):
    QThread(parent)
{

    this->p_socket = new TcpSocket(ip,port);
    this->p_socket->moveToThread(this);
    //this->p_socket->setSocketDescriptor(tcpDes);
}

TcpTaskBase::~TcpTaskBase()
{

}

