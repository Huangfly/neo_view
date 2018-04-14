#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QFile>
#include <QLabel>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <iostream>
#include <list>
#include <math.h>
#include "map_socket_type.h"


using namespace std ;

namespace Ui {
class Map_View;
}

namespace MapWin {


    class Map_View : public QOpenGLWidget
    {
        Q_OBJECT
    public slots:
        void OnUpdateUI();
    public:
        explicit Map_View(QWidget *parent = 0);
        ~Map_View();

        void showCursorPoseLabel(float x,float y);

        void resetPose();
        void locksetGoal();
        void unlocksetGoal();
        int getGoalsFromList(ST_POSE *pose,char mode);
        void clearFrontGoals();
        void clearGoals();
        void resetNextGoal();
        void clearMap();

        void OnSaveMap();


        void emitUpdateUI();

        list<ST_POSE> m_goals;


    signals:
        void sendGoal();
        void updateUI();
    private:
        Ui::Map_View *ui;
        QWidget *main_view_ctl;
        QLabel *m_robot_info_label;
        QLabel *m_cursor_info_label;

/*
        float view_resolution;

        QPoint m_lastPos;
        float m_xRot,m_yRot,m_zRot;
        float m_xMove,m_yMove,m_zMove,m_roll;
        char *map_data;

        float map_resolution;
        ST_POSE map_pose;
        int map_width;
        int map_hight;
        float map_zoom;

        STATUS_PACKAGE_ACK robotPose;
        float robot_yaw;*/

        bool isLockRot;
        bool isSetPose;
    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);




    };

}
#endif // MAP_VIEW_H
