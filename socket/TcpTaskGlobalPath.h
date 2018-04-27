#ifndef TCPTASKGLOBALPATH
#define TCPTASKGLOBALPATH

#include "TcpTaskBase.h"

namespace MapTcp {
    class TcpTaskGlobalPath : public TcpTaskBase
    {
        Q_OBJECT
    public:
        TcpTaskGlobalPath(QObject *parent = 0);
        ~TcpTaskGlobalPath();
        void startThread(){ if(!isRunning()) this->start(); }
    protected:
        void run();
    private:
        //bool isSendGoal;
        std::vector<QVector2D> paths;
    };
}

#endif // TCPTASKGLOBALPATH

