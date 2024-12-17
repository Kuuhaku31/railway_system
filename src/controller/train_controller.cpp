//
// Created by sy200 on 24-12-17.
//

#include "train_controller.h"

#include <algorithm>
#include <cstring>
#include <vector>


#define DATA_COUNT 500
#define START_STATION "北京"
#define ARRIVE_STATION "上海"

static std::vector<TrainData> database;

int32_t
RailwaySystemGetTrainDataCount()
{
    return DATA_COUNT;
}

int32_t
RailwaySystemSearchTrainData(TrainData* train_data, uint32_t num, const SearchRequest* search_request)
{
    static bool is_inited = false;
    if(!is_inited)
    {
        is_inited = true;

        database.resize(DATA_COUNT);

        for(uint32_t i = 0; i < database.size(); i++)
        {
            database[i].id = i;
            strcpy(database[i].number, "G1234");
            strcpy(database[i].start_station, START_STATION);
            strcpy(database[i].arrive_station, ARRIVE_STATION);
            database[i].start_time    = 0;
            database[i].arrive_time   = 0;
            database[i].ticket_remain = 100;
            database[i].ticket_price  = 100.0f;
            database[i].train_status  = TRAIN_STATUS_RUNNING;
        }
    }

    for(uint32_t i = 0; i < num; i++)
    {
        train_data[i] = database[i];
    }

    return 0;
}

int32_t
RailwaySystemDelTrainData(int32_t train_data_id)
{
    auto new_end = std::remove_if(database.begin(), database.end(), [train_data_id](const TrainData& data) { return data.id == train_data_id; });

    database.erase(new_end, database.end());

    return 0;
}

int32_t
RailwaySystemInsertTrainData(TrainData train_data)
{
    // 检测是否存在相同的数据
    for(auto& data : database)
    {
        if(data.id == train_data.id)
        {
            return -1;
        }
    }

    // 插入数据
    database.push_back(train_data);

    // 排序
    std::sort(database.begin(), database.end(), [](const TrainData& a, const TrainData& b) { return a.id < b.id; });

    return 0;
}

int32_t
RailwaySystemUpdateTrainData(TrainData train_data)
{
    // 插入或修改数据
    for(auto& data : database)
    {
        if(data.id == train_data.id)
        {
            data = train_data;
            return 0;
        }
    }
    return -1;
}
