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

#include "map_main.h"
//#include "map_menu.h"

using namespace std ;

namespace Ui {
class map_view;
}

class map_view : public QOpenGLWidget
{
    Q_OBJECT
public slots:
    void OnUpdateUI();
public:
    explicit map_view(QWidget *parent = 0);
    ~map_view();
    void keyPressEvent(QKeyEvent *e);
    void updateMap(char *data,int w,int h, float resolution,ST_POSE pose);
    void updateRobotPose(STATUS_PACKAGE_ACK pose);
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
    Ui::map_view *ui;
    map_main *main_view_ctl;
    QLabel *m_robot_info_label;
    QLabel *m_cursor_info_label;


    float view_resolution;

    QPoint m_lastPos;
    float m_xRot,m_yRot,m_zRot;
    float m_xMove,m_yMove,m_zMove,m_roll;
    char *map_data;
    GLubyte map_data1[512][512][3];

    float map_resolution;
    ST_POSE map_pose;
    int map_width;
    int map_hight;
    float map_zoom;
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
    void create_MapData(int w, int h);
    void delete_MapData();
    void globalpose_to_mappose(float &x, float &y);


    STATUS_PACKAGE_ACK robotPose;
    float robot_yaw;

};


#endif // MAP_VIEW_H
