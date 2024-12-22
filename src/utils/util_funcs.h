
// util_funcs.h

#pragma once

#include "date.h"
#include "train.h"
#include "window_rect.h"

#include <imgui.h>
#include <string>
#include <vector>

// 将uint64_t类型的时间转换为Date类型
Date
uint64_time_to_date(uint64_t time);

// 将Date类型的时间转换为uint64_t类型
uint64_t
date_to_uint64_time(Date date);

std::string
date_to_string(const Date& date);

std::string
parse_train_status(TrainStatus status);

ImVec4
parse_train_status_color(TrainStatus status);

bool
InputTime(const char* label, Date* time);

uint32_t
float_to_uint32_price(float price);

float
uint32_price_to_float(uint32_t price);

std::string
uint32_price_to_string(uint32_t price);

ImVec2
get_rect_pos(const WindowRect& rect);

ImVec2
get_rect_size(const WindowRect& rect);

// Kuuhaku-kazari
// 2024.12.21
