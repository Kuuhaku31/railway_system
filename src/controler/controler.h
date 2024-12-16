
// controler.h

#pragma once

#include "train_data.h"

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
    // 插入或修改数据
    // 根据 id 值，如果该 id 已存在则修改，否则插入
    int32_t RailwaySystemInsertTrainData(const TrainData& train_data);

    // 删除数据，根据传入id
    int32_t RailwaySystemDelTrainData(int32_t train_data_id);

    // 查询
    // 将结果存储到容器中
    // flag=0时代表全部查询
    int32_t RailwaySystemSearchTrainData(std::vector<TrainData*>& train_data, int32_t flag = 0);

public:
    TrainDatas train_datas;
    TrainData  processing_data; // 用户正在处理的数据

private:
    Controler()                            = default;
    ~Controler()                           = default;
    Controler(const Controler&)            = delete;
    Controler& operator=(const Controler&) = delete;

    static Controler* instance;
};
