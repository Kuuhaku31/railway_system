
// system_controller.h

#pragma once

#include "train.h"

#include <stdint.h>

void
SystemControlerInit();

void
SystemControllerUpdate();

void // 添加日志
SystemControllerAddLog(bool is_add_time, const char* fmt, ...);

void // 添加车次日志
SystemControllerAddLogForTrain(bool is_add_time, const char* label, const TrainData* data);

void // 导出日志
SystemControllerExportLogs();

void
SystemControllerChangePageIdx(uint32_t new_idx);

void
SystemControllerChangePageItemsCount(int new_count);

bool
SystemControllerIsDataInBuffer();

uint32_t
SystemControllerGetPageIdx();

uint32_t
SystemControllerGetPageItemCountCurrent();

uint32_t
SystemControllerGetPageItemCount();

uint32_t
SystemControllerGetPageCount();

TrainData*
SystemControllerGetTrainDatas();
