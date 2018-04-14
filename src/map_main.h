#ifndef MAP_MAIN_H
#define MAP_MAIN_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
//#include <Sock>
#include <QTimer>
#include <string>
#include "map_socket_type.h"
#include "ManagerSocket.h"
#include "Map_Control.h"
#include "Map_Model.h"

using namespace MapWin;

namespace Ui {
class map_main;
}

class map_main : public QWidget
{
    Q_OBJECT

public slots:
    void OnLockRobot();
    void OnRobotTimer();
    void OnResetPose();
    void OnLockRot();
    void OnSendGoal();
    void OnCancelMove();

signals:
    //void SaveMap();
public:
    explicit map_main(QWidget *parent = 0);
    ~map_main();
    void OnActivateNode(QString str,bool enable);
    bool isPatrol();
    void actionRobot();
    void lockRobot();

    QString sysIP;
    QString sysPort;
    QTimer *m_robot_timer;
    unsigned int timerCount;
    STATUS_PACKAGE_ACK m_robot_status;
    //QWidget *m_map_view_ctl;
    //QWidget *m_map_menu_ctl;
    ManagerSocket *m_socketMag;

    MapWin::Map_Control m_MapViewCtl;
    MapWin::Map_Model m_MapViewModle;
protected:
private:
    Ui::map_main *ui;


    bool isActionRobot;
};

#endif // MAP_MAIN_H
