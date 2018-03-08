/********************************************************************************
** Form generated from reading UI file 'map_view.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAP_VIEW_H
#define UI_MAP_VIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_map_view
{
public:

    void setupUi(QWidget *map_view)
    {
        if (map_view->objectName().isEmpty())
            map_view->setObjectName(QStringLiteral("map_view"));
        map_view->resize(400, 300);

        retranslateUi(map_view);

        QMetaObject::connectSlotsByName(map_view);
    } // setupUi

    void retranslateUi(QWidget *map_view)
    {
        map_view->setWindowTitle(QApplication::translate("map_view", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class map_view: public Ui_map_view {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAP_VIEW_H
