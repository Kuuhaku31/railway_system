
// view.h

#pragma once

#include "imgui_setup.h"

#include "date.h"
#include "text.h"
#include "train.h"

#include <string>
#include <vector>

std::string
date_to_string(const Date& date);

std::string
parse_train_status(int status);

ImVec4
parse_train_status_color(int status);

bool
InputTime(const char* label, Date* time);

uint32_t
float_to_uint32_price(float price);

float
uint32_price_to_float(uint32_t price);

std::string
uint32_price_to_string(uint32_t price);

// 单例模式
class View
{
public:
    static View& Instance();

public:
    void ViewInit();
    void ViewQuit();

    void ViewShowWindows();

private:
    void show_train_datas_window(bool* p_open = nullptr); // 显示车次信息
    void show_user_input_window(bool* p_open = nullptr);  // 获取用户输入的字符串
    void show_console_window(bool* p_open = nullptr);     // 显示日志
    void show_search_window(bool* p_open = nullptr);      // 显示搜索窗口

private:
    // 第一个分割线位置
    int first_separator_pos = 50;

    // 布局比例
    float data_window_height  = 0.65f;
    float inuput_window_width = 0.65f;

    // 车次数据窗口位置和大小
    ImVec2 data_window_pos;
    ImVec2 data_window_size;

    // 用户输入窗口位置和大小
    ImVec2 input_window_pos;
    ImVec2 input_window_size;

    // 控制台窗口位置和大小
    ImVec2 console_window_pos;
    ImVec2 console_window_size;

    void update_view_layout(); // 更新窗口布局

public:
    bool console_scroll_to_bottom = false;

    bool is_use_filter = false; // 是否使用过滤器


public:
    ImFont* font_default = nullptr;
    ImFont* font_chinese = nullptr;

public: // 组件样式
    ImVec2 button_size_02 = { 90, 35 };

    // 设置按钮的正常颜色
    ImVec4 normal_normal_color  = ImVec4(BUTTON_COLOR_NORMAL_NORMAL);  // 正常正常颜色
    ImVec4 normal_hovered_color = ImVec4(BUTTON_COLOR_NORMAL_HOVERED); // 正常悬停颜色
    ImVec4 normal_active_color  = ImVec4(BUTTON_COLOR_NORMAL_ACTIVE);  // 正常按下颜色

    // 设置按钮的活动颜色
    ImVec4 active_normal_color  = ImVec4(BUTTON_COLOR_ACTIVE_NORMAL);  // 活动正常颜色
    ImVec4 active_hovered_color = ImVec4(BUTTON_COLOR_ACTIVE_HOVERED); // 活动悬停颜色
    ImVec4 active_active_color  = ImVec4(BUTTON_COLOR_ACTIVE_ACTIVE);  // 活动按下颜色

    ImVec4 disable_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 禁用颜色

private:
    View()                       = default;
    ~View()                      = default;
    View(const View&)            = delete;
    View& operator=(const View&) = delete;

    static View* instance;
};
