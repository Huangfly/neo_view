#include "map_menu.h"
#include "ui_map_menu.h"

extern QString sysIP;
extern QString sysPort;

#define NODE_ON  1
#define NODE_OFF 0


void map_menu::OnPatrol()
{
    if(ui->PatrolCTL->isChecked())
    {
        ui->PatrolCTL->setText("Patrol\non");
        this->isPatrol = true;
    }
    else
    {
        ui->PatrolCTL->setText("Patrol\noff");
        this->isPatrol = false;
    }
}

void map_menu::OnNav_NavigationMode()
{
    if(ui->Nav_NavigationMode->isChecked()){
        if(!ui->Nav_ExploreMode->isChecked()){
            m_map_main_ctl->actionRobot();
            m_map_main_ctl->OnActivateNode("navigation",1);
        }else{
            ui->Nav_NavigationMode->setChecked(false);
        }

    }else{
        m_map_main_ctl->OnActivateNode("navigation",0);
        m_map_main_ctl->lockRobot();
    }
}

void map_menu::OnNav_ExploreMode()
{
    if(ui->Nav_ExploreMode->isChecked()
        && ui->Nav_NavigationMode->isChecked()){
        m_map_main_ctl->OnActivateNode("explore",1);
    }else{
        ui->Nav_ExploreMode->setChecked(false);
        m_map_main_ctl->OnActivateNode("explore",0);
    }
}



void map_menu::OnClearGoals()
{
    if(m_map_view_ctl == NULL)return;

    m_map_view_ctl->clearGoals();
    m_map_view_ctl->update();
}

void map_menu::OnEditChange()
{
    if(!ui->lineEdit_IP->text().isEmpty())
      sysIP = ui->lineEdit_IP->text();
    if(!ui->lineEdit_Port->text().isEmpty())
      sysPort = ui->lineEdit_Port->text();
    //ui->lineEdit_IP->
}

void map_menu::OnSlam_SaveMap()
{
    this->m_map_view_ctl->OnSaveMap();
    printf("Save Map1\n");
}

void map_menu::OnSlam_ClearMap()
{
    this->m_map_view_ctl->clearMap();
}


void map_menu::OnAction_BtnPress()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    printf("%s  %d\n",btn->objectName().toStdString().c_str(),btn->isChecked());
    m_cmdvel_timer->disconnect(this);
    m_cmdvel_timer->stop();


    //disconnect(m_cmdvel_timer,SIGNAL(timeout()),this);
    if(btn->objectName() == ui->action_left->objectName()){
        connect(m_cmdvel_timer,SIGNAL(timeout()),this,SLOT(OnAction_Left()));
        m_cmdvel_timer->start(200);
    }else if(btn->objectName() == ui->action_right->objectName()){
        connect(m_cmdvel_timer,SIGNAL(timeout()),this,SLOT(OnAction_Right()));
        m_cmdvel_timer->start(200);
    }else if(btn->objectName() == ui->action_forward->objectName()){
        connect(m_cmdvel_timer,SIGNAL(timeout()),this,SLOT(OnAction_Forward()));
        m_cmdvel_timer->start(200);
    }else if(btn->objectName() == ui->action_back->objectName()){
        connect(m_cmdvel_timer,SIGNAL(timeout()),this,SLOT(OnAction_Back()));
        m_cmdvel_timer->start(200);
    }
}

void map_menu::OnAction_BtnRelease()
{
    m_cmdvel_timer->stop();
    m_map_main_ctl->m_socket->OnCmdVel(0.0,0.0,0.0);
    return;
}

void map_menu::OnAction_Left()
{
    printf("OnAction_Left\n");
    m_map_main_ctl->m_socket->OnCmdVel(0.0,0.0,0.8);
}

void map_menu::OnAction_Right()
{
    m_map_main_ctl->m_socket->OnCmdVel(0.0,0.0,-0.8);
}

void map_menu::OnAction_Forward()
{
    m_map_main_ctl->m_socket->OnCmdVel(0.25,0.0,0.0);
}

void map_menu::OnAction_Back()
{
    m_map_main_ctl->m_socket->OnCmdVel(-0.25,0.0,0.0);
}

map_menu::map_menu(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::map_menu)
{
    ui->setupUi(this);

    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->lineEdit_IP->setValidator(pReg);

    QRegExp rx2("-?\\d{1,4}");
    QRegExpValidator *pReg2 = new QRegExpValidator(rx2, this);
    ui->lineEdit_Port->setValidator(pReg2);


    connect(ui->action_left,SIGNAL(pressed()),this,SLOT(OnAction_BtnPress()));
    connect(ui->action_right,SIGNAL(pressed()),this,SLOT(OnAction_BtnPress()));
    connect(ui->action_forward,SIGNAL(pressed()),this,SLOT(OnAction_BtnPress()));
    connect(ui->action_back,SIGNAL(pressed()),this,SLOT(OnAction_BtnPress()));

    connect(ui->action_left,SIGNAL(released()),this,SLOT(OnAction_BtnRelease()));
    connect(ui->action_right,SIGNAL(released()),this,SLOT(OnAction_BtnRelease()));
    connect(ui->action_forward,SIGNAL(released()),this,SLOT(OnAction_BtnRelease()));
    connect(ui->action_back,SIGNAL(released()),this,SLOT(OnAction_BtnRelease()));

    connect(ui->PatrolCTL,SIGNAL(clicked()),this,SLOT(OnPatrol()));
    connect(ui->Nav_NavigationMode,SIGNAL(clicked()),this,SLOT(OnNav_NavigationMode()));
    connect(ui->Nav_ExploreMode,SIGNAL(clicked()),this,SLOT(OnNav_ExploreMode()));
    connect(ui->clearGoals,SIGNAL(clicked()),this,SLOT(OnClearGoals()));

    connect(ui->slam_savemap,SIGNAL(clicked()),this,SLOT(OnSlam_SaveMap()));
    connect(ui->slam_clearMap,SIGNAL(clicked()),this,SLOT(OnSlam_ClearMap()));

    connect(ui->lineEdit_IP,SIGNAL(textChanged(QString)),this,SLOT(OnEditChange()));
    connect(ui->lineEdit_Port,SIGNAL(textChanged(QString)),this,SLOT(OnEditChange()));

    this->isPatrol = ui->PatrolCTL->isChecked();
    m_cmdvel_timer = new QTimer(this);
}

map_menu::~map_menu()
{
    delete ui;
}

void map_menu::init(map_main *p, map_view *pp)
{
    this->m_map_main_ctl = p;
    this->m_map_view_ctl = pp;

    ui->lineEdit_IP->setText(sysIP);
    ui->lineEdit_Port->setText(sysPort);
}
