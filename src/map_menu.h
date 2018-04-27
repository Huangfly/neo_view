#ifndef MAP_MENU_H
#define MAP_MENU_H

#include <QTabWidget>
#include "Map_View.h"
#include "map_main.h"

namespace Ui {
class map_menu;
}

class map_menu : public QTabWidget
{
    Q_OBJECT
public slots:
    void OnNav_Mapping();
    void OnNav_NavigationMode();
    void OnNav_ExploreMode();
    void OnNav_Location();
    void OnClearGoals();


    void OnEditChange();

    void OnSlam_SaveMap();
    void OnSlam_LoadMap();
    void OnSlam_ClearMap();

    void OnAction_BtnPress();
    void OnAction_BtnRelease();
    void OnAction_Left();
    void OnAction_Right();
    void OnAction_Forward();
    void OnAction_Back();

public:
    explicit map_menu(QWidget *parent = 0);
    ~map_menu();
    void init(map_main *p, Map_View *pp);
    bool isPatrol;

private:
    Ui::map_menu *ui;
    map_main *m_map_main_ctl;
    Map_View *m_map_view_ctl;
    CMDVEL_PACKAGE_POP cmdvel_body;
    QTimer *m_cmdvel_timer;
};


#endif // MAP_MENU_H
