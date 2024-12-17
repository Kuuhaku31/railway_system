
// date.h

#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} Date;

// 将uint64_t类型的时间转换为Date类型
Date
uint64_time_to_date(uint64_t time);

// 将Date类型的时间转换为uint64_t类型
uint64_t
date_to_uint64_time(const Date& date);
