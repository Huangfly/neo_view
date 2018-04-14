#ifndef TCPTASKCMDVEL_H
#define TCPTASKCMDVEL_H

#include "TcpTaskBase.h"

using namespace MapTcp;

namespace MapTcp {
    class TcpTaskCmdVel : public TcpTaskBase
    {
        Q_OBJECT
    public:
        TcpTaskCmdVel(QString ip,QString port,QObject *parent = 0);
        ~TcpTaskCmdVel();
        void startThread(float x, float y, float z){

            if(!isRunning()){
                pop_body.x = x;
                pop_body.y = y;
                pop_body.z = z;
                this->start();
            }
        }
        bool isRunning(void){ return isThreadRunning; }
    protected:
        void run();
    private:
        bool isThreadRunning;
        CMDVEL_PACKAGE_POP pop_body;
    };
}

#endif // TCPTASKCMDVEL_H
