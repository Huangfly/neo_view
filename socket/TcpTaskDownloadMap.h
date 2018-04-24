#ifndef TCPTASKDOWNLOADMAP_H
#define TCPTASKDOWNLOADMAP_H

#include "TcpTaskBase.h"
#include <vector>

namespace MapTcp {
    class TcpTaskDownloadMap : public TcpTaskBase
    {
        Q_OBJECT
    public:
        TcpTaskDownloadMap(QObject *parent = 0);
        ~TcpTaskDownloadMap();
        void startThread(){ if(!isRunning()) this->start(); }
        bool isRunning(void){ return isThreadRunning; }
    protected:
        void run();
    private:
        bool isThreadRunning;
        char* map_Data;
        unsigned long map_Data_len;
        std::vector<char> MapDatas;
    };
}
#endif // TCPTASKDOWNLOADMAP_H
