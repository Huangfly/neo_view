#ifndef CONTROLGL_H
#define CONTROLGL_H

#include "Map_View.h"
#include "Map_Model.h"

using namespace MapWin;

namespace MapWin {
    class Map_Control
    {
    public:
        Map_Control();
        ~Map_Control();
        void init(Map_View *view, Map_Model *model);
        void create();
        void runThread();

        //OpenGL
        void initGL();
        void drawGL();
        void resizeGL(int w,int h);
        //windows event
        QPoint mouse_LastPos;
        bool isLockMove;
        bool isSetGoal;
        void lockMove(){ isLockMove = true; }
        void unlockMove(){ isLockMove = false; }
        void enableSetGoal(){ lockMove(); isSetGoal = true; }
        void unableSetGoal(){ unlockMove(); isSetGoal = false; }
        void mouseWheel(QWheelEvent *event);
        void mousePress(QMouseEvent *event);
        void mouseMove(QMouseEvent *event);
        void mouseRelease(QMouseEvent *event);
        void resetView();

        //
        void updateMap(char *data,int w,int h, float resolution,ST_POSE pose);
        void updateMap(std::vector<char> data,int w,int h, float resolution,ST_POSE pose);
        void clearMap();
        void LoadMap(QString file_name);
        void SaveMap(QString file_name);

        void updateRobotPose(Neo_Packet::STATUS_PACKAGE_ACK pose);
        bool GetGoal(MapWin::POSE &goal);
        bool GetGoal(Neo_Packet::GOAL_PACKAGE_POP *goal);
        void ClearGoals(){ model_->clearGoals();view_->updateUI(); }

        //
        void updateLidarData(std::vector<float> vec, float angle_min,float angle_max,float angle_increment, ST_POSE pose);

        void updateGlobalPath(std::vector<QVector2D> vec);

        Map_View *view_;
        Map_Model *model_;
    };
}
#endif // CONTROLGL_H
