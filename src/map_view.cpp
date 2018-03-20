#include "map_view.h"
#include "ui_map_view.h"
#include "map_menu.h"
#include <GL/gl.h>
#include <GL/glu.h>
#define checkImageWidth 10
#define checkImageHeight 10

#define MAP_SIZE 10.0
#define MAP_BACKGROUND_COLOR 0.8
#define MAP_VIEW_SIZE 800.0

static ST_POSE set_pose;
float PI=3.1415926f;

static float calc_distance(float x, float y)
{
    return sqrt(x*x+y*y);
}

static void angle_to_Quaternion(float angle, float q[4])
{
    q[0] = 0.0f;
    q[1] = 0.0f;
    q[2] = 0.0f;
    q[3] = 1.0f;

    float c3 = cos(angle/2);
    float c2 = cos(0.0f);
    float c1 = cos(0.0f);
    float s3 = sin(angle/2);
    float s2 = sin(0.0f);
    float s1 = sin(0.0f);

    q[3] = c1*c2*c3 + s1*s2*s3;
    q[0] = s1*c2*c3 - c1*s2*s3;
    q[1] = c1*s2*c3 + c1*s2*c3;
    q[2] = c1*c2*s3 - s1*s2*c3;
}

static void Quaternion_to_angle(float &angle, float q[4])
{
    angle = atan2f(q[0]*q[1] + q[3]*q[2],
            0.5f - q[1]*q[1] - q[2]*q[2]);
}


//         (x,y+a)        (x+a,y+a)
//             ____________
//            |            |
//            |            |
//            |            |
//            |            |
//            |____________|
//          (x,y)         (x+a,y)
//
static void setPoint(float x, float y,float z,float resolution, float red, float green, float blue)
{
   glBegin(GL_QUADS);
   glColor3f(red, green, blue);
   glVertex3f(x, y, z);
   glVertex3f(x+resolution, y, z);
   glVertex3f(x+resolution, y+resolution, z);
   glVertex3f(x, y+resolution, z);
   glEnd();
}

/*             __
//            |  |__
//            |     |
//            |  ___|
//            |__|
//
//
//
*/
float points[8][3]  = {{-0.3f,-1.0f,0.0f},{-0.3f,1.0f,0.0f},{0.0f,-1.0f,0},{0.0f,1.0f,0.0f},{-0.3f,-1.0f,0.5},{-0.3f,1.0f,0.5},{0.0f,-1.0f,0.5},{0.0f,1.0f,0.5}};
float points1[8][3] = {{0.0f,-0.2f,0.0f},{0.0f,0.2f,0.0f},{1.0f,-0.2f,0},{1.0f,0.2f,0.0f},{0.0f,-0.2f,0.5},{0.0f,-0.2f,0.5},{1.0f,-0.2f,0.5},{1.0f,0.2f,0.5}};

static void quad(int n1, int n2, int n3, int n4, float size)
{
    glBegin(GL_QUADS);

            glVertex3f(points[n1][0]*size,points[n1][1]*size,points[n1][2]*size);

            glVertex3f(points[n2][0]*size,points[n2][1]*size,points[n2][2]*size);

            glVertex3f(points[n3][0]*size,points[n3][1]*size,points[n3][2]*size);

            glVertex3f(points[n4][0]*size,points[n4][1]*size,points[n4][2]*size);

    glEnd();
}

static void quad1(int n1, int n2, int n3, int n4, float size)
{
    glBegin(GL_QUADS);

            glVertex3f(points1[n1][0]*size,points1[n1][1]*size,points1[n1][2]*size);

            glVertex3f(points1[n2][0]*size,points1[n2][1]*size,points1[n2][2]*size);

            glVertex3f(points1[n3][0]*size,points1[n3][1]*size,points1[n3][2]*size);

            glVertex3f(points1[n4][0]*size,points1[n4][1]*size,points1[n4][2]*size);

    glEnd();
}

static void setStaticPose(float size, float red, float green, float blue)
{

   glColor3f(red, green, blue);
   quad(6,2,3,7,size);

   quad(5,1,0,4,size);

   quad(7,3,1,5,size);

   quad(4,0,2,6,size);

   quad(2,0,1,3,size);

   quad(7,5,4,6,size);


   quad1(6,2,3,7,size);

   quad1(5,1,0,4,size);

   quad1(7,3,1,5,size);

   quad1(4,0,2,6,size);

   quad1(2,0,1,3,size);

   quad1(7,5,4,6,size);
}

/*
//             /\
//            /  \
//           /    \
//          /_    _\
//            |  |
//            |__|
//
*/
static void setPose(float x, float y,float z, float size, float red, float green, float blue, float q[4])
{
   float PI=3.1415926f;
   float R=size;  //半径
   float angle = atan2f(q[0]*q[1] + q[3]*q[2],
           0.5f - q[1]*q[1] - q[2]*q[2]);

   if(angle >= 2*PI)angle = 0.0f;
   float top_pose_x = x+R*cos(angle);
   float top_pose_y = y+R*sin(angle);
   float side_pose_r_x = x+R/2.0f*cos(angle-PI/1.5);
   float side_pose_r_y = y+R/2.0f*sin(angle-PI/1.5);

   float side_pose_l_x = x+R/2.0f*cos(angle+PI/1.5);
   float side_pose_l_y = y+R/2.0f*sin(angle+PI/1.5);
   glBegin(GL_POLYGON);
   glColor3f(red, green, blue);
   glVertex3f(x, y, z);
   glVertex3f(side_pose_r_x, side_pose_r_y, z);
   glVertex3f(top_pose_x, top_pose_y, z);
   glVertex3f(side_pose_l_x, side_pose_l_y, z);
   glEnd();
}
/*
// |
// |
// |
// |
// |
// |
*/
static void setGoal(float x, float y,float z, float size, float red, float green, float blue, float q[4])
{

   float R;  //半径
   int n = 3600;
   float angle;

   if(q == NULL)
   {
      angle = 0.0f;
   }
   else
   {
      angle = atan2f(q[0]*q[1] + q[3]*q[2],
              0.5f - q[1]*q[1] - q[2]*q[2]);
   }
   R=angle;
   R=size;
   glBegin(GL_POLYGON);
   glColor3f(red, green, blue);
   for(int i = 0; i<n; i++)
   {
       glVertex3f(R*cos(2*PI/n*i)+x, R*sin(2*PI/n*i)+y, z);
   }
   glEnd();
}


void map_view::OnUpdateUI()
{
    this->update();
}

map_view::map_view(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::map_view)
{
    m_robot_info_label = new QLabel(this);
    m_cursor_info_label = new QLabel(this);

    m_robot_info_label->setStyleSheet("QLabel {background: rgba(255, 255, 255, 0);}");
    m_cursor_info_label->setStyleSheet("QLabel {background: rgba(255, 255, 255, 0);}");

    ui->setupUi(this);

    map_zoom = MAP_SIZE;

    m_xRot=m_yRot=m_zRot=0.0;
    m_xMove=m_yMove=m_zMove=0.0;
    m_roll = 1.0;
    map_data = NULL;
    create_MapData(0,0);
    this->map_zoom = MAP_SIZE;
    this->isLockRot = false;
    this->map_resolution = 0.05;

    this->isSetPose = false;
    this->m_goals.clear();
    robot_yaw = 0.0f;
    setMouseTracking(true);
    memset(&map_pose,0,sizeof(ST_POSE));
    map_pose.Quaternion[3] = 1.0f;

    main_view_ctl = (map_main*)parent;

    connect(this,SIGNAL(updateUI()),this,SLOT(OnUpdateUI()));
}

map_view::~map_view()
{
    delete ui;
    delete_MapData();
}



void map_view::initializeGL()
{
    glClearColor (MAP_BACKGROUND_COLOR, MAP_BACKGROUND_COLOR, MAP_BACKGROUND_COLOR, 0.0);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);///开启深度缓存测试
    printf("initializeGL...\n");
}

void map_view::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho((map_zoom*w/h),(map_zoom*w/h),map_zoom,map_zoom,-100,100);
    printf("resizeGL...\n");

    m_robot_info_label->setGeometry(10,h-30,w-10,20);
    m_robot_info_label->setText("");

    m_cursor_info_label->setGeometry(10,h-50,w-10,20);
    m_cursor_info_label->setText("");
}



void map_view::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, (GLsizei)(this->geometry().width()), (GLsizei)(this->geometry().height()));
    glOrtho(-MAP_SIZE,MAP_SIZE,-MAP_SIZE,MAP_SIZE,-100,100);
    gluLookAt(0,0,1.0,   0,0,0,    0,1.0,0);
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(m_xMove,m_yMove,0.0);
    glScalef(m_roll,m_roll,m_roll);

    //------Map------
    char *p = map_data;
    float color = 0.0;
    int kk = 0;
    for(int i = 0; i<map_hight; i++)
    {
       for(int j = 0; j<map_width; j++)
       {
           if(*p == 0){
               color = 1.0;
           }else if(*p < 98 && *p > 0){
               color = (127-*p)/127.0f;
           }else if(*p >= 98){
               color = 0.0;
           }else{
               color = MAP_BACKGROUND_COLOR;
           }
           setPoint(j*map_resolution+map_pose.x ,i*map_resolution+map_pose.y,0.001, map_resolution, color,color,color);
           p++;
           kk++;
       }
    }
    glFlush();
    //------Pose------

    if(map_width > 0)
    {
        setStaticPose(0.3,   1.0,0.0,0.0);

        setPose(this->robotPose.x,robotPose.y,robotPose.z + 0.001111f,   0.3,   0.0,0.0,1.0,  this->robotPose.Quaternion);
    }
    //------Label------
    char buf[50];
    sprintf(buf,"robot pose [%3.2f %3.2f %3.2f]",this->robotPose.x,robotPose.y,robot_yaw);
    m_robot_info_label->setText(buf);
    //------Goals------
    if(m_goals.size()>0){
        list<ST_POSE>::iterator it = m_goals.begin();
        if(robotPose.movebase_status == 1){
            setGoal(it->x,it->y, 0.0f + 0.0011111f,   0.1,   0.0,1.0,0.0,  NULL);
        }else{
            setGoal(it->x,it->y, 0.0f + 0.0011111f,   0.1,   0.0,1.0,1.0,  NULL);
        }
        it++;
        for(; it!=m_goals.end(); it++)
        {
            setGoal(it->x,it->y, 0.0f + 0.0011111f,   0.1,   0.0,1.0,1.0,  NULL);

        }
    }
    //------Grid------
    glBegin(GL_LINES);
    glColor3f(0.7, 0.7, 0.7);
    int cellcount = map_zoom;
    float grid_z = 0.0011;
           for(int i = -cellcount; i<=cellcount; i++)
           {
               glVertex3d(i,cellcount,grid_z);
               glVertex3d(i,-cellcount,grid_z);
           }
           for(int i = -cellcount; i<=cellcount; i++)
           {
               glVertex3d(cellcount,i,grid_z);
               glVertex3d(-cellcount,i,grid_z);
           }
    glEnd();
    glFlush();


    glLoadIdentity();

}

void map_view::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    switch(key)
    {
    case Qt::Key_Z:
        m_roll -= 0.2;
        break;
    case Qt::Key_X:
        m_roll += 0.2;
        break;
    default:
        break;
    }
    //update();
}

void map_view::globalpose_to_mappose(float &x, float &y)
{
    float dx = x;
    float dy = y;
    float w = (float)this->geometry().width();
    float h = (float)this->geometry().height();

    x = ((dx-w/2.0f)/(w/map_zoom/2.0f)-m_xMove)/m_roll;
    y = ((h/2.0f-dy)/(h/map_zoom/2.0f)-m_yMove)/m_roll;

}

void map_view::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    if(this->isSetPose)
    {
        set_pose.z = 0.0f;

        set_pose.x = (float)m_lastPos.x();
        set_pose.y = (float)m_lastPos.y();
        globalpose_to_mappose(set_pose.x,set_pose.y);

        float aaa = atan2f((set_pose.y-robotPose.y), (set_pose.x-robotPose.x));
        angle_to_Quaternion(aaa, set_pose.Quaternion);
    }
}

void map_view::mouseMoveEvent(QMouseEvent *event)
{

    float dx = (float)event->x();
    float dy = (float)event->y();
    float nx = (float)m_lastPos.x();
    float ny = (float)m_lastPos.y();
    globalpose_to_mappose(dx,dy);
    globalpose_to_mappose(nx,ny);
    //float x = (((float)(m_lastPos.x())-(float)this->geometry().width())/40.0f-m_xMove)/m_roll;
    //float y = (((float)this->geometry().height()-(float)(m_lastPos.y())/40.0f)-m_yMove)/m_roll;
    QString str;
    str.sprintf("[%3.2f  %3.2f]",dx,dy);
    m_cursor_info_label->setText(str);
    m_cursor_info_label->update();
    m_lastPos = event->pos();

    if (event->buttons() & Qt::LeftButton && !this->isSetPose)
    {

       m_xMove+=(dx-nx);

       m_yMove+=(dy-ny);
       printf("move:%f %f\n",m_xMove,m_yMove);

       update();
    }

}

void map_view::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->isSetPose || event->buttons() & Qt::LeftButton)
    {
        m_goals.push_back(set_pose);
        update();
    }
}

void map_view::wheelEvent(QWheelEvent *event)
{
    float rolldistance=event->delta();

    m_roll+=rolldistance/1000;
    if(m_roll <= 0.1)m_roll = 0.1;
    printf("Roll:%f\n",m_roll);
    update();
}

void map_view::create_MapData(int w, int h)
{
    if(map_data != NULL)
    {
        delete_MapData();
    }
    map_data = (new char[w*h+20]);
    /*for(int i = 0; i < w; i++)
    {
        map_data[i] = new GLubyte *[h];
        for(int j = 0; j<h; j++)
        {
            map_data[i][j] = new GLubyte[3];
            //memset(&map_data[i][j][0],0,3);
        }
    }*/
    map_width = w;
    map_hight = h;

}

void map_view::delete_MapData()
{
    if(map_data == NULL)
    {
        return;
    }
   /* for(int i = 0; i < map_width; i++)
    {
        for(int j = 0; j<map_hight; j++)
        {
            delete map_data[i][j];
        }
        delete map_data[i];
    }*/
    delete [] map_data;
    map_width = 0;
    map_hight = 0;
    map_data = NULL;
}

void map_view::updateRobotPose(STATUS_PACKAGE_ACK pose)
{
    // reached Goal?

    if( calc_distance(m_goals.begin()->x - pose.x, m_goals.begin()->y - pose.y) < 0.4f ){
        if(!main_view_ctl->isPatrol() || m_goals.size() < 2) {
            clearFrontGoals();
        }else{
            resetNextGoal();
        }

        if(m_goals.size()>0)
        {
           //(map_main*)(this->parent())->sendGoal();
            emit sendGoal();
        }
    }

    memcpy(&this->robotPose,&pose,sizeof(STATUS_PACKAGE_ACK));
    if(robotPose.movebase_status == 1)printf("action movebase...\n");
    Quaternion_to_angle(robot_yaw, robotPose.Quaternion);
    robot_yaw = robot_yaw*57.295779;
    //update();
}

void map_view::updateMap(char *data, int w, int h, float resolution,ST_POSE pose)
{

    if((w*h > map_width*map_hight))
    {
        delete_MapData();
        create_MapData(w,h);
    }
    if(map_data == NULL)return;
    map_width = w;
    map_hight = h;

    memcpy(&map_pose,&pose,sizeof(ST_POSE));
    printf("updateMap x:%f y:%f \n",map_pose.x,map_pose.y);
    map_resolution = resolution;
    memcpy(map_data,data,w*h);

}

void map_view::resetPose()
{
    this->m_roll = 1.0;
    this->m_xMove = this->m_yMove = 0.0;
    this->m_xRot = this->m_yRot = 0.0;
    update();
}

void map_view::locksetGoal()
{
    this->isSetPose = true;
}

void map_view::unlocksetGoal()
{
    this->isSetPose = false;
    //this->m_goals.clear();
}

int map_view::getGoalsFromList(ST_POSE *pose, char mode)
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

void map_view::clearFrontGoals()
{
    if(m_goals.size() > 0) this->m_goals.pop_front();
}

void map_view::clearGoals()
{
    if(m_goals.size() > 0) m_goals.clear();
}

void map_view::resetNextGoal()
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

void map_view::clearMap()
{
    clearGoals();
    delete_MapData();
    emit map_view::updateUI();
}

void map_view::OnSaveMap()
{
    QFile file("tt.txt");

    if (file.open(QIODevice::WriteOnly)) //打开方式：可读、二进制方式
    {
        char buf[10] = {0};
        for(int i = 0; i<map_width*map_hight; i++)
        {
            sprintf(buf,"%d ",map_data[i]);
            file.write(buf, strlen(buf));
        }
        printf("Save Map\n");

        file.close();
    }
}

void map_view::emitUpdateUI()
{
    emit this->updateUI();
}
