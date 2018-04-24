#ifndef TCPTASKGOAL_H
#define TCPTASKGOAL_H

#include "TcpTaskBase.h"

namespace MapTcp {
    class TcpTaskGoal : public TcpTaskBase
    {
        Q_OBJECT
    public:
        TcpTaskGoal(QObject *parent = 0);
        ~TcpTaskGoal();
        void startSendGoalThread();
        void startCancelGoalThread();
    protected:
        void run();
    private:
        bool isSendGoal;
    };
}
#endif // TCPTASKGOAL_H
