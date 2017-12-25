#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QObject>
#include <QThread>
#include "map_socket_type.h"


class socketThread : public QObject
{
    Q_OBJECT
public:
    explicit socketThread();
    void OnDownloadMap(void *data);
    static void* DownloadMap(void *data);
    void OnRobotStatus(void *data);
    static void* RobotStatus(void *data);
    void OnSendGoal(void *data);
    static void *SendGoal(void *data);

    static bool isRunOnRobotStatus;
    static bool isRunOnDownloadMap;
    static bool isRunOnSendGoal;

signals:

public slots:
};

#endif // SOCKETTHREAD_H
