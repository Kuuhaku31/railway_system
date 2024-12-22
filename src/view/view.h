
// view.h

#pragma once

#include "imgui_setup.h"

#include "date.h"
#include "text.h"
#include "train.h"

#include <string>
#include <vector>

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
    void show_filters_window(bool* p_open = nullptr);     // 显示搜索窗口

private:
    // 第一个分割线位置
    int first_separator_pos = 50;

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

// Kuuhaku-kazari
// 2024.12.21
