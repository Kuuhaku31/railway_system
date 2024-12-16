
// train.h

#include <stdint.h>

typedef char* string;

// 车次结构体
struct Train
{
    uint32_t train_id; // 唯一 ID

    uint32_t train_ticket_count; // 票数
    float    train_ticket_price; // 价格
    uint64_t train_start_time;   // 出发时间
    uint64_t train_arrive_time;  // 到达时间

    string train_number;         // 车次
    string train_start_station;  // 始发站
    string train_arrive_station; // 到达站

    bool is_running; // 是否有效
};
