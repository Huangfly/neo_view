#include "Map_View.h"
#include "ui_Map_View.h"
#include "map_main.h"
#include "map_menu.h"
#include "Map_Model.h"


#define checkImageWidth 10
#define checkImageHeight 10

#define MAP_SIZE 10.0
#define MAP_BACKGROUND_COLOR 0.8
#define MAP_VIEW_SIZE 800.0



//static ST_POSE set_pose;


void Map_View::OnUpdateUI()
{
    this->update();
}

Map_View::Map_View(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::Map_View)
{
    m_robot_info_label = new QLabel(this);
    m_cursor_info_label = new QLabel(this);

    m_robot_info_label->setStyleSheet("QLabel {background: rgba(255, 255, 255, 0);}");
    m_cursor_info_label->setStyleSheet("QLabel {background: rgba(255, 255, 255, 0);}");

    ui->setupUi(this);

    this->isLockRot = false;

    this->isSetPose = false;
    this->m_goals.clear();
    setMouseTracking(true);
    //memset(&map_pose,0,sizeof(ST_POSE));

    main_view_ctl = parent;

    connect(this,SIGNAL(updateUI()),this,SLOT(OnUpdateUI()));

    const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
    const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
    printf("OOpenGL Version：%s\n",OpenGLVersion );
    printf("OGLU Version：%s\n", gluVersion);
}

Map_View::~Map_View()
{
    delete ui;
}



void Map_View::initializeGL()
{
    map_main *main_ptr = (map_main*)this->main_view_ctl;
    main_ptr->m_MapViewCtl.initGL();
}

void Map_View::resizeGL(int w, int h)
{
    map_main *main_ptr = (map_main*)this->main_view_ctl;
    main_ptr->m_MapViewCtl.resizeGL(w, h);
}

void Map_View::paintGL()
{
    map_main *main_ptr = (map_main*)this->main_view_ctl;
    main_ptr->m_MapViewCtl.drawGL();
}

void Map_View::mousePressEvent(QMouseEvent *event)
{
    map_main *main_ptr = (map_main*)this->main_view_ctl;
    main_ptr->m_MapViewCtl.mousePress(event);
}

void Map_View::mouseMoveEvent(QMouseEvent *event)
{
    map_main *main_ptr = (map_main*)this->main_view_ctl;
    main_ptr->m_MapViewCtl.mouseMove(event);
}

void Map_View::mouseReleaseEvent(QMouseEvent *event)
{
    map_main *main_ptr = (map_main*)this->main_view_ctl;
    main_ptr->m_MapViewCtl.mouseRelease(event);
}

void Map_View::wheelEvent(QWheelEvent *event)
{
    map_main *main_ptr = (map_main*)this->parent();
    main_ptr->m_MapViewCtl.mouseWheel(event);
}

void Map_View::showCursorPoseLabel(float x, float y)
{
    QString str;
    str.sprintf("[%3.2f  %3.2f]",x,y);
    m_cursor_info_label->setGeometry(10,this->geometry().height()-50,this->geometry().width()-10,20);
    m_cursor_info_label->setText(str);
    m_cursor_info_label->update();
}

void Map_View::resetPose()
{
    update();
}

void Map_View::locksetGoal()
{
    this->isSetPose = true;
}

void Map_View::unlocksetGoal()
{
    this->isSetPose = false;
}

int Map_View::getGoalsFromList(ST_POSE *pose, char mode)
{
    if(pose == NULL) return -1;
    if(m_goals.size() <= 0) return 0;
    if(mode == 0){
        list<ST_POSE>::iterator it = m_goals.begin();
        pose->x = it->x;
        pose->y = it->y;
        pose->z = it->z;
        memcpy(pose->Quaternion,it->Quaternion,sizeof(pose->Quaternion));

        return m_goals.size();
    }else{
        return 0;
    }
}

void Map_View::clearFrontGoals()
{
    if(m_goals.size() > 0) this->m_goals.pop_front();
}

void Map_View::clearGoals()
{
    if(m_goals.size() > 0) m_goals.clear();
}

void Map_View::resetNextGoal()
{
    if(m_goals.size()<=1)return;
    ST_POSE pose;
    list<ST_POSE>::iterator it = m_goals.begin();
    pose.x = it->x;
    pose.y = it->y;
    pose.z = it->z;
    memcpy(pose.Quaternion,it->Quaternion,sizeof(pose.Quaternion));
    m_goals.pop_front();
    m_goals.push_back(pose);

}

void Map_View::clearMap()
{
}

void Map_View::OnSaveMap()
{
    QFile file("11.map");

    if (file.open(QIODevice::WriteOnly)) //打开方式：可读、二进制方式
    {/*
        char buf[10] = {0};
        for(int i = 0; i<map_width*map_hight; i++)
        {
            sprintf(buf,"%d ",map_data[i]);
            file.write(buf, strlen(buf));
        }
        printf("Save Map\n");*/

        file.close();
    }
}

void Map_View::emitUpdateUI()
{
    emit this->updateUI();
}
