#include "map_main.h"
#include "ui_map_main.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  /* netdb is necessary for struct hostent */
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>

#define PORT 8888  /* server port */





bool map_main::isPatrol()
{
    return ui->_map_menu->isPatrol;
}

void map_main::actionRobot()
{
    this->isActionRobot = true;
    this->m_robot_timer->start(200);
}

void map_main::lockRobot()
{
    this->isActionRobot = false;
    this->m_robot_timer->stop();
}

void map_main::OnActionRobot()
{
  /*  if(this->isActionRobot)
    {
        //if(!closesocket())return;
        ui->pushButton->setText("Action Robot");
        this->isActionRobot = false;
        this->m_robot_timer->stop();
    }
    else
    {
        //if(!connectsocket())return;
        ui->pushButton->setText("Close Robot");
        this->isActionRobot = true;
        this->m_robot_timer->start(200);
    }
    this->update();*/
}


void map_main::OnRobotTimer()
{
    if(!isActionRobot)return;
    if(m_robot_status.updateMap)
    {
        if(!socketThread::isRunOnDownloadMap)
        {
            m_socket->OnDownloadMap(this);
        }
    }else{

    if(!socketThread::isRunOnRobotStatus)
        m_socket->OnRobotStatus(this);
    }
}

void map_main::OnResetPose()
{
    ui->_map_view->resetPose();
}

void map_main::OnLockRot()
{
    if(ui->pushButton_2->isChecked())
    {
        ui->_map_view->locksetGoal();
    }
    else
    {
        ui->_map_view->unlocksetGoal();
    }
}

void map_main::OnSendGoal()
{
    if(!this->isActionRobot)return;
    if(!socketThread::isRunOnSendGoal){
        m_socket->OnSendGoal(this);
    }
    emit ui->pushButton_2->click();
}

void map_main::OnCancelMove()
{
    ui->_map_view->clearGoals();
    if(!socketThread::isRunOnCancelGoal){
        m_socket->OnCancelGoal(this);
    }
}

map_main::map_main(QWidget *parent) :
    QWidget(parent),
    sysIP("192.168.1.121"),
    sysPort("8888"),
    ui(new Ui::map_main)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    setFixedSize(this->width(), this->height());
    ui->_map_menu->init(this, ui->_map_view);

    this->m_map_view_ctl = ui->_map_view;
    this->m_map_menu_ctl = ui->_map_menu;
    this->isActionRobot = false;
    this->m_robot_timer = new QTimer(this);

    connect(ui->pushButton,SIGNAL(clicked()),SLOT(OnActionRobot()));
    connect(ui->pushButton_2,SIGNAL(clicked()),SLOT(OnLockRot()));
    connect(ui->pushButton_3,SIGNAL(clicked()),SLOT(OnResetPose()));
    connect(ui->pushButton_4,SIGNAL(clicked()),SLOT(OnSendGoal()));
    connect(ui->pushButton_5,SIGNAL(clicked()),SLOT(OnCancelMove()));
    connect(ui->_map_view,SIGNAL(sendGoal()),this,SLOT(OnSendGoal()));

    connect(m_robot_timer,SIGNAL(timeout()), this, SLOT(OnRobotTimer()));


    memset(&m_robot_status,0,sizeof(STATUS_PACKAGE_ACK));

    this->m_socket = new socketThread();
    //this->m_robot_timer->start(1);
}

map_main::~map_main()
{
    delete ui;
    delete m_robot_timer;
}

void map_main::OnActivateNode(std::string str,char enable)
{
    if(!this->isActionRobot)return;
    m_socket->OnActivateNode(str,enable);
    //printf("%s.\n",str.c_str());
}
