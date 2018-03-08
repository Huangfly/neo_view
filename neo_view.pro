#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T19:41:16
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = neo_view
TEMPLATE = app
LIBS += -lGLU

SOURCES += main.cpp\
        map_view.cpp \
    map_menu.cpp \
    map_main.cpp \
    socketthread.cpp

HEADERS  += map_view.h \
    map_menu.h \
    map_main.h \
    map_socket_type.h \
    socketthread.h

FORMS    += map_view.ui \
    map_menu.ui \
    map_main.ui
