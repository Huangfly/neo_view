#ifndef TCPTASKLOADMAP_H
#define TCPTASKLOADMAP_H

#include <QObject>
#include "TcpTaskBase.h"
#include <vector>

namespace MapTcp {
    class TcpTaskLoadMap : public TcpTaskBase
    {
    public:
        TcpTaskLoadMap(QObject *parent = 0);
        ~TcpTaskLoadMap();
        void startThread(){ if(!this->isRunning()) this->start(); }
    protected:
        void run();
    private:
    };
}

#endif // TCPTASKLOADMAP_H
