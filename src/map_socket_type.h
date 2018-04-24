#ifndef MAP_SOCKET_TYPE
#define MAP_SOCKET_TYPE

#define PACK_HEARD		0 //
#define PACK_LOGIN		1 //
#define PACK_MAP        2 //
#define PACK_GOAL		3 //
#define PACK_CANCELGOAL	4 //
#define PACK_NODECTL	5 //
#define PACK_CMDVEL     8 //
#define PACK_LIDAR      9 //
#define PACK_LOADMAP    10//

#define LOADMAP_PACKAGE_SIZE    512

typedef struct {
    float x;
    float y;
    float z;
    float Quaternion[4];//xyzw
}ST_POSE;


typedef struct packet_head
{
        int size;// header len + data len + sum len( 1 byte )
        int funcId;
        int msg_code;//00 successed   FF fail
        int trans_id;
}P_HEAD;

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


#endif // MAP_SOCKET_TYPE

