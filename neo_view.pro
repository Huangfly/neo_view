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

SOURCES += socket/CThreadBase.cpp \
    socket/CThreadPool.cpp \
    src/main.cpp \
    src/map_main.cpp \
    src/map_menu.cpp \
    src/map_view.cpp \
    src/socketthread.cpp

HEADERS  += socket/CThreadBase.h \
    socket/CThreadPool.h \
    src/map_main.h \
    src/map_menu.h \
    src/map_socket_type.h \
    src/map_view.h \
    src/socketthread.h

FORMS    += ui/map_view.ui \
    ui/map_menu.ui \
    ui/map_main.ui
