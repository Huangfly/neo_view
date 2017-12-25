#ifndef MAP_MAIN_H
#define MAP_MAIN_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
//#include <Sock>
#include <QTimer>
#include "map_socket_type.h"
#include "socketthread.h"
//#include "map_menu.h"


namespace Ui {
class map_main;
}

class map_main : public QWidget
{
    Q_OBJECT

public slots:
    void OnActionRobot();
    void OnRobotTimer();
    void OnResetPose();
    void OnLockRot();
    void OnSendGoal();

signals:
    //void SaveMap();
public:
    explicit map_main(QWidget *parent = 0);
    ~map_main();
    void OnDownloadMap();
    bool isPatrol();

    QString sysIP;
    QString sysPort;
    QTimer *m_robot_timer;
    STATUS_PACKAGE_ACK m_robot_status;
    QWidget *m_map_view_ctl;
    QWidget *m_map_menu_ctl;
protected:
private:
    Ui::map_main *ui;
    socketThread *m_socket;
    QTcpSocket *socket;

    bool isActionRobot;
};

#endif // MAP_MAIN_H
