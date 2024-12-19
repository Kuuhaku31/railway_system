
// view.h

#pragma once

#include "imgui_setup.h"

#include "date.h"
#include "train.h"

#include <string>
#include <vector>

std::string
date_to_string(const Date& date);

std::string
parse_train_status(TrainStatus status);

ImVec4
parse_train_status_color(TrainStatus status);

bool
InputTime(const char* label, Date* time);

// 单例模式
class View
{
public:
    static View& Instance();

public:
    void ViewInit();
    void ViewQuit();

    void ViewShowWindows();

    void ViewConsoleAddLog(const char* fmt, ...) IM_FMTARGS(2); // 添加日志

private:
    void show_train_datas_window(bool* p_open = nullptr); // 显示车次信息
    void show_user_input_window(bool* p_open = nullptr);  // 获取用户输入的字符串
    void show_console_window(bool* p_open = nullptr);     // 显示日志
    void show_search_window(bool* p_open = nullptr);      // 显示搜索窗口

public: // 窗口布局相关配置
    bool is_show_demo_window   = false;
    bool is_show_train_datas   = true;
    bool is_show_user_input    = true;
    bool is_show_console       = true;
    bool is_show_search_window = false;

    // 背景颜色
    Color clear_color = { 0x33, 0x33, 0x33, 0xff };

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
    ImFont* font_default = nullptr;
    ImFont* font_chinese = nullptr;

public:
    bool console_scroll_to_bottom = false;
    bool table_to_selected        = false;

private:
    std::vector<std::string> logs;

private:
    View()                       = default;
    ~View()                      = default;
    View(const View&)            = delete;
    View& operator=(const View&) = delete;

    static View* instance;
};
