
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
parse_train_status(TrainStatus status)
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
parse_train_status_color(TrainStatus status)
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
    is_update |= ImGui::InputScalar((std::string("年##") + label).c_str(), ImGuiDataType_U16, &time->year), ImGui::SameLine();
    // 输入月
    is_update |= ImGui::InputScalar((std::string("月##") + label).c_str(), ImGuiDataType_U8, &time->month), ImGui::SameLine();
    // 输入日
    is_update |= ImGui::InputScalar((std::string("日##") + label).c_str(), ImGuiDataType_U8, &time->day);
    // 输入小时
    is_update |= ImGui::InputScalar((std::string("时##") + label).c_str(), ImGuiDataType_U8, &time->hour), ImGui::SameLine();
    // 输入分钟
    is_update |= ImGui::InputScalar((std::string("分##") + label).c_str(), ImGuiDataType_U8, &time->minute), ImGui::SameLine();
    // 输入秒
    is_update |= ImGui::InputScalar((std::string("秒##") + label).c_str(), ImGuiDataType_U8, &time->second);
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
