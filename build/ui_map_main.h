/********************************************************************************
** Form generated from reading UI file 'map_main.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAP_MAIN_H
#define UI_MAP_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <map_menu.h>
#include <map_view.h>

QT_BEGIN_NAMESPACE

class Ui_map_main
{
public:
    map_view *_map_view;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    map_menu *_map_menu;
    QPushButton *pushButton_5;

    void setupUi(QWidget *map_main)
    {
        if (map_main->objectName().isEmpty())
            map_main->setObjectName(QStringLiteral("map_main"));
        map_main->resize(800, 900);
        map_main->setStyleSheet(QLatin1String("QWidget{\n"
"background: rgb(66, 66, 66);\n"
"}"));
        _map_view = new map_view(map_main);
        _map_view->setObjectName(QStringLiteral("_map_view"));
        _map_view->setGeometry(QRect(0, 100, 800, 800));
        _map_view->setStyleSheet(QStringLiteral(""));
        pushButton = new QPushButton(map_main);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 120, 100, 30));
        pushButton->setFocusPolicy(Qt::NoFocus);
        pushButton->setStyleSheet(QLatin1String("QPushButton{\n"
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
        pushButton->setCheckable(true);
        pushButton_2 = new QPushButton(map_main);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 120, 100, 30));
        pushButton_2->setFocusPolicy(Qt::NoFocus);
        pushButton_2->setStyleSheet(QLatin1String("QPushButton{\n"
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
        pushButton_2->setCheckable(true);
        pushButton_3 = new QPushButton(map_main);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(240, 120, 100, 30));
        pushButton_3->setFocusPolicy(Qt::NoFocus);
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
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
        pushButton_4 = new QPushButton(map_main);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(350, 120, 100, 30));
        pushButton_4->setFocusPolicy(Qt::NoFocus);
        pushButton_4->setStyleSheet(QLatin1String("QPushButton{\n"
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
        _map_menu = new map_menu(map_main);
        _map_menu->setObjectName(QStringLiteral("_map_menu"));
        _map_menu->setGeometry(QRect(0, 0, 800, 100));
        pushButton_5 = new QPushButton(map_main);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(460, 120, 100, 30));
        pushButton_5->setFocusPolicy(Qt::NoFocus);
        pushButton_5->setStyleSheet(QLatin1String("QPushButton{\n"
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

        retranslateUi(map_main);

        QMetaObject::connectSlotsByName(map_main);
    } // setupUi

    void retranslateUi(QWidget *map_main)
    {
        map_main->setWindowTitle(QApplication::translate("map_main", "NeoSlamWare", 0));
        pushButton->setText(QApplication::translate("map_main", "Action Robot", 0));
        pushButton_2->setText(QApplication::translate("map_main", "Set Goal", 0));
        pushButton_3->setText(QApplication::translate("map_main", "Reset", 0));
        pushButton_4->setText(QApplication::translate("map_main", "Run Goals", 0));
        pushButton_5->setText(QApplication::translate("map_main", "Cancel Move", 0));
    } // retranslateUi

};

namespace Ui {
    class map_main: public Ui_map_main {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAP_MAIN_H
