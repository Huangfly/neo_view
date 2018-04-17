#include "Map_Model.h"
#include "main.h"
#include "map_main.h"

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


Map_Model::Map_Model()
{
    //Map paramer init
    m_xRot=m_yRot=m_zRot=0.0;
    m_xMove=m_yMove=m_zMove=0.0;
    m_roll = 1.0;
    map_zoom = MAP_SIZE;
    map_grid_size = MAP_SIZE;
    //Map Data init
    map_data = NULL;
    this->map_resolution = 0.05;
    MapDatas.clear();
    createMapDataPtr(0,0);
    //robot init
    robot_yaw = 0.0f;
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

Map_Model::~Map_Model()
{


}

void Map_Model::init(int w_width,int w_hight)
{
    this->win_width = w_width;
    this->win_hight = w_hight;
}

void Map_Model::initGL()
{
    glEnable( GL_MULTISAMPLE );
    glEnable( GL_DEPTH_TEST );
    glClearColor (MAP_BACKGROUND_COLOR, MAP_BACKGROUND_COLOR, MAP_BACKGROUND_COLOR, 0.0);
    //glLoadIdentity();
    //glEnable(GL_DEPTH_TEST);///开启深度缓存测试
    printf("initializeGL...\n");
}
static unsigned long draw_count = 0;
void Map_Model::drawGL()
{
    printf("drawGL..%ld\n",draw_count);
    draw_count++;
    //if(draw_count%100 != 0)return ;
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glViewport(0, 0, (GLsizei)(this->win_width), (GLsizei)(this->win_hight));
    glOrtho(-MAP_SIZE,MAP_SIZE,-MAP_SIZE,MAP_SIZE,-100,100);
    gluLookAt(0,0,1.0,   0,0,0,    0,1.0,0);
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(m_xMove,m_yMove,0.0);
    glScalef(m_roll,m_roll,m_roll);

    //------Map------
    if(MapDatas.data.size() > 0){
        char *p = map_data;
        float color = 0.0;
        int kk = 0;
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
               p++;
               kk++;
           }
        }
        glFlush();
    }
    //------Pose------

    if(MapDatas.data.size() > 0)
    {
        setStaticPose(0.3,   1.0,0.0,0.0);

        setPose(this->robotPose.x,robotPose.y,robotPose.z + 0.001111f,   0.3,   0.0,0.0,1.0,  this->robotPose.Quaternion);
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
            setGoal(it->x,it->y, 0.0f + 0.0011111f,   0.3,   1.0,0.0,0.0,  NULL);
        }else{
            setGoal(it->x,it->y, 0.0f + 0.0011111f,   0.3,   1.0,0.0,0.0,  NULL);
        }
        it++;
        for(; it!=Goals.end(); it++)
        {
            setGoal(it->x,it->y, 0.0f + 0.0011111f,   0.3,   1.0,0.0,0.0,  NULL);

        }
    }
#endif



    if(LidarDatas.data.size()>0){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glShadeModel(GL_SMOOTH);
        //glBegin(GL_LINE_LOOP);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glBegin(GL_POLYGON_OFFSET_LINE);
        //glBegin(GL_POLYGON);
        glColor4f(1.0f, 0.0f, 0.0f,0.3f);
        float lidar_x,lidar_y;
        float lidar_x_old,lidar_y_old;
        MapWin::LidarDataToMapCoordinate(
                    lidar_x_old,lidar_y_old,
                    this->LidarDatas.pose,
                    LidarDatas.data[0],
                    0.0f);
        for(unsigned int i = 1; i<LidarDatas.data.size();i++){
            MapWin::LidarDataToMapCoordinate(
                        lidar_x,lidar_y,
                        this->LidarDatas.pose,
                        LidarDatas.data[i],
                        i*LidarDatas.angle_increment);
            glBegin(GL_POLYGON);
            glVertex3f(lidar_x_old, lidar_y_old, 0.5f);
            glVertex3f(lidar_x, lidar_y, 0.5f);
            glVertex3f(this->LidarDatas.pose.x, this->LidarDatas.pose.y, 0.5f);
            glEnd();
            lidar_x_old = lidar_x;
            lidar_y_old = lidar_y;

        }
        //glEnd();
    }

    //------Grid------
    glBegin(GL_LINES);
    glColor3f(0.7, 0.7, 0.7);
    int cellcount = map_grid_size;
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

void Map_Model::resizeGL(int w, int h)
{
    /*glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho((map_zoom*w/h),(map_zoom*w/h),map_zoom,map_zoom,-100,100);*/
    printf("resizeGL...\n");
/*
    m_robot_info_label->setGeometry(10,win_hight-30,win_width-10,20);
    m_robot_info_label->setText("");

    m_cursor_info_label->setGeometry(10,win_hight-50,win_width-10,20);
    m_cursor_info_label->setText("");*/
}

void Map_Model::setMapZoom(float scale)
{
    m_roll += scale;
    if(m_roll <= 0.1)m_roll = 0.1;
    printf("Roll:%f\n",m_roll);
}

void Map_Model::addGoal(POSE goalPose)
{
    this->Goals.push_back(goalPose);
}

void Map_Model::popGoal()
{
    if(Goals.size() <= 0) return;
    this->Goals.pop_front();
}

void Map_Model::popGoalBack()
{
    if(Goals.size() <= 0) return;
    this->Goals.pop_back();
}

bool Map_Model::getGoal(POSE &goal)
{
    if(Goals.size() > 0){
        goal = this->Goals.front();
        return true;
    }
    return false;
}

void Map_Model::createMapDataPtr(int w, int h)
{
    if(map_data != NULL)
    {
        clearMapDataPtr();
    }
    map_data = new char[w*h+20];
    map_width = w;
    map_hight = h;
}

void Map_Model::clearMapDataPtr()
{
    if(map_data != NULL)
    {
        delete [] map_data;
        map_width = 0;
        map_hight = 0;
        map_data = NULL;
    }
}

void Map_Model::clearMap()
{
    clearMapDataPtr();
    MapDatas.clear();
}

void Map_Model::updateMap(char *data, int w, int h, float resolution, ST_POSE pose)
{
    /*if((w*h > map_width*map_hight))
    {
        createMapDataPtr(w,h);
    }
    if(map_data == NULL)return;
    map_width = w;
    map_hight = h;
    memcpy(&map_pose,&pose,sizeof(ST_POSE));
    printf("updateMap x:%f y:%f \n",map_pose.x,map_pose.y);
    map_resolution = resolution;
    memcpy(map_data,data,w*h);*/
    MapDatas.clear();
    MapDatas.height = h;
    MapDatas.width = w;
    MapDatas.pushBuf(data,(w*h));
    MapDatas.resolution = resolution;
    MapDatas.pose.FromPOSE(pose);
}


void Map_Model::updateMap(std::vector<char> data, int w, int h, float resolution, ST_POSE pose)
{
    MapDatas.clear();
    MapDatas.height = h;
    MapDatas.width = w;
    MapDatas.pushVector(data,(w*h));
    MapDatas.resolution = resolution;
    MapDatas.pose.FromPOSE(pose);
}

void Map_Model::SaveMap(QString file_name)
{
    QFile file(file_name);

    if (file.open(QIODevice::WriteOnly)) //打开方式：可读、二进制方式
    {
        char buf[125] = {0};
        sprintf(buf,"[%f,%f,%f,%f,%f,%f,%f]",
                MapDatas.pose.x,
                MapDatas.pose.y,
                MapDatas.pose.z,
                MapDatas.pose.Quaternion[0],
                MapDatas.pose.Quaternion[1],
                MapDatas.pose.Quaternion[2],
                MapDatas.pose.Quaternion[3]);
        file.write(buf, strlen(buf));
        sprintf(buf,"[%d,%d,%f]",MapDatas.width,MapDatas.height,MapDatas.resolution);
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
}

void Map_Model::updateRobot(STATUS_PACKAGE_ACK pose)
{

    // is goal areach?
    if( Goals.size()>0 && MapWin::calc_distance(Goals.begin()->x - pose.x, Goals.begin()->y - pose.y) < 0.4f ){
        this->popGoal();
        map_main *ctl = Win::GetMainWin();
        ctl->OnSendGoal();
    }

    memcpy(&this->robotPose,&pose,sizeof(ST_POSE));
    //if(robotPose.movebase_status == 1)printf("action movebase...\n");
    MapWin::Quaternion_to_angle(robot_yaw, robotPose.Quaternion);
    robotPose_.FromPOSE(robotPose);
    robot_yaw = robot_yaw*57.295779;
}

void Map_Model::updateLidarData(std::vector<float> vec, float angle_min,float angle_max,float angle_increment,ST_POSE pose)
{
    LidarDatas.clear();
    LidarDatas.angle_increment = angle_increment;
    LidarDatas.angle_max = angle_max;
    LidarDatas.angle_min = angle_min;
    LidarDatas.pushVector(vec);
    LidarDatas.pose.FromPOSE(pose);
}

