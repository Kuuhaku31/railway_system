
// date.cpp

#include "date.h"

// 将uint64_t类型的时间转换为Date类型
Date
uint64_time_to_date(uint64_t time)
{
    Date date;
    date.year   = (time >> 48) & 0xffff;
    date.month  = (time >> 40) & 0xff;
    date.day    = (time >> 32) & 0xff;
    date.hour   = (time >> 24) & 0xff;
    date.minute = (time >> 16) & 0xff;
    date.second = (time >> 8) & 0xff;
    return date;
}

// 将Date类型的时间转换为uint64_t类型
uint64_t
date_to_uint64_time(const Date& date)
{
    return ((uint64_t)date.year << 48) |
           ((uint64_t)date.month << 40) |
           ((uint64_t)date.day << 32) |
           ((uint64_t)date.hour << 24) |
           ((uint64_t)date.minute << 16) |
           ((uint64_t)date.second << 8);
}
