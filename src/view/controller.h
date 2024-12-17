
// controller.h

#pragma once

#include "date.h"
#include "train.h"

#include <cstdint>
#include <string>
#include <vector>

std::string
date_to_string(const Date& date);

std::string
parse_train_status(TrainStatus status);

// 单例
class Controller
{
public:
    typedef std::vector<TrainData> TrainDatas;

public:
    static Controller& Instance();

public:
    void ControlerInit();

    void Getdata(); // 获取数据

public:
    // 插入或修改数据
    // 根据 id 值，如果该 id 已存在则修改，否则插入
    int32_t RailwaySystemInsertTrainData();

    // 删除数据，根据传入id
    int32_t RailwaySystemDelTrainData(int32_t train_data_id);

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
    // std::vector<TrainData> database;

private:
    Controller()                             = default;
    ~Controller()                            = default;
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;

    static Controller* instance;
};
