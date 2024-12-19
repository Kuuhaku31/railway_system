//
// Created by sy200 on 24-12-17.
//

#ifndef RAILWAYSYSTEM_CONTROLLER_H
#define RAILWAYSYSTEM_CONTROLLER_H

#include "train.h"

#include <stdint.h>

// 查询结果
typedef struct
{
    uint32_t data_total_count;  // 查询到的数据总数
    uint32_t data_return_count; // 实际返回的数据数量
    uint32_t page_count;        // 查询到的数据一共有几页
} SearchResult;


// 查询车次信息
// 传入数组的首地址，以及数组的大小
// 以及查询要求
// 根据 search_request 的内容查询数据库中符合要求的车次信息
// 将数据库中符合查询要求的车次信息：
// **按照id升序排列**
// **依次拷贝到train_data数组中**
// **返回拷贝的车次信息数量**
SearchResult
RailwaySystemSearchTrainData(TrainData* train_data, uint32_t num, uint32_t page_idx, const TrainQuery* search_request);


// 以下函数返回值小于0均表示表示执行失败

// 插入数据
// 插入数据到数据库中
// 如果数据库中不存在该车次信息，更新失败，返回负数
int32_t
RailwaySystemInsertTrainData(TrainData train_data);

// 修改数据
// 根据 id 值
int32_t
RailwaySystemUpdateTrainData(TrainData train_data);

// 删除数据，根据传入id
int32_t
RailwaySystemDelTrainData(int32_t train_data_id);

#endif // RAILWAYSYSTEM_CONTROLLER_H
