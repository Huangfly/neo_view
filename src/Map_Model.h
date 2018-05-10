#ifndef MODELGL_H
#define MODELGL_H

#include <QObject>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include <string.h>
#include "map_socket_type.h"
#include "Camera.h"

namespace MapWin {




    class POSE{
    public:
        POSE(){ x = 0.0f;y = 0.0f; z = 0.0f; }
        float x;
        float y;
        float z;
        float Quaternion[4];//xyzw
        void ToPOSE(Neo_Packet::GOAL_PACKAGE_POP *pop){

            pop->x = x;
            pop->y = y;
            pop->z = z;

            memcpy(pop->Quaternion,Quaternion,sizeof(Quaternion));
        }

        void clear(){
            x = 0.0f;y = 0.0f; z = 0.0f;
            memset(Quaternion,0,sizeof(Quaternion));
            Quaternion[3] = 1.0f;
        }

        void ToPOSE(ST_POSE *pop){

            pop->x = x;
            pop->y = y;
            pop->z = z;

            memcpy(pop->Quaternion,Quaternion,sizeof(Quaternion));
        }

        void ToPOSE(Neo_Type::POSE *pop){

            pop->x = x;
            pop->y = y;
            pop->z = z;

            memcpy(pop->Quaternion,Quaternion,sizeof(Quaternion));
        }

        void FromPOSE(Neo_Packet::STATUS_PACKAGE_ACK pop){
            x = pop.x;
            y = pop.y;
            z = pop.z;

            memcpy(Quaternion,pop.Quaternion,sizeof(Quaternion));
        }

        void FromPOSE(ST_POSE pop){
            x = pop.x;
            y = pop.y;
            z = pop.z;

            memcpy(Quaternion,pop.Quaternion,sizeof(Quaternion));
        }

        POSE& operator=(const POSE b)
        {
            this->x = b.x;
            this->y = b.y;
            this->z = b.z;

            memcpy(this->Quaternion,b.Quaternion,sizeof(b.Quaternion));
            return *this;
        }
    };


    class LidarVector{
    public:
        LidarVector(){

        }

        void clear(){ angle_min = 0.0f;
                      angle_max = 0.0f;
                      angle_increment = 0.0f;
                      data.clear(); }
        void pushBuf(float buf[], int size){
            if(size <= 0 || size > 512)return;

            for(int i = 0; i< size; i++){
                this->data.push_back(buf[i]);
            }
        }

        void pushVector(std::vector<float> vec){

            for(unsigned int i = 0; i< vec.size(); i++){
                this->data.push_back(vec[i]);
            }
        }

        float angle_min;
        float angle_max;
        float angle_increment;
        MapWin::POSE pose;
        std::vector<float> data;
    };

    class MapVector{
    public:
        MapVector(){

        }

        void clear(){ width = 0;
                      height = 0;
                      resolution = 0.0f;
                      pose.clear();
                      data.clear(); }
        void pushBuf(char buf[], int size){
            if(size <= 0)return;

            for(int i = 0; i< size; i++){
                this->data.push_back(buf[i]);
            }
        }

        void pushVector(std::vector<char> vec){

            for(unsigned int i = 0; i< vec.size(); i++){
                this->data.push_back(vec[i]);
            }
        }

        void pushVector(std::vector<char> vec, int size){

            for(int i = 0; i< size; i++){
                this->data.push_back(vec[i]);
            }
        }

        unsigned int copyToBuf(char *buf, unsigned int start, unsigned int end){
            unsigned int i = 0;
            if(end > data.size()) return 0;
            for( i = start; i< end; i++){
                buf[i-start] = this->data[i];
            }
            return end - start;
        }

        MapVector& operator=(const MapVector right)
        {
            this->width = right.width;
            this->height = right.height;
            this->resolution = right.resolution;
            this->pose = right.pose;
            this->data = right.data;
            return *this;
        }

        int width;
        int height;
        float resolution;
        MapWin::POSE pose;
        std::vector<char> data;
    };

    class PathVector{
    public:
        PathVector(){

        }

        void clear(){ data.clear(); }

        void pushVector(std::vector<QVector2D> vec){

            for(unsigned int i = 0; i< vec.size(); i++){
                this->data.push_back(vec[i]);
            }
        }

        void pushVector(std::vector<QVector2D> vec, int size){

            for(int i = 0; i< size; i++){
                this->data.push_back(vec[i]);
            }
        }
        std::vector<QVector2D> data;
    };

    void globalpose_to_mappose(
            float &x,
            float &y,
            float win_w,
            float win_h,
            float zoom,
            float roll,
            float moveX,
            float moveY);
    float calc_distance(float x, float y);
    void angle_to_Quaternion(float angle, float quaternion[]);
    void Quaternion_to_angle(float &angle, float q[4]);
    void LidarDataToMapCoordinate(float &x,float &y,MapWin::POSE robot_pose,float rang,float angle);

}

namespace MapWin {

    class Map_Model
    {
    public:
        Map_Model();
        ~Map_Model();

        void init(int w_width,int w_hight);

        //OpenGL
        void initGL();
        void drawGL();
        void resizeGL(int w, int h);


        //OpenGL camera
        Camera& GetCamera(){ return m_Camera; }
        void resetCamera(){ m_Camera.reset();m_Camera.SetPos(QVector3D(0.0f,0.0f,10.0f)); }

        //Goal
        void addGoal(MapWin::POSE goalPose);
        void popGoal();
        void popGoalBack();
        bool getGoal(MapWin::POSE &goal);
        int countGoals(){ return Goals.size(); }
        void clearGoals(){ this->Goals.clear(); }

        //Map
        void clearMap();
        void updateMap(char *data, int w, int h, float resolution,ST_POSE pose);
        void updateMap(std::vector<char> data, int w, int h, float resolution,ST_POSE pose);
        void GetMap(MapWin::MapVector &data);
        void SaveMap(QString file_name);
        void LoadMap(QString file_name);

        //Robot Pose
        void updateRobot(Neo_Packet::STATUS_PACKAGE_ACK pose);
        void getRobotPose(MapWin::POSE &pose){
            pose = robotPose_;
        }
        //Lidar
        void updateLidarData(std::vector<float> vec, float angle_min,float angle_max,float angle_increment,ST_POSE pose);
        void clearLidar();

        //Paths
        void updateGlobalPath(std::vector<QVector2D> vec);
        void clearGlobalPath();

    protected:
    private:
        //windows paramer
        Camera m_Camera;
        int win_width,win_hight;

        // members
        bool mouseLeftDown;
        bool mouseRightDown;
        int mouseX;
        int mouseY;
        float cameraAngleX;
        float cameraAngleY;
        float cameraDistance;

        //map grid
        int map_grid_size;
        pthread_mutex_t *mutex_update_ptr = NULL;

        //map
        MapWin::MapVector MapDatas;

        //robot pose
        MapWin::POSE robotPose_;

        //globalpath
        MapWin::PathVector GlobalPaths;

        //Goals
        std::list<MapWin::POSE> Goals;
        MapWin::LidarVector LidarDatas;
    };

}
#endif // MODELGL_H
