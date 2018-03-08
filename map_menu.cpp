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

}

void map_menu::OnNav_ExploreMode()
{

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



    connect(ui->PatrolCTL,SIGNAL(clicked()),this,SLOT(OnPatrol()));
    connect(ui->Nav_NavigationMode,SIGNAL(clicked()),this,SLOT(OnNav_NavitionMode()));
    connect(ui->Nav_ExploreMode,SIGNAL(clicked()),this,SLOT(OnNav_ExploreMode()));
    connect(ui->clearGoals,SIGNAL(clicked()),this,SLOT(OnClearGoals()));

    connect(ui->slam_savemap,SIGNAL(clicked()),this,SLOT(OnSlam_SaveMap()));
    connect(ui->slam_clearMap,SIGNAL(clicked()),this,SLOT(OnSlam_ClearMap()));

    connect(ui->lineEdit_IP,SIGNAL(textChanged(QString)),this,SLOT(OnEditChange()));
    connect(ui->lineEdit_Port,SIGNAL(textChanged(QString)),this,SLOT(OnEditChange()));

    this->isPatrol = ui->PatrolCTL->isChecked();
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
