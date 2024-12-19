
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
    bool is_use_filter                       = false;
    bool is_search_by_id                     = false;
    bool is_search_by_number_keyword         = false;
    bool is_search_by_start_station_keyword  = false;
    bool is_search_by_arrive_station_keyword = false;
    bool is_search_by_start_time             = false;
    bool is_search_by_arrive_time            = false;
    bool is_search_by_ticket_remain          = false;
    bool is_search_by_ticket_price           = false;
    bool is_search_by_train_status           = false;

    uint32_t    search_id            = 0;
    char        search_number[16]    = { 0 };
    char        search_start[64]     = { 0 };
    char        search_arrive[64]    = { 0 };
    Date        search_start_time    = { 0 };
    Date        search_arrive_time   = { 0 };
    uint32_t    search_ticket_remain = 0;
    float       search_ticket_price  = 0;
    TrainStatus search_train_status  = TRAIN_STATUS_NORMAL;

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
