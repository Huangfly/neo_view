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
CONFIG += c++11 console

SOURCES += \
    src/main.cpp \
    src/map_main.cpp \
    src/map_menu.cpp \
    src/Map_Control.cpp \
    src/Map_Model.cpp \
    src/Map_View.cpp \
    socket/TcpTaskRobotStatus.cpp \
    socket/TcpTaskBase.cpp \
    socket/TcpTaskDownloadMap.cpp \
    socket/TcpSocket.cpp \
    socket/TcpTaskCmdVel.cpp \
    src/ManagerSocket.cpp \
    socket/TcpTaskActionNode.cpp \
    socket/TcpTaskGoal.cpp \
    socket/TcpTaskLidar.cpp \
    src/Camera.cpp \
    socket/TcpTaskLoadMap.cpp \
    socket/TcpPacket.cpp \
    socket/TcpTaskGlobalPath.cpp

HEADERS  += src/map_main.h \
    src/map_menu.h \
    src/map_socket_type.h \
    src/Map_Control.h \
    src/Map_Model.h \
    src/Map_View.h \
    socket/TcpTaskRobotStatus.h \
    src/main.h \
    socket/TcpTaskBase.h \
    socket/TcpTaskDownloadMap.h \
    socket/TcpSocket.h \
    socket/TcpTaskCmdVel.h \
    src/ManagerSocket.h \
    socket/TcpTaskActionNode.h \
    socket/TcpTaskGoal.h \
    socket/TcpTaskLidar.h \
    src/Camera.h \
    socket/TcpTaskLoadMap.h \
    socket/TcpPacket.h \
    socket/TcpTaskGlobalPath.h \
    NeoWare_SDK/Type_MutexMap.hpp \
    NeoWare_SDK/TypePacket_CancelGoal.h \
    NeoWare_SDK/TypePacket_CmdVel.h \
    NeoWare_SDK/TypePacket_GlobalPath.h \
    NeoWare_SDK/TypePacket_LidarDatas.h \
    NeoWare_SDK/TypePacket_MapDatas.h \
    NeoWare_SDK/TypePacket_MapUpload.h \
    NeoWare_SDK/TypePacket_RobotStatus.h \
    NeoWare_SDK/TypePacket_RosControl.h \
    NeoWare_SDK/TypePacket_SendGoal.h \
    NeoWare_SDK/Type_Pose.h

FORMS    += \
    src/map_menu.ui \
    src/map_main.ui \
    src/Map_View.ui

RESOURCES += \
    ui/myres.qrc
