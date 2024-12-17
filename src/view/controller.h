
// controller.h

#pragma once

// #include "train.h"

#include <cstdint>
#include <string>
#include <vector>

struct Date
{
    int year  = 0;
    int month = 0;
    int day   = 0;

    int hour   = 0;
    int minute = 0;
    int second = 0;
};

// 枚举车次状态：正常、延期、停运、其他
enum class TrainStatus
{
    NORMAL,
    DELAY,
    STOP,
    OTHER
};

// 车次结构体
struct TrainData
{
    int         train_id = -1; // 唯一 ID
    std::string train_number;  // 车次

    std::string train_start_station;  // 始发站
    std::string train_arrive_station; // 到达站

    Date train_start_time;  // 出发时间
    Date train_arrive_time; // 到达时间

    int   train_ticket_count = 0;    // 票数
    float train_ticket_price = 0.0f; // 价格

    TrainStatus train_status = TrainStatus::OTHER; // 是否有效
};

std::string
date_to_string(const Date& date);

std::string
parse_train_status(TrainStatus status);


// 单例
class Controller
{
public:
    typedef std::vector<TrainData*> TrainDatas;

public:
    static Controller& Instance();

public:
    void ControlerInit();

public:
    // 插入或修改数据
    // 根据 id 值，如果该 id 已存在则修改，否则插入
    int32_t RailwaySystemInsertTrainData();

    // 删除数据，根据传入id
    int32_t RailwaySystemDelTrainData(int32_t train_data_id);

    // 传入查询数量
    // 将结果存储到容器中
    // num 为 -1 时，查询所有数据
    int32_t RailwaySystemSearchTrainData(int32_t num = -1);

    // 整理数据
    // 将数据按照id排序
    // 删除无效数据
    int32_t RailwaySystemArrangeTrainData();

    // 将指定 id 的数据k拷贝到 processing_data 中
    // 返回是否找到对应数据
    bool SelectTrainData(int train_data_id);

public:
    TrainDatas train_datas;     // 从数据库中查询到的数据
    TrainData  processing_data; // 用户正在编辑的数据

private:
    std::vector<TrainData> database;

private:
    Controller()                             = default;
    ~Controller()                            = default;
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;

    static Controller* instance;
};
