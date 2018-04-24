#include "ui_map_main.h"
#include "map_main.h"

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

void map_main::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_F2:
        {
            mFullScreen = !mFullScreen;
            if(mFullScreen){
                this->showFullScreen();
            }else{
                this->showNormal();
            }

            //ui->_map_view->FullScreen();
            break;
        }
        default:
            break;
    }
}

void map_main::resizeEvent(QResizeEvent *size)
{
    QSize mapViewSize = QSize(size->size().width(),size->size().height()-100);
    QSize menuViewSize = QSize(size->size().width(),100);
    printf("MainWin resize\n");
    ui->_map_view->resize(mapViewSize);
    ui->_map_menu->resize(menuViewSize);
}

void map_main::OnLockRobot()
{
    if(ui->pushButton_1->isChecked())
    {
        m_MapViewCtl.lockMove();
    }
    else
    {
        m_MapViewCtl.unlockMove();
    }
}

void map_main::OnRobotTimer()
{

    if(!isActionRobot)return;

    timerCount++;
    if(timerCount%1 == 0){
        m_socketMag->OnDownloadLidarData();
    }



    if(timerCount%5 == 0)
    {
        m_socketMag->OnDownloadMap();
    }

    m_socketMag->OnRobotStatus();
}

void map_main::OnResetPose()
{
    //ui->_map_view->resetPose();
}

void map_main::OnLockRot()
{
    if(ui->pushButton_2->isChecked())
    {
        this->m_MapViewCtl.enableSetGoal();
    }
    else
    {
        this->m_MapViewCtl.unableSetGoal();
    }
}

void map_main::OnSendGoal()
{
    if(!this->isActionRobot)return;
    m_socketMag->OnSendGoal();
    //emit ui->pushButton_2->click();
}

void map_main::OnCancelMove()
{
    m_MapViewCtl.ClearGoals();
    m_socketMag->OnCancelGoal();
}

map_main::map_main(QWidget *parent) :
    QWidget(parent),
    m_MapViewCtl(),
    m_MapViewModle(),
    ui(new Ui::map_main)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    //setFixedSize(this->width(), this->height());
    mFullScreen = false;
    ui->_map_menu->init(this, ui->_map_view);
    //this->m_map_view_ctl = ui->_map_view;
    //this->m_map_menu_ctl = ui->_map_menu;
    this->isActionRobot = false;
    this->m_robot_timer = new QTimer(this);

    connect(ui->pushButton_1,SIGNAL(clicked()),SLOT(OnLockRobot()));
    connect(ui->pushButton_2,SIGNAL(clicked()),SLOT(OnLockRot()));
    connect(ui->pushButton_3,SIGNAL(clicked()),SLOT(OnResetPose()));
    connect(ui->pushButton_4,SIGNAL(clicked()),SLOT(OnSendGoal()));
    connect(ui->pushButton_5,SIGNAL(clicked()),SLOT(OnCancelMove()));
    connect(ui->_map_view,SIGNAL(sendGoal()),this,SLOT(OnSendGoal()));

    connect(m_robot_timer,SIGNAL(timeout()), this, SLOT(OnRobotTimer()));


    memset(&m_robot_status,0,sizeof(STATUS_PACKAGE_ACK));

    this->m_socketMag = new MapTcp::ManagerSocket();


    this->m_MapViewModle.init(ui->_map_view->geometry().width(),ui->_map_view->geometry().height());

    this->m_MapViewCtl.init(ui->_map_view,&this->m_MapViewModle);
    this->m_MapViewCtl.create();

    //mLayout = new QGridLayout;


    timerCount = 0;

}

map_main::~map_main()
{
    delete ui;
    delete m_robot_timer;
}

void map_main::OnActivateNode(QString str,bool enable)
{
    //if(!this->isActionRobot)return;
    m_socketMag->OnActivateNode(str,enable);
    //printf("%s.\n",str.c_str());
}
