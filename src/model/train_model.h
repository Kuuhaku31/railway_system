//
// Created by sy200 on 24-12-17.
//

#ifndef RAILWAYSYSTEM_TRAIN_MODEL_H
#define RAILWAYSYSTEM_TRAIN_MODEL_H
#ifdef __cplusplus
extern "C"{
#endif

#include <sqlite3.h>
#include <stdint.h>
#include "utils.h"
#include "train.h"
#include "error.h"

void analyzeCondition(TrainQuery *condition, char *buffer, int bufferSize);

void analyzeChange(TrainChange *condition, char *buffer, int bufferSize);

// 添加
int addTrain(TrainData *train);

int addTrainList(TrainData *trains, int num);

//修改

int editTrain(TrainQuery *query, TrainChange *change);

//删除

int deleteTrain(TrainQuery *query);
// 查询

/**
 * 通过id获取火车信息
 * @param id [in] 火车ID
 * @param train [out] TrainData的结构体指针，用于填充列车信息
 * @return 错误码，0代表成功
 */
int getTrainById(uint32_t id, TrainData *train);

/**
 * 通过车次获取火车信息
 * @param number [in] 车次
 * @param train [out] TrainData的结构体指针，用于填充列车信息
 * @return 错误码，0代表成功
 */
int getTrainByNumber(char *number, TrainData *train);

/**
 * 查询符合条件的火车
 * @param condition [in] 查询条件。
 * @param train [out] 符合条件的车次列表
 * @param num [out] 符合条件的车次个数
 * @return 错误码
 */
int getTrainList(TrainQuery *condition,
                 TrainData **train,
                 int32_t *num);

#ifdef __cplusplus
};
#endif
#endif //RAILWAYSYSTEM_TRAIN_MODEL_H
