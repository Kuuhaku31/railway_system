
// controler.h

#pragma once

#include "entity.h"

#include <cstdint>
#include <vector>

// 单例
class Controler
{
public:
    typedef std::vector<TrainData*> TrainDatas;

public:
    static Controler& Instance();

public:
    void ControlerInit();

public:
    // 插入或修改数据
    // 根据 id 值，如果该 id 已存在则修改，否则插入
    int32_t RailwaySystemInsertTrainData(const TrainData& train_data);

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
    void SelectTrainData(int train_data_id);

public:
    TrainDatas train_datas;     // 从数据库中查询到的数据
    TrainData  processing_data; // 用户正在处理的数据

private:
    std::vector<TrainData> database;

private:
    Controler()                            = default;
    ~Controler()                           = default;
    Controler(const Controler&)            = delete;
    Controler& operator=(const Controler&) = delete;

    static Controler* instance;
};
