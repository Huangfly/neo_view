#ifndef TCPTASKROBOTSTATUS_H
#define TCPTASKROBOTSTATUS_H

#include "TcpTaskBase.h"


namespace MapTcp {
    class TcpTaskRobotStatus : public TcpTaskBase
    {
        Q_OBJECT
    public:
        TcpTaskRobotStatus(QString ip,QString port,QObject *parent = 0);
        ~TcpTaskRobotStatus();
        void startThread(){ if(!isRunning()) this->start(); }
        bool isRunning(void){ return isThreadRunning; }
    protected:
        void run();
    private:
        bool isThreadRunning;
    };
}
#endif // TCPTASKROBOTSTATUS_H
