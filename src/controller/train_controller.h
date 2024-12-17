//
// Created by sy200 on 24-12-17.
//

#ifndef RAILWAYSYSTEM_CONTROLLER_H
#define RAILWAYSYSTEM_CONTROLLER_H

#include "train.h"

#include <stdint.h>

enum SearchFlag
{
    SEARCH_TRAIN_DATA_ALL       = 0x00000001, // 查询所有车次信息
    SEARCH_TRAIN_DATA_BY_ID     = 0x00000002, // 根据id查询
    SEARCH_TRAIN_DATA_BY_NUMBER = 0x00000004, // 根据车次关键词查询
    SEARCH_TRAIN_DATA_BY_START  = 0x00000008, // 根据始发站关键词查询
    SEARCH_TRAIN_DATA_BY_ARRIVE = 0x00000010, // 根据到达站关键词查询
};

// 查询要求
typedef struct
{
    uint32_t flag; // 查询标志

    uint32_t* id;       // 一个id数组，查询包含该id的车次信息
    uint32_t  id_count; // 数组大小

    // 查询关键词，查询包含该关键词的车次信息
    char number[16];         // 车次查询关键词
    char start_station[64];  // 始发站查询关键词
    char arrive_station[64]; // 到达站查询关键词

} SearchRequest;

// 以下函数返回值小于0均表示表示执行失败

// 获取数据库中的车次信息数量
int32_t
RailwaySystemGetTrainDataCount();

// 查询车次信息
// 传入数组的首地址，以及数组的大小
// 以及查询要求
// 根据SearchRequest的内容查询数据库中符合要求的车次信息
// 将数据库中符合查询要求的车次信息：
// **按照id升序排列**
// **依次拷贝到train_data数组中**
// **返回拷贝的车次信息数量**
int32_t
RailwaySystemSearchTrainData(TrainData* train_data, uint32_t num, const SearchRequest* search_request);
// 特别的
// 当传入的 search_request 为 nullptr
// 或者 search_request 的 flag 的最低位为 1
// 查询数据库中所有车次信息

#endif // RAILWAYSYSTEM_CONTROLLER_H
