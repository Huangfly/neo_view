#ifndef MAP_MENU_H
#define MAP_MENU_H

#include <QTabWidget>
#include "map_view.h"
#include "map_main.h"

namespace Ui {
class map_menu;
}

class map_menu : public QTabWidget
{
    Q_OBJECT
public slots:
    void OnPatrol();
    void OnClearGoals();


    void OnEditChange();

    void OnSlam_SaveMap();
    void OnSlam_ClearMap();

public:
    explicit map_menu(QWidget *parent = 0);
    ~map_menu();
    void init(map_main *p, map_view *pp);
    bool isPatrol;

private:
    Ui::map_menu *ui;
    map_main *m_map_main_ctl;
    map_view *m_map_view_ctl;
};

#endif // MAP_MENU_H
