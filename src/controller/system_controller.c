
// system_controller.c

#include "system_controller.h"


#include "train_controller.h"

#include "date.h"
#include "text.h"
#include "window_rect.h"

#include <stdio.h>
#include <string.h>

#define true 1
#define false 0

#define DATA_BUFFER_SIZE 1000

bool system_is_running = true; // 系统是否运行

// 用户操作
bool system_is_insert                = false; // 插入当前编辑的数据
bool system_is_del                   = false; // 删除当前数据
bool system_is_update                = false; // 更新当前数据
bool system_is_clear_processing_data = false; // 清空当前编辑的数据
bool system_is_cancel                = false; // 取消操作

bool system_is_fresh_data            = false; // 是否刷新数据，从数据库中获取数据
bool system_is_fresh_processing_data = false; // 是否刷新处理数据，先添加查询 id ，查询数据库

TrainQuery system_search_request;  // 查询条件（用户编辑）
TrainData  system_processing_data; // 用户正在编辑的数据，特别的，当 id 为 0 时时，表示没有选中任何数据


// 从数据库中查询到的数据参数
uint32_t result_data_count_total   = 0; // 数据库中查询到的数据总数
uint32_t result_data_count_current = 0; // buffer 中实际的数据数量
uint32_t result_page_count         = 0; // 总页数

uint32_t page_idx        = 1;   // 当前页数
uint32_t page_item_count = 100; // 每一页期望显示的数据数量

TrainData train_data_buffer[DATA_BUFFER_SIZE]; // 从数据库中查询到的数据


// 窗口参数
bool view_is_show_train_datas   = true;  // 显示车次信息
bool view_is_show_user_input    = true;  // 获取用户输入窗口
bool view_is_show_console       = true;  // 显示控制台
bool view_is_show_search_window = false; // 显示过滤器

// 窗口布局比例
float view_data_window_height  = 0.65f;
float view_inuput_window_width = 0.65f;

// 窗口位置和大小
WindowRect view_app_window_rect     = { 0 }; // 应用程序窗口
WindowRect view_data_window_rect    = { 0 }; // 车次数据窗口
WindowRect view_input_window_rect   = { 0 }; // 用户输入窗口
WindowRect view_console_window_rect = { 0 }; // 控制台窗口

// 背景颜色
uint8_t view_clear_color[4] = { VIEW_DEFAULT_CLEAR_COLOR };


bool view_table_to_selected = false;

void // 更新窗口布局
update_view_layout()
{
    // 数据窗口位置和大小
    view_data_window_rect.x = 0;
    view_data_window_rect.y = 0;
    view_data_window_rect.w = view_app_window_rect.w;
    view_data_window_rect.h = (view_is_show_user_input || view_is_show_console) ? view_app_window_rect.h * view_data_window_height : view_app_window_rect.h;

    // 用户输入窗口位置和大小
    view_input_window_rect.x = 0;
    view_input_window_rect.y = view_app_window_rect.h * view_data_window_height;
    view_input_window_rect.w = view_is_show_console ? view_app_window_rect.w * view_inuput_window_width : view_app_window_rect.w;
    view_input_window_rect.h = view_app_window_rect.h * (1 - view_data_window_height);

    // 控制台窗口位置和大小
    view_console_window_rect.x = view_is_show_user_input ? view_app_window_rect.w * view_inuput_window_width : 0;
    view_console_window_rect.y = view_app_window_rect.h * view_data_window_height;
    view_console_window_rect.w = view_is_show_user_input ? view_app_window_rect.w * (1 - view_inuput_window_width) : view_app_window_rect.w;
    view_console_window_rect.h = view_app_window_rect.h * (1 - view_data_window_height);
}

void
clear_train_data(TrainData* data)
{
    static const char* STR_UNKNOWN = "Unknown";

    data->id = 0;
    strcpy(data->number, STR_UNKNOWN);
    strcpy(data->start_station, STR_UNKNOWN);
    strcpy(data->arrive_station, STR_UNKNOWN);
    data->start_time    = 0;
    data->arrive_time   = 0;
    data->ticket_remain = 0;
    data->ticket_price  = 0;
    data->train_status  = TRAIN_STATUS_UNKNOWN;
}

void
clear_search_request(TrainQuery* request)
{
    request->id       = 0;
    request->query_id = GREATER;

    request->ticket_remain       = 0;
    request->query_ticket_remain = IGNORE_THIS;

    request->ticket_price       = 0;
    request->query_ticket_price = IGNORE_THIS;

    request->start_time       = 0;
    request->query_start_time = IGNORE_THIS;

    request->arrive_time       = 0;
    request->query_arrive_time = IGNORE_THIS;

    strcpy(request->number, "");
    request->query_number = IGNORE_THIS;

    strcpy(request->start_station, "");
    request->query_start_station = IGNORE_THIS;

    strcpy(request->arrive_station, "");
    request->query_arrive_station = IGNORE_THIS;

    request->train_status       = 0;
    request->query_train_status = IGNORE_THIS;
}

bool
fresh_processing_data_from_buffer()
{
    if(!system_processing_data.id) return false;

    for(int i = 0; i < page_item_count; i++)
    {
        if(train_data_buffer[i].id == system_processing_data.id)
        {
            system_processing_data = train_data_buffer[i];
            return true;
        }
    }

    return false;
}

void
insert_data()
{
    static const char* STR_SUCCESS = "Insert Success: ";
    static const char* STR_FAILED  = "Insert Failed: ";

    int32_t res = RailwaySystemInsertTrainData(system_processing_data);

    SystemControllerAddLogForTrain(true, res >= 0 ? STR_SUCCESS : STR_FAILED, &system_processing_data);

    system_is_fresh_data = true;
}

void
update_data()
{
    static const char* STR_SUCCESS = "Update Success: ";
    static const char* STR_FAILED  = "Update Failed: ";

    int32_t res = RailwaySystemUpdateTrainData(system_processing_data);

    SystemControllerAddLogForTrain(true, res >= 0 ? STR_SUCCESS : STR_FAILED, &system_processing_data);

    system_is_fresh_data = true;
}

void
delete_data()
{
    static const char* STR_SUCCESS = "Delete Success: ";
    static const char* STR_FAILED  = "Delete Failed: ";

    fresh_processing_data_from_buffer();

    int32_t res = RailwaySystemDelTrainData(system_processing_data.id);

    SystemControllerAddLogForTrain(true, res >= 0 ? STR_SUCCESS : STR_FAILED, &system_processing_data);

    system_is_fresh_data = true;
}

void
request_data()
{
    SearchResult res = RailwaySystemSearchTrainData(train_data_buffer, page_item_count, page_idx, &system_search_request);

    result_data_count_total   = res.data_total_count;
    result_data_count_current = res.data_return_count;
    result_page_count         = res.page_count;

    // 合法化页数
    if(page_idx < 1)
    {
        page_idx = 1;
    }
    else if(page_idx > result_page_count)
    {
        page_idx = result_page_count;
    }
}

void
SystemControlerInit()
{
    // 清空数据
    for(int i = 0; i < DATA_BUFFER_SIZE; i++)
    {
        clear_train_data(&train_data_buffer[i]);
    }

    // 清空编辑数据
    clear_train_data(&system_processing_data);

    // 清空查询条件
    clear_search_request(&system_search_request);

    // 请求数据
    request_data();

    SystemControllerAddLog(false, WELOME_TEXT_0);
    SystemControllerAddLog(false, WELOME_TEXT_1);
    SystemControllerAddLog(true, "System Init Success");
}

void
SystemControllerUpdate()
{
    // 插入操作
    if(system_is_insert)
    {
        // 插入数据
        insert_data();

        system_is_fresh_processing_data = true;
        system_is_fresh_data            = true;
    }

    // 删除操作
    if(system_is_del)
    {
        fresh_processing_data_from_buffer(); // 刷新处理数据
        delete_data();                       // 删除当前数据

        system_is_fresh_processing_data = true;
        system_is_fresh_data            = true;
    }

    // 更新操作
    if(system_is_update)
    {
        update_data(); // 更新当前数据

        system_is_fresh_processing_data = true; // 刷新处理数据
        system_is_fresh_data            = true;
    }

    // 清除操作
    if(system_is_clear_processing_data)
    {
        clear_train_data(&system_processing_data);
    }

    // 取消操作
    if(system_is_cancel)
    {
        system_processing_data.id = 0;
        view_is_show_user_input   = false;
    }

    // 请求数据
    if(system_is_fresh_data)
    {
        request_data();
    }

    // 如果需要刷新编辑数据
    if(system_is_fresh_processing_data)
    {
        // 从数据缓存中获取数据
        if(fresh_processing_data_from_buffer())
        {
            view_table_to_selected = !view_table_to_selected;
        }
    }

    // 重置操作状态
    system_is_insert                = false;
    system_is_del                   = false;
    system_is_update                = false;
    system_is_clear_processing_data = false;
    system_is_cancel                = false;
    system_is_fresh_processing_data = false;
    system_is_fresh_data            = false;

    // 更新窗口布局
    update_view_layout();
}

void
SystemControllerChangePageIdx(uint32_t new_idx)
{
    page_idx = new_idx;

    // 合法化页数
    if(page_idx < 1)
    {
        page_idx = 1;
    }
    else if(page_idx > result_page_count)
    {
        page_idx = result_page_count;
    }

    system_is_fresh_data = true;
}

void
SystemControllerChangePageItemsCount(int new_count)
{
    page_item_count = new_count;

    if(page_item_count < 1)
    {
        page_item_count = 1;
    }
    else if(page_item_count > DATA_BUFFER_SIZE)
    {
        page_item_count = DATA_BUFFER_SIZE;
    }

    system_is_fresh_data = true;
}

bool
SystemControllerIsDataInBuffer()
{
    if(!system_processing_data.id) return false;

    for(int i = 0; i < page_item_count; i++)
    {
        if(train_data_buffer[i].id == system_processing_data.id)
        {
            return true;
        }
    }

    return false;
}

uint32_t // 获取数据总数
SystemControllerGetDataCountTotal()
{
    return result_data_count_total;
}

uint32_t // 获取buffer中实际的数据数量
SystemControllerGetPageItemCountCurrent()
{
    return result_data_count_current;
}

uint32_t // 获取总页数
SystemControllerGetPageCount()
{
    return result_page_count;
}

uint32_t // 获取当前页数
SystemControllerGetPageIdx()
{
    return page_idx;
}

uint32_t // 获取每页显示的数据数量
SystemControllerGetPageItemCount()
{
    return page_item_count;
}


const TrainData* // 获取数据缓存
SystemControllerGetTrainDatas()
{
    return train_data_buffer;
}

#undef true
#undef false

// Kuuhaku-kazari
// 2024.12.21
