#ifndef MAP_SOCKET_TYPE
#define MAP_SOCKET_TYPE

#include "NeoWare_SDK/TypePacket_CancelGoal.h"
#include "NeoWare_SDK/TypePacket_CmdVel.h"
#include "NeoWare_SDK/TypePacket_GlobalPath.h"
#include "NeoWare_SDK/TypePacket_LidarDatas.h"
#include "NeoWare_SDK/TypePacket_MapDatas.h"
#include "NeoWare_SDK/TypePacket_MapUpload.h"
#include "NeoWare_SDK/TypePacket_RobotStatus.h"
#include "NeoWare_SDK/TypePacket_RosControl.h"
#include "NeoWare_SDK/TypePacket_SendGoal.h"

#define LOADMAP_PACKAGE_SIZE    512
#define PATH_MAX_SIZE 50

namespace Neo_Packet {

    enum PacketType {
        ROBOTSTATUS = 0,
        MAPDATAS,
        SENDGOAL,
        CANCELGOAL,
        ROSCONTROL,
        CMDVEL,
        LIDARDATAS,
        MAPUPLOAD,
        GLOBALPLAN,
        FORWARDTHENINTERPOLATE
    };

    typedef struct {
        int size;
        int function_id;
        int device_id;
        int ref;
    } HEAD;
}

typedef struct {
    float x;
    float y;
    float z;
    float Quaternion[4];//xyzw
}ST_POSE;

typedef struct {
    float x;
    float y;
}ST_POINT;


/*
typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
}MAP_PACKAGE_POP;

typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
    float resolution;
    float x;
    float y;
    float z;
    float Quaternion[4];
    unsigned int size;
    unsigned int width;
    unsigned int hight;
    char data[512];
}MAP_PACKAGE_ACK;

typedef struct {
    char isAck;
}STATUS_PACKAGE_POP;

typedef struct {
    float x;
    float y;
    float z;
    float Quaternion[4];
    char updateMap;
    char movebase_status;
    char ref[2];
}STATUS_PACKAGE_ACK;

typedef struct {
    float x;
    float y;
    float z;
    float Quaternion[4];
}GOAL_PACKAGE_POP;

typedef struct {
    char isSuccess;
}GOAL_PACKAGE_ACK;

typedef struct {
    char isAck;
}CANCELGOAL_PACKAGE_POP;

typedef struct {
    char ack;
}CANCELGOAL_PACKAGE_ACK;

typedef struct {
    char enable;//0 enable  1 unable
    char node_name[20];
}NODECTL_PACKAGE_POP;

typedef struct {
    char ack[10];
}NODECTL_PACKAGE_ACK;

typedef struct {
    float x;
    float y;
    float z;
}CMDVEL_PACKAGE_POP;

typedef struct {
    char ack;
}CMDVEL_PACKAGE_ACK;


typedef struct {
    unsigned int serial_num;
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
    float angle_min;
    float angle_max;
    float angle_increment;
    ST_POSE pose;
    unsigned int this_rangs_size;
    float rangs[100];
}LIDAR_PACKAGE_ACK;

typedef struct {
    unsigned int serial_num;
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
}LIDAR_PACKAGE_POP;

typedef struct {
    unsigned int serial_num;
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
    ST_POSE map_pose;
    unsigned int width;
    unsigned int height;
    float resolution;
    unsigned int data_size;
    unsigned char data[LOADMAP_PACKAGE_SIZE];
} LOADMAP_PACKAGE_POP;


typedef struct {
    unsigned int serial_num;
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
} LOADMAP_PACKAGE_ACK;



typedef struct {
    unsigned int serial_num;
} GLOBALPATH_PACKAGE_POP;

typedef struct {
    unsigned int serial_num;
    unsigned int path_size;
    ST_POINT path[PATH_MAX_SIZE];
} GLOBALPATH_PACKAGE_ACK;
*/
#endif // MAP_SOCKET_TYPE

