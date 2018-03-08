#ifndef MAP_SOCKET_TYPE
#define MAP_SOCKET_TYPE

#define PACK_HEARD		0 //
#define PACK_LOGIN		1 //
#define PACK_MAP        2 //
#define PACK_GOAL		3 //
#define PACK_CANCELGOAL	4 //
#define PACK_NODECTL	5 //

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

#endif // MAP_SOCKET_TYPE

