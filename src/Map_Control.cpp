#include "Map_Control.h"
#include <thread>
#include <unistd.h>

static pid_t tid = 0;

static void *thread_proxy_func(void *data)
{
    Map_Control *p_ctl = (Map_Control *)data;
    tid = pthread_self();
    //pthread_detach(tid);
    p_ctl->runThread();
    tid = 0;
    pthread_exit(NULL);
    return NULL;
}

Map_Control::Map_Control()
{
    isLockMove = false;
    isSetGoal = false;
}

Map_Control::~Map_Control()
{
    //if(tid != 0)
        //pthread_join(tid, NULL);
}

void Map_Control::init(Map_View *view, Map_Model *model)
{
    this->view_ = view;
    this->model_ = model;
}

void Map_Control::create()
{
    pthread_t id;
    int ret = pthread_create(&id,NULL,thread_proxy_func,this);
    if(!ret)
    {
        printf("Map_Control succeed!\n");

    }
    else
    {
        printf("Map_Control Fail to Create Thread.\n");
    }
}

void Map_Control::runThread()
{
    while(1){
        sleep(1);
    }
}

void Map_Control::initGL()
{
    this->model_->initGL();
}

void Map_Control::drawGL()
{
    this->model_->drawGL();
}

void Map_Control::resizeGL(int w, int h)
{
    this->model_->resizeGL(w,h);
}

void Map_Control::mouseWheel(QWheelEvent *event)
{
    if(isLockMove)return;
    float rolldistance=event->delta();

    /*if(model_->getRoll() > 1.0f){
        rolldistance = rolldistance/10.0f;
    }*/
    float scale=rolldistance/100.0;
    printf("scale:%f\n",scale);
    this->model_->GetCamera().ZoomIn(scale);
    this->view_->emitUpdateUI();
}

void Map_Control::mousePress(QMouseEvent *event)
{
    mouse_LastPos = event->pos();
    if( isSetGoal && (event->buttons() & Qt::LeftButton) ){
        MapWin::POSE goal;
        MapWin::POSE pre_goal;
        if(model_->countGoals() > 0){
            model_->getGoal(pre_goal);
        }else{
            model_->getRobotPose(pre_goal);
        }
        QVector2D pos = model_->GetCamera().GetPosFromWindows(event->x(),event->y());
        goal.x = pos.x();
        goal.y = pos.y();
        goal.z = 0.0f;
        float angle = atan2f((goal.y - pre_goal.y), (goal.x - pre_goal.x));
        angle_to_Quaternion(angle, goal.Quaternion);
        this->model_->addGoal(goal);
        this->view_->emitUpdateUI();
    }else if( isSetGoal && (event->buttons() & Qt::RightButton) ){
        this->model_->popGoalBack();
        this->view_->emitUpdateUI();
    }
}

void Map_Control::mouseMove(QMouseEvent *event)
{
    float dx = (float)event->x();
    float dy = (float)event->y();
    float nx = (float)mouse_LastPos.x();
    float ny = (float)mouse_LastPos.y();

    //printf("move:%f,%f\n",(dx-nx),(dy-ny));
    if (event->buttons() & Qt::LeftButton && !isLockMove)
    {
        model_->GetCamera().SetPosFromWindows(-(dx-nx),(dy-ny));
    }else if(event->buttons() & Qt::RightButton && !isLockMove){
        model_->GetCamera().SetHorizontalAngle(dx-nx);
    }
    QVector2D pos = model_->GetCamera().GetPosFromWindows(dx,dy);
    view_->showCursorPoseLabel(pos.x(),pos.y());
    view_->emitUpdateUI();

    mouse_LastPos = event->pos();

}

void Map_Control::mouseRelease(QMouseEvent *event)
{

}

void Map_Control::resetView()
{
    model_->resetCamera();
    view_->emitUpdateUI();
}

void Map_Control::updateMap(char *data, int w, int h, float resolution, ST_POSE pose)
{
    model_->updateMap(data,w,h,resolution,pose);
    view_->emitUpdateUI();
}

void Map_Control::updateMap(std::vector<char> data, int w, int h, float resolution, ST_POSE pose)
{
    model_->updateMap(data,w,h,resolution,pose);
    view_->emitUpdateUI();
}

void Map_Control::clearMap()
{
    model_->clearMap();
    model_->clearLidar();
    model_->clearGoals();
    view_->emitUpdateUI();
}

void Map_Control::LoadMap(QString file_name)
{
    model_->LoadMap(file_name);
    view_->emitUpdateUI();
}

void Map_Control::SaveMap(QString file_name)
{
    model_->SaveMap(file_name);
}

void Map_Control::updateRobotPose(STATUS_PACKAGE_ACK pose)
{
    model_->updateRobot(pose);
    view_->emitUpdateUI();
}

bool Map_Control::GetGoal( POSE &goal )
{
    if(model_->countGoals() < 1) return false;
    this->model_->getGoal(goal);
    //this->model_->popGoal();
    return true;
}

bool Map_Control::GetGoal(GOAL_PACKAGE_POP *goal)
{
    if(model_->countGoals() < 1) return false;
    MapWin::POSE mgoal;
    this->model_->getGoal(mgoal);

    mgoal.ToPOSE(goal);

    return true;
}

void Map_Control::updateLidarData(std::vector<float> vec, float angle_min,float angle_max,float angle_increment, ST_POSE pose)
{
    model_->updateLidarData(vec,angle_min,angle_max,angle_increment,pose);
    view_->emitUpdateUI();
}

void Map_Control::updateGlobalPath(std::vector<QVector2D> vec)
{
    model_->updateGlobalPath(vec);
    view_->emitUpdateUI();
}

