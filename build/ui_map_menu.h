/********************************************************************************
** Form generated from reading UI file 'map_menu.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAP_MENU_H
#define UI_MAP_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_map_menu
{
public:
    QWidget *navigation;
    QPushButton *PatrolCTL;
    QPushButton *clearGoals;
    QPushButton *Nav_Mapping;
    QPushButton *Nav_Navigation;
    QPushButton *Nav_LidarCtl;
    QWidget *slam;
    QPushButton *slam_savemap;
    QPushButton *slam_clearMap;
    QWidget *setting;
    QLineEdit *lineEdit_IP;
    QLineEdit *lineEdit_Port;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QTabWidget *map_menu)
    {
        if (map_menu->objectName().isEmpty())
            map_menu->setObjectName(QStringLiteral("map_menu"));
        map_menu->resize(800, 100);
        map_menu->setFocusPolicy(Qt::NoFocus);
        map_menu->setStyleSheet(QLatin1String("\n"
"\n"
"QTabWidget::pane { \n"
"\n"
"border-top: 2px solid rgb(78, 125, 224);\n"
"background: rgb(66, 66, 66);\n"
"}\n"
"QTabWidget::tab-bar {\n"
"    background-color: rgb(0, 0, 0);\n"
"}\n"
"QTabBar::tab {\n"
"    color:rgb(255, 255, 255);\n"
"    background: rgb(66, 66, 66);\n"
"    min-width: 8ex;\n"
"    padding: 6px;\n"
"}\n"
"\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    border-top: 2px solid rgb(78, 125, 224);\n"
"    color: rgb(78, 125, 224);\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    \n"
"    border-color: rgb(66, 66, 66);\n"
"    border-top: 2px solid rgb(78, 125, 224);\n"
"}"));
        navigation = new QWidget();
        navigation->setObjectName(QStringLiteral("navigation"));
        PatrolCTL = new QPushButton(navigation);
        PatrolCTL->setObjectName(QStringLiteral("PatrolCTL"));
        PatrolCTL->setGeometry(QRect(20, 15, 80, 40));
        PatrolCTL->setFocusPolicy(Qt::NoFocus);
        PatrolCTL->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        PatrolCTL->setCheckable(true);
        clearGoals = new QPushButton(navigation);
        clearGoals->setObjectName(QStringLiteral("clearGoals"));
        clearGoals->setGeometry(QRect(420, 15, 80, 40));
        clearGoals->setFocusPolicy(Qt::NoFocus);
        clearGoals->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        Nav_Mapping = new QPushButton(navigation);
        Nav_Mapping->setObjectName(QStringLiteral("Nav_Mapping"));
        Nav_Mapping->setGeometry(QRect(120, 15, 80, 40));
        Nav_Mapping->setFocusPolicy(Qt::NoFocus);
        Nav_Mapping->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        Nav_Mapping->setCheckable(true);
        Nav_Navigation = new QPushButton(navigation);
        Nav_Navigation->setObjectName(QStringLiteral("Nav_Navigation"));
        Nav_Navigation->setGeometry(QRect(220, 15, 80, 40));
        Nav_Navigation->setFocusPolicy(Qt::NoFocus);
        Nav_Navigation->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        Nav_Navigation->setCheckable(true);
        Nav_LidarCtl = new QPushButton(navigation);
        Nav_LidarCtl->setObjectName(QStringLiteral("Nav_LidarCtl"));
        Nav_LidarCtl->setGeometry(QRect(320, 15, 80, 40));
        Nav_LidarCtl->setFocusPolicy(Qt::NoFocus);
        Nav_LidarCtl->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        Nav_LidarCtl->setCheckable(true);
        map_menu->addTab(navigation, QString());
        slam = new QWidget();
        slam->setObjectName(QStringLiteral("slam"));
        slam_savemap = new QPushButton(slam);
        slam_savemap->setObjectName(QStringLiteral("slam_savemap"));
        slam_savemap->setGeometry(QRect(20, 15, 80, 40));
        slam_savemap->setFocusPolicy(Qt::NoFocus);
        slam_savemap->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        slam_clearMap = new QPushButton(slam);
        slam_clearMap->setObjectName(QStringLiteral("slam_clearMap"));
        slam_clearMap->setGeometry(QRect(120, 15, 80, 40));
        slam_clearMap->setFocusPolicy(Qt::NoFocus);
        slam_clearMap->setStyleSheet(QLatin1String("QPushButton{\n"
"color:rgb(255, 255, 255);\n"
"border-radius:10px;\n"
"background-color:rgb(78, 125, 224);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(28, 59, 124);\n"
"}\n"
"\n"
"QPushButton::checked {\n"
"   background-color: rgb(28, 59, 124);\n"
"}"));
        map_menu->addTab(slam, QString());
        setting = new QWidget();
        setting->setObjectName(QStringLiteral("setting"));
        lineEdit_IP = new QLineEdit(setting);
        lineEdit_IP->setObjectName(QStringLiteral("lineEdit_IP"));
        lineEdit_IP->setGeometry(QRect(150, 5, 161, 25));
        lineEdit_IP->setStyleSheet(QLatin1String("QLineEdit {\n"
"    border: 2px solid rgb(78, 125, 224);  \n"
"    border-radius: 3px; \n"
"    background: white;  \n"
"    selection-background-color: green;\n"
"    font-size: 14px ;  \n"
"}\n"
" \n"
"QLineEdit:hover {\n"
"    border: 1px solid blue; \n"
"}\n"
""));
        lineEdit_Port = new QLineEdit(setting);
        lineEdit_Port->setObjectName(QStringLiteral("lineEdit_Port"));
        lineEdit_Port->setGeometry(QRect(150, 35, 160, 25));
        lineEdit_Port->setStyleSheet(QLatin1String("QLineEdit {\n"
"    border: 2px solid rgb(78, 125, 224);  \n"
"    border-radius: 3px; \n"
"    background: white;  \n"
"    selection-background-color: green;\n"
"    font-size: 14px ;  \n"
"}\n"
" \n"
"QLineEdit:hover {\n"
"    border: 1px solid blue; \n"
"}"));
        lineEdit_Port->setMaxLength(5);
        label = new QLabel(setting);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 0, 70, 30));
        label->setStyleSheet(QStringLiteral("color: rgb(78, 125, 224);"));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(setting);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 60, 67, 31));
        label_3 = new QLabel(setting);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(80, 30, 70, 30));
        label_3->setStyleSheet(QStringLiteral("color: rgb(78, 125, 224);"));
        label_3->setAlignment(Qt::AlignCenter);
        map_menu->addTab(setting, QString());

        retranslateUi(map_menu);

        map_menu->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(map_menu);
    } // setupUi

    void retranslateUi(QTabWidget *map_menu)
    {
        map_menu->setWindowTitle(QApplication::translate("map_menu", "TabWidget", 0));
        PatrolCTL->setText(QApplication::translate("map_menu", "Patrol\n"
"off", 0));
        clearGoals->setText(QApplication::translate("map_menu", "Clear\n"
"Goals", 0));
        Nav_Mapping->setText(QApplication::translate("map_menu", "MAPPING", 0));
        Nav_Navigation->setText(QApplication::translate("map_menu", "NAV", 0));
        Nav_LidarCtl->setText(QApplication::translate("map_menu", "LIDAR", 0));
        map_menu->setTabText(map_menu->indexOf(navigation), QApplication::translate("map_menu", "Navigation", 0));
        slam_savemap->setText(QApplication::translate("map_menu", "Save Map", 0));
        slam_clearMap->setText(QApplication::translate("map_menu", "Clear\n"
"Map", 0));
        map_menu->setTabText(map_menu->indexOf(slam), QApplication::translate("map_menu", "Slam", 0));
        label->setText(QApplication::translate("map_menu", "IP:", 0));
        label_2->setText(QApplication::translate("map_menu", "TextLabel", 0));
        label_3->setText(QApplication::translate("map_menu", "Port:", 0));
        map_menu->setTabText(map_menu->indexOf(setting), QApplication::translate("map_menu", "Setting", 0));
    } // retranslateUi

};

namespace Ui {
    class map_menu: public Ui_map_menu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAP_MENU_H
