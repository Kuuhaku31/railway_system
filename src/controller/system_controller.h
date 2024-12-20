
// system_controller.h

#pragma once

#include "train.h"

#include <stdint.h>

void // 初始化系统控制器
SystemControlerInit();

void // 更新系统控制器
SystemControllerUpdate();

void // 添加日志
SystemControllerAddLog(bool is_add_time, const char* fmt, ...);

void // 添加车次日志
SystemControllerAddLogForTrain(bool is_add_time, const char* label, const TrainData* data);

void // 导出日志
SystemControllerExportLogs();

void // 设置数据缓冲区参数
SystemControllerChangePageIdx(uint32_t new_idx);

void // 设置数据缓冲区参数
SystemControllerChangePageItemsCount(int new_count);

bool // 判断数据是否在缓冲区中
SystemControllerIsDataInBuffer();

uint32_t // 获取当前页数
SystemControllerGetPageIdx();

uint32_t // 获取当前页数据数量
SystemControllerGetPageItemCountCurrent();

uint32_t // 获取总页数
SystemControllerGetPageItemCount();

uint32_t // 获取每页数据数量
SystemControllerGetPageCount();

TrainData* // 获取车次数据
SystemControllerGetTrainDatas();
