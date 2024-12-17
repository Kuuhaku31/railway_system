
// date.cpp

#include "date.h"

// 每月天数（非闰年）
static const uint8_t days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// 判断闰年
static int
is_leap_year(uint16_t year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 计算从1970年开始的秒数转换为Date
Date
uint64_time_to_date(uint64_t time)
{
    Date     date = { 0 };
    uint16_t year = 1970;
    uint32_t days, seconds;

    // 1. 计算总天数和剩余秒数
    days    = time / 86400; // 每天86400秒
    seconds = time % 86400;

    // 2. 计算具体年份
    while(1)
    {
        uint16_t days_in_year = is_leap_year(year) ? 366 : 365;
        if(days < days_in_year)
            break;
        days -= days_in_year;
        year++;
    }
    date.year = year;

    // 3. 计算月份
    uint8_t month = 0;
    for(month = 0; month < 12; month++)
    {
        uint8_t days_in_this_month = days_in_month[month];
        if(month == 1 && is_leap_year(year)) // 二月闰年
            days_in_this_month = 29;

        if(days < days_in_this_month)
            break;
        days -= days_in_this_month;
    }
    date.month = month + 1; // 月份从1开始
    date.day   = days + 1;  // 天数从1开始

    // 4. 计算时、分、秒
    date.hour = seconds / 3600;
    seconds %= 3600;
    date.minute = seconds / 60;
    date.second = seconds % 60;

    return date;
}

// 将Date结构体转换为1970年开始的秒数
uint64_t
date_to_uint64_time(Date date)
{
    uint64_t total_seconds = 0;

    // 1. 添加完整的年份秒数
    for(uint16_t year = 1970; year < date.year; year++)
    {
        total_seconds += (is_leap_year(year) ? 366 : 365) * 86400;
    }

    // 2. 添加完整月份的秒数
    for(uint8_t month = 1; month < date.month; month++)
    {
        uint8_t days_in_this_month = days_in_month[month - 1];
        if(month == 2 && is_leap_year(date.year)) // 二月闰年
            days_in_this_month = 29;
        total_seconds += days_in_this_month * 86400;
    }

    // 3. 添加天、时、分、秒
    total_seconds += (date.day - 1) * 86400; // 天从1开始
    total_seconds += date.hour * 3600;
    total_seconds += date.minute * 60;
    total_seconds += date.second;

    return total_seconds;
}
