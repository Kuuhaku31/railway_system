
// tool.cpp

#include "view.h"

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
    if(time->year < 0)
    {
        time->year = 0;
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

static View& view = View::Instance();

bool*
View_is_show_user_input()
{
    return &view.is_show_user_input;
}

bool*
View_table_to_selected()
{
    return &view.table_to_selected;
}
