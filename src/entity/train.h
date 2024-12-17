//
// Created by sy200 on 24-12-16.
//

#ifndef RAILWAYSYSTEM_TRAIN_H
#define RAILWAYSYSTEM_TRAIN_H
#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

enum TrainStatus {
    TRAIN_STATUS_NORMAL = 0,//正常
    TRAIN_STATUS_DELAYED = 1,//停运
    TRAIN_STATUS_STOPPED = 2,//延误
    TRAIN_STATUS_CANCELLED = 3,//取消
    TRAIN_STATUS_UNKNOWN = 4//未知
};

typedef struct {
    int32_t id; // 唯一 ID

    char number[16];         // 车次
    char start_station[64];  // 始发站
    char arrive_station[64]; // 到达站

    int64_t start_time;   // 出发时间
    int64_t arrive_time;  // 到达时间

    int32_t ticket_remain; // 票数
    float ticket_price; // 价格, 分为单位

    enum TrainStatus train_status; // 列车状态
} TrainData;

enum condition{
    IGNORE_THIS=0,
    GREATER=1,
    SMALLER=2,
    NO_GREATER=3,
    NO_SMALLER=4,
    EQUAL=5,
    NOT_EQUAL=6
};
typedef struct {

    int32_t id; // 唯一 ID
    int query_id;

    int32_t ticket_remain; // 票数
    int query_ticket_remain;

    int32_t ticket_price; // 价格, 分为单位
    int query_ticket_price;

    int64_t start_time;// 出发时间
    int query_start_time;

    int64_t arrive_time;  // 到达时间
    int query_arrive_time;

    char number[16];         // 车次
    int query_number;

    char start_station[64];  // 始发站
    int query_start_station;

    char arrive_station[64]; // 到达站
    int query_arrive_station;

    bool is_running; // 是否运行
    int query_is_running;
} TrainQuery;

typedef struct {

    int32_t id; // 唯一 ID
    bool change_id;

    int32_t ticket_remain; // 票数
    bool change_ticket_remain;

    int32_t ticket_price; // 价格, 分为单位
    bool change_ticket_price;

    int64_t start_time;// 出发时间
    bool change_start_time;

    int64_t arrive_time;  // 到达时间
    bool change_arrive_time;

    char *number;         // 车次
    bool change_number;

    char *start_station;  // 始发站
    bool change_start_station;

    char *arrive_station; // 到达站
    bool change_arrive_station;

    bool is_running; // 是否运行
    bool change_is_running;
} TrainChange;
#ifdef __cplusplus
}
#endif
#endif //RAILWAYSYSTEM_TRAIN_H
