#ifndef TCPTASKACTIONNODE_H
#define TCPTASKACTIONNODE_H

#include "TcpTaskBase.h"

namespace MapTcp {
    class TcpTaskActionNode : public TcpTaskBase
    {
        Q_OBJECT
    public:
        TcpTaskActionNode(QString ip,QString port,QObject *parent = 0);
        ~TcpTaskActionNode();
        void startThread(QString str,bool enable){

            if(!isRunning()){
                memset(&pop_body,0,sizeof(pop_body.node_name));
                memcpy(pop_body.node_name,str.toStdString().c_str(),str.size());
                if(enable) pop_body.enable = 1;
                else pop_body.enable = 0;
                this->start();
            }

        }
        bool isRunning(void){ return isThreadRunning; }
    protected:
        void run();
    private:
        bool isThreadRunning;
        NODECTL_PACKAGE_POP pop_body;
    };
}
#endif // TCPTASKACTIONNODE_H
