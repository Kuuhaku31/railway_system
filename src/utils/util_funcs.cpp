
// util_funcs.cpp

#include "util_funcs.h"

#include "text.h"
#include "train.h"

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


std::string
date_to_string(const Date& date)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", date.year, date.month, date.day, date.hour, date.minute, date.second);
    return std::string(buf);
}

std::string
parse_train_status(int status)
{
    switch(status)
    {
    case TRAIN_STATUS_NORMAL: return "NORMAL";
    case TRAIN_STATUS_DELAYED: return "DELAY";
    case TRAIN_STATUS_STOPPED: return "STOP";
    case TRAIN_STATUS_CANCELLED: return "CANCEL";
    default:
    case TRAIN_STATUS_UNKNOWN: return "UNKNOWN";
    }
}

ImVec4
parse_train_status_color(int status)
{
    switch(status)
    {
    case TRAIN_STATUS_NORMAL: return ImVec4(0, 255, 0, 255);        // 绿色
    case TRAIN_STATUS_DELAYED: return ImVec4(255, 255, 0, 255);     // 黄色
    case TRAIN_STATUS_STOPPED: return ImVec4(255, 0, 0, 255);       // 红色
    case TRAIN_STATUS_CANCELLED: return ImVec4(128, 128, 128, 255); // 灰色
    default:                                                        // 未知状态
    case TRAIN_STATUS_UNKNOWN: return ImVec4(0, 0, 255, 255);       // 蓝色
    }
}

bool
InputTime(const char* label, Date* time)
{
    bool is_update = false;

    ImGui::Text(label);
    ImGui::PushItemWidth(100);

    // 输入年
    is_update |= ImGui::InputScalar((std::string("年##") + label).c_str(), ImGuiDataType_U16, &time->year);
    if(time->year < 1970)
    {
        time->year = 1970;
    }

    // 输入月
    ImGui::SameLine();
    is_update |= ImGui::InputScalar((std::string("月##") + label).c_str(), ImGuiDataType_U8, &time->month);
    if(time->month < 1)
    {
        time->month = 1;
    }
    else if(time->month > 12)
    {
        time->month = 12;
    }

    // 输入日
    ImGui::SameLine();
    is_update |= ImGui::InputScalar((std::string("日##") + label).c_str(), ImGuiDataType_U8, &time->day);
    if(time->day < 1)
    {
        time->day = 1;
    }
    else if(time->day > 31)
    {
        time->day = 31;
    }

    // 输入小时
    is_update |= ImGui::InputScalar((std::string("时##") + label).c_str(), ImGuiDataType_U8, &time->hour);
    if(time->hour < 0)
    {
        time->hour = 0;
    }
    else if(time->hour > 23)
    {
        time->hour = 23;
    }

    // 输入分钟
    ImGui::SameLine();
    is_update |= ImGui::InputScalar((std::string("分##") + label).c_str(), ImGuiDataType_U8, &time->minute);
    if(time->minute < 0)
    {
        time->minute = 0;
    }
    else if(time->minute > 59)
    {
        time->minute = 59;
    }

    // 输入秒
    ImGui::SameLine();
    is_update |= ImGui::InputScalar((std::string("秒##") + label).c_str(), ImGuiDataType_U8, &time->second);
    if(time->second < 0)
    {
        time->second = 0;
    }
    else if(time->second > 59)
    {
        time->second = 59;
    }

    ImGui::PopItemWidth();

    return is_update;
}

uint32_t
float_to_uint32_price(float price)
{
    return (uint32_t)(price * 100);
}

float
uint32_price_to_float(uint32_t price)
{
    return price / 100.0f;
}

std::string
uint32_price_to_string(uint32_t price)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%u.%02u", price / 100, price % 100);
    return std::string(buf);
}
