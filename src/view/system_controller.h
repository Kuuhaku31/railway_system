
// system_controller.h

#pragma once

#include "train.h"

#include <stdint.h>

void
SystemControlerInit();

void
SystemControllerUpdate();

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
