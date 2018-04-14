#ifndef TCPTASKLIDAR_H
#define TCPTASKLIDAR_H

#include <QObject>
#include "TcpTaskBase.h"
#include <vector>

namespace MapTcp {
    class TcpTaskLidar : public TcpTaskBase
    {
    public:
        TcpTaskLidar(QString ip,QString port,QObject *parent = 0);
        ~TcpTaskLidar();
        void startThread(){ if(!isRunning()) this->start(); }
        bool isRunning(void){ return isThreadRunning; }
    protected:
        void run();
    private:
        bool isThreadRunning;
        std::vector<float> lidar_Data;
    };
}

#endif // TCPTASKLIDAR_H
