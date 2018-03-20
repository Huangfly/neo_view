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
    socket/CSocket.cpp \
    socket/CTcpSocket.cpp \
    socket/CUdpSocket.cpp \
    socket/CStreamBase.cpp \
    src/main.cpp \
    src/map_main.cpp \
    src/map_menu.cpp \
    src/map_view.cpp \
    src/socketthread.cpp \
    socket/CHostAddr.cpp

HEADERS  += socket/CThreadBase.h \
    socket/CThreadPool.h \
    socket/CSocket.h \
    socket/CTcpSocket.h \
    socket/CUdpSocket.h \
    socket/CStreamBase.h \
    src/map_main.h \
    src/map_menu.h \
    src/map_socket_type.h \
    src/map_view.h \
    src/socketthread.h \
    socket/CHostAddr.h

FORMS    += src/map_view.ui \
    src/map_menu.ui \
    src/map_main.ui

RESOURCES += \
    ui/myres.qrc
