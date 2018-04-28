#include "Map_Model.h"
#include "main.h"
#include "map_main.h"
#include <QOpenGLFunctions>

#define checkImageWidth 10
#define checkImageHeight 10

#define MAP_SIZE 10.0
#define MAP_BACKGROUND_COLOR 0.8
#define MAP_VIEW_SIZE 800.0

static float PI=3.1415926f;

using namespace Win;
using namespace MapWin;

void MapWin::globalpose_to_mappose(
        float &x,
        float &y,
        float win_w,
        float win_h,
        float zoom,
        float roll,
        float moveX,
        float moveY)
{
    float dx = x;
    float dy = y;

    x = ((dx-win_w/2.0f)/(win_w/zoom/2.0f)-moveX)/roll;
    y = ((win_h/2.0f-dy)/(win_h/zoom/2.0f)-moveY)/roll;

}

float MapWin::calc_distance(float x, float y)
{
    return sqrt(x*x+y*y);
}

void MapWin::angle_to_Quaternion(float angle, float quaternion[])
{
    float c3 = cos(angle/2);
    float c2 = cos(0.0f);
    float c1 = cos(0.0f);
    float s3 = sin(angle/2);
    float s2 = sin(0.0f);
    float s1 = sin(0.0f);

    quaternion[0] = 0.0f;
    quaternion[1] = 0.0f;
    quaternion[2] = 0.0f;
    quaternion[3] = 1.0f;



    quaternion[3] = c1*c2*c3 + s1*s2*s3;
    quaternion[0] = s1*c2*c3 - c1*s2*s3;
    quaternion[1] = c1*s2*c3 + c1*s2*c3;
    quaternion[2] = c1*c2*s3 - s1*s2*c3;
}

void MapWin::Quaternion_to_angle(float &angle, float q[4])
{
    angle = atan2f(q[0]*q[1] + q[3]*q[2],
            0.5f - q[1]*q[1] - q[2]*q[2]);
}

void MapWin::LidarDataToMapCoordinate(float &x, float &y, POSE robot_pose, float rang, float angle)
{
    float robot_yaw;
    MapWin::Quaternion_to_angle(robot_yaw,robot_pose.Quaternion);
    robot_yaw += angle-3.141592653f;
    x = rang*cos(robot_yaw)+robot_pose.x;
    y = rang*sin(robot_yaw)+robot_pose.y;
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
   glVertex2f(x, y);
   glVertex2f(x+resolution, y);
   glVertex2f(x+resolution, y+resolution);
   glVertex2f(x, y+resolution);
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
float points[8][3]  = {
    {-0.3f,-1.0f,0.0f},
    {-0.3f,1.0f,0.0f},
    {0.0f,-1.0f,0.0f},
    {0.0f,1.0f,0.0f},
    {-0.3f,-1.0f,0.005f},
    {-0.3f,1.0f,0.005f},
    {0.0f,-1.0f,0.005f},
    {0.0f,1.0f,0.005f}
};
float points1[8][3] = {
    {0.0f,-0.2f,0.0f},
    {0.0f,0.2f,0.0f},
    {1.0f,-0.2f,0.0f},
    {1.0f,0.2f,0.0f},
    {0.0f,-0.2f,0.005f},
    {0.0f,-0.2f,0.005f},
    {1.0f,-0.2f,0.005f},
    {1.0f,0.2f,0.005f}
};

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
   glVertex2f(x, y);
   glVertex2f(side_pose_r_x, side_pose_r_y);
   glVertex2f(top_pose_x, top_pose_y);
   glVertex2f(side_pose_l_x, side_pose_l_y);
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
static void setGoal(float x, float y,float z, float size, float red, float green, float blue, float q[])
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
       glVertex2f(R*cos(2*PI/n*i)+x, R*sin(2*PI/n*i)+y);//, z);
   }
   glEnd();
}

/*-------------------------------------------------------------------------------------------------------*/
Map_Model::Map_Model():
    m_Camera()
{
    if(mutex_update_ptr == NULL){
        mutex_update_ptr = new pthread_mutex_t;
        pthread_mutex_init(mutex_update_ptr,NULL);
    }
    //Map paramer init
    map_grid_size = MAP_SIZE;
    //Map Data init
    MapDatas.clear();
    //robot init
    robotPose_.clear();
    //robot Goals init
    Goals.clear();
    //Lidar Data
    LidarDatas.clear();
   /* LidarDatas.data.push_back(10.2);
    LidarDatas.data.push_back(0.2);
    LidarDatas.data.push_back(8.2);
    LidarDatas.data.push_back(2.2);
    LidarDatas.angle_increment = 0.058;*/
}
/*-------------------------------------------------------------------------------------------------------*/
Map_Model::~Map_Model()
{


}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::init(int w_width,int w_hight)
{
    this->win_width = w_width;
    this->win_hight = w_hight;
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::initGL()
{
    m_Camera.SetPos(QVector3D(0.0f,0.0f,10.0f));
    glShadeModel(GL_SMOOTH);
    glClearDepth( 1.0 );
    glEnable( GL_MULTISAMPLE );
    //glEnable( GL_DEPTH_TEST );
    glDisable(GL_DEPTH_TEST);
    glDepthFunc( GL_LEQUAL );
    glClearColor (MAP_BACKGROUND_COLOR, MAP_BACKGROUND_COLOR, MAP_BACKGROUND_COLOR, 0.0);
    //glLoadIdentity();
    //glEnable(GL_DEPTH_TEST);///开启深度缓存测试
    printf("initializeGL...\n");
}

/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::resizeGL(int w, int h)
{
    printf("w:%dh:%d\n",w,h);
    m_Camera.SetWindows(w,h);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));
    this->win_hight = h;
    this->win_width = w;


    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    /*
     * fovy: camera scense size
     * */
    gluPerspective( 90.0, (GLfloat)(w)/(GLfloat)(h), 0.1f, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    printf("resizeGL...\n");
}
/*-------------------------------------------------------------------------------------------------------*/
//static unsigned long draw_count = 0;
void Map_Model::drawGL()
{
    //printf("drawGL..%ld\n",draw_count);
    //draw_count++;
    pthread_mutex_lock(mutex_update_ptr);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    //glPushMatrix( );
    m_Camera.Apply();

    //------Map------
    if(MapDatas.data.size() > 0){
        float color = 0.0;
        for(int i = 0; i<MapDatas.height; i++)
        {
           for(int j = 0; j<MapDatas.width; j++)
           {
               char ch = MapDatas.data[(i*MapDatas.width)+j];
               if(ch == 0){
                   color = 1.0;
               }else if(ch < 98 && ch > 0){
                   color = (127-ch)/127.0f;
               }else if(ch >= 98){
                   color = 0.0;
               }else{
                   color = MAP_BACKGROUND_COLOR;
               }
               setPoint(j*MapDatas.resolution+MapDatas.pose.x ,
                        i*MapDatas.resolution+MapDatas.pose.y,
                        0.001,
                        MapDatas.resolution,
                        color,color,color);
           }
        }
        //glFlush();
    }

    //------GlobalPath-----
    if(GlobalPaths.data.size()>0){
        std::vector<QVector2D>::iterator it = GlobalPaths.data.begin();
        for(; it != GlobalPaths.data.end(); it++)
        {
            setGoal(it->x(),it->y(), 0.0f + 0.000002,   0.03,   0.0,1.0,0.0,  NULL);

        }
    }

    //------Pose------
    //if(MapDatas.data.size() > 0)
    {
        setStaticPose(0.3,   1.0,0.0,0.0);

        setPose(this->robotPose_.x,robotPose_.y,robotPose_.z + 0.0000001,   0.3,   0.0,0.0,1.0,  this->robotPose_.Quaternion);
    }
    //------Label------
#if 0
    char buf[50];
    sprintf(buf,"robot pose [%3.2f %3.2f %3.2f]",this->robotPose.x,robotPose.y,robot_yaw);
    m_robot_info_label->setText(buf);
#endif

    //------Goals------
#if 1
    if(Goals.size()>0){
        std::list<MapWin::POSE>::iterator it = Goals.begin();
        if(1){//robotPose.movebase_status == 1){
            setGoal(it->x,it->y, 0.0f + 0.000002,   0.2,   1.0,0.0,0.0,  NULL);
        }else{
            setGoal(it->x,it->y, 0.0f + 0.000002,   0.2,   1.0,0.0,0.0,  NULL);
        }
        it++;
        for(; it!=Goals.end(); it++)
        {
            setGoal(it->x,it->y, 0.0f + 0.000002,   0.2,   1.0,0.0,0.0,  NULL);

        }
    }
#endif


    //------Lidar------
    if(LidarDatas.data.size()>0){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor4f(1.0f, 0.0f, 0.0f,0.3f);
        float lidar_x,lidar_y;
        float lidar_x_old,lidar_y_old;
        float lidar_x_start,lidar_y_start;
        unsigned int i = 0;
        for(; i<LidarDatas.data.size();i++){
            if(std::isinf(LidarDatas.data[i]))continue;
            MapWin::LidarDataToMapCoordinate(
                        lidar_x_old,lidar_y_old,
                        this->LidarDatas.pose,
                        LidarDatas.data[i],
                        i*LidarDatas.angle_increment);
            lidar_x_start = lidar_x_old;
            lidar_y_start = lidar_y_old;
        }
        for(; i<LidarDatas.data.size();i++){
            if(std::isinf(LidarDatas.data[i]))continue;
            MapWin::LidarDataToMapCoordinate(
                        lidar_x,lidar_y,
                        this->LidarDatas.pose,
                        LidarDatas.data[i],
                        i*LidarDatas.angle_increment);

            glBegin(GL_POLYGON);
            glColor4f(1.0f, 0.0f, 0.0f,0.1f);
            glVertex2f(lidar_x_old, lidar_y_old);
            glVertex2f(lidar_x, lidar_y);
            glVertex2f(this->LidarDatas.pose.x, this->LidarDatas.pose.y);
            glEnd();
            setPoint(lidar_x ,
                     lidar_y,
                     0.001,
                     MapDatas.resolution*2,
                     1.0f,0.0f,0.0f);
            lidar_x_old = lidar_x;
            lidar_y_old = lidar_y;

        }
        glBegin(GL_POLYGON);
        glColor4f(1.0f, 0.0f, 0.0f,0.1f);
        glVertex2f(lidar_x_old, lidar_y_old);
        glVertex2f(lidar_x_start, lidar_y_start);
        glVertex2f(this->LidarDatas.pose.x, this->LidarDatas.pose.y);
        glEnd();
        //glEnd();
    }


    //------Grid------
    glBegin(GL_LINES);
    glColor3f(0.7, 0.7, 0.7);
    int cellcount = map_grid_size;
    for(int i = -cellcount; i<=cellcount; i++)
    {
       glVertex2d(i,cellcount);
       glVertex2d(i,-cellcount);
    }
    for(int i = -cellcount; i<=cellcount; i++)
    {
       glVertex2d(cellcount,i);
       glVertex2d(-cellcount,i);
    }
    glEnd();
    //glFlush();

    //glPopMatrix();
    pthread_mutex_unlock(mutex_update_ptr);
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::addGoal(POSE goalPose)
{
    this->Goals.push_back(goalPose);
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::popGoal()
{
    if(Goals.size() <= 0) return;
    this->Goals.pop_front();
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::popGoalBack()
{
    if(Goals.size() <= 0) return;
    this->Goals.pop_back();
}
/*-------------------------------------------------------------------------------------------------------*/
bool Map_Model::getGoal(POSE &goal)
{
    if(Goals.size() > 0){
        goal = this->Goals.front();
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::clearMap()
{
    MapDatas.clear();
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::updateMap(char *data, int w, int h, float resolution, ST_POSE pose)
{
    pthread_mutex_lock(mutex_update_ptr);
    MapDatas.clear();
    MapDatas.height = h;
    MapDatas.width = w;
    MapDatas.pushBuf(data,(w*h));
    MapDatas.resolution = resolution;
    MapDatas.pose.FromPOSE(pose);
    pthread_mutex_unlock(mutex_update_ptr);
}

/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::updateMap(std::vector<char> data, int w, int h, float resolution, ST_POSE pose)
{
    pthread_mutex_lock(mutex_update_ptr);
    MapDatas.clear();
    MapDatas.height = h;
    MapDatas.width = w;
    MapDatas.pushVector(data,(w*h));
    MapDatas.resolution = resolution;
    MapDatas.pose.FromPOSE(pose);
    pthread_mutex_unlock(mutex_update_ptr);
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::GetMap(MapVector &data)
{
    pthread_mutex_lock(mutex_update_ptr);
    data = this->MapDatas;
    pthread_mutex_unlock(mutex_update_ptr);
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::SaveMap(QString file_name)
{
    pthread_mutex_lock(mutex_update_ptr);
    QFile file(file_name);

    if (file.open(QIODevice::WriteOnly)) //打开方式：可读、二进制方式
    {
        char buf[125] = {0};
        sprintf(buf,"[%f,%f,%f,%f,%f,%f,%f]\n",
                MapDatas.pose.x,
                MapDatas.pose.y,
                MapDatas.pose.z,
                MapDatas.pose.Quaternion[0],
                MapDatas.pose.Quaternion[1],
                MapDatas.pose.Quaternion[2],
                MapDatas.pose.Quaternion[3]);
        file.write(buf, strlen(buf));
        sprintf(buf,"[%d,%d,%f]\n",MapDatas.width,MapDatas.height,MapDatas.resolution);
        file.write(buf, strlen(buf));
        file.write("[", 1);

        for(unsigned int i = 0; i<MapDatas.data.size(); i++)
        {
            sprintf(buf,"%d ",MapDatas.data[i]);
            file.write(buf, strlen(buf));
        }
        file.write("]", 1);
        printf("Save Map\n");

        file.close();
    }
    pthread_mutex_unlock(mutex_update_ptr);
}

void Map_Model::LoadMap(QString file_name)
{
    pthread_mutex_lock(mutex_update_ptr);
    QFile file(file_name);

    while (file.open(QIODevice::ReadOnly)) //打开方式：可读、二进制方式
    {
        MapDatas.clear();
        //----- pose
        QByteArray line_poae = file.readLine();
        line_poae.remove(0,1);
        line_poae.remove(line_poae.size()-2,2);
        QString str_pose(line_poae);

        printf("line1:%s\n",str_pose.toStdString().c_str());
        QStringList pose_list = str_pose.split(",");
        if(pose_list.size() < 7) break;
        MapDatas.pose.x = pose_list[0].toFloat();
        MapDatas.pose.y = pose_list[1].toFloat();
        MapDatas.pose.z = pose_list[2].toFloat();
        MapDatas.pose.Quaternion[0] = pose_list[3].toFloat();
        MapDatas.pose.Quaternion[1] = pose_list[4].toFloat();
        MapDatas.pose.Quaternion[2] = pose_list[5].toFloat();
        MapDatas.pose.Quaternion[3] = pose_list[6].toFloat();
        for(int i = 0; i < pose_list.size(); i++){
            printf(":%s\n",pose_list[i].toStdString().c_str());
        }

        //----- paramer
        QByteArray line_paramer = file.readLine();
        line_paramer.remove(0,1);
        line_paramer.remove(line_paramer.size()-2,2);
        QString paramer_str(line_paramer);

        printf("line2:%s\n",paramer_str.toStdString().c_str());
        QStringList paramer_list = paramer_str.split(",");
        if(paramer_list.size() < 3) break;
        MapDatas.width = paramer_list[0].toInt();
        MapDatas.height = paramer_list[1].toInt();
        MapDatas.resolution = paramer_list[2].toFloat();
        for(int i = 0; i < paramer_list.size(); i++){
            printf(":%s\n",paramer_list[i].toStdString().c_str());
        }

        //----- data
        QByteArray line_data = file.readLine();
        line_data.remove(0,1);
        line_data.remove(line_data.size()-1,2);
        QString data_str(line_data);

        printf("line3:%s\n",paramer_str.toStdString().c_str());
        QStringList data_list = data_str.split(" ");
        if(data_list.size() < (MapDatas.width*MapDatas.height)) break;
        std::vector<char> _data;
        for(int i = 0; i < data_list.size(); i++){
            _data.push_back((char)(data_list[i].toInt()));
        }
        MapDatas.pushVector(_data);
        printf("LoadMap end\n");
        break;
    }
    file.close();
    pthread_mutex_unlock(mutex_update_ptr);
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::updateRobot(STATUS_PACKAGE_ACK pose)
{
    pthread_mutex_lock(mutex_update_ptr);
    // is goal areach?
    if( Goals.size()>0 && MapWin::calc_distance(Goals.begin()->x - pose.x, Goals.begin()->y - pose.y) < 0.4f ){
        this->popGoal();
        map_main *ctl = Win::GetMainWin();
        ctl->OnSendGoal();
    }

    robotPose_.FromPOSE(pose);

    pthread_mutex_unlock(mutex_update_ptr);
}
/*-------------------------------------------------------------------------------------------------------*/
void Map_Model::updateLidarData(std::vector<float> vec, float angle_min,float angle_max,float angle_increment,ST_POSE pose)
{
    pthread_mutex_lock(mutex_update_ptr);
    LidarDatas.clear();
    LidarDatas.angle_increment = angle_increment;
    LidarDatas.angle_max = angle_max;
    LidarDatas.angle_min = angle_min;
    LidarDatas.pushVector(vec);
    LidarDatas.pose.FromPOSE(pose);
    pthread_mutex_unlock(mutex_update_ptr);
}

void Map_Model::clearLidar()
{
    pthread_mutex_lock(mutex_update_ptr);
    LidarDatas.clear();
    pthread_mutex_unlock(mutex_update_ptr);
}

void Map_Model::updateGlobalPath(std::vector<QVector2D> vec)
{
    pthread_mutex_lock(mutex_update_ptr);
    GlobalPaths.clear();
    GlobalPaths.pushVector(vec);
    pthread_mutex_unlock(mutex_update_ptr);
}

void Map_Model::clearGlobalPath()
{
    pthread_mutex_lock(mutex_update_ptr);
    GlobalPaths.clear();
    pthread_mutex_unlock(mutex_update_ptr);
}

