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

namespace MapWin {




    class POSE{
    public:
        POSE(){ x = 0.0f;y = 0.0f; z = 0.0f; }
        float x;
        float y;
        float z;
        float Quaternion[4];//xyzw
        void ToPOSE(GOAL_PACKAGE_POP *pop){

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

        void FromPOSE(STATUS_PACKAGE_ACK pop){
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

        int width;
        int height;
        float resolution;
        MapWin::POSE pose;
        std::vector<char> data;
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
        void setMapZoom(float scale);
        float getZoom(){ return map_zoom; }
        void getMoveXY(float &x,float &y){ x = m_xMove; y = m_yMove; }
        void addMoveXY(float x,float y){ m_xMove += x; m_yMove += y; }
        float getRoll(){ return m_roll; }
        void setRoll(float scale){ m_roll += scale; if(m_roll <= 0.1) m_roll = 0.1; }
        void globalpose_to_mappose(float &x, float &y)
        {
            float dx = x;
            float dy = y;

            x = ( (dx-win_width/2.0f)/(win_width/map_zoom/2.0f) - m_xMove )/m_roll;
            y = ( (win_hight/2.0f-dy)/(win_hight/map_zoom/2.0f) - m_yMove )/m_roll;

        }
        void moveMap(float x, float y){ m_xMove += x; m_yMove += y; }

        //Goal
        void addGoal(MapWin::POSE goalPose);
        void popGoal();
        void popGoalBack();
        bool getGoal(MapWin::POSE &goal);
        int countGoals(){ return Goals.size(); }
        void clearGoals(){ this->Goals.clear(); }

        //Map
        void createMapDataPtr(int w, int h);
        void clearMapDataPtr();
        void clearMap();
        void updateMap(char *data, int w, int h, float resolution,ST_POSE pose);
        void updateMap(std::vector<char> data, int w, int h, float resolution,ST_POSE pose);
        void SaveMap(QString file_name);

        //Robot Pose
        void updateRobot(STATUS_PACKAGE_ACK pose);
        void getRobotPose(MapWin::POSE &pose){
            pose.x = robotPose.x;
            pose.y = robotPose.y;
            pose.z = robotPose.z;
            memcpy(pose.Quaternion,robotPose.Quaternion,sizeof(pose.Quaternion));
        }
        //
        void updateLidarData(std::vector<float> vec, float angle_min,float angle_max,float angle_increment,ST_POSE pose);

    protected:
    private:
        //windows paramer
        int win_width,win_hight;
        float map_zoom;
        float m_xRot,m_yRot,m_zRot;
        float m_xMove,m_yMove,m_zMove,m_roll;

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

        //map
        char *map_data;
        MapWin::MapVector MapDatas;
        int map_width;
        int map_hight;
        float map_resolution;
        ST_POSE map_pose;

        //robot pose
        ST_POSE robotPose;
        MapWin::POSE robotPose_;
        float robot_yaw;

        //Goals
        std::list<MapWin::POSE> Goals;
        MapWin::LidarVector LidarDatas;
    };

}
#endif // MODELGL_H
