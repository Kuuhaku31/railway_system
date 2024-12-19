
// system_controller.c

#include "system_controller.h"

#include "train_controller.h"

#include <string.h>

#define true 1
#define false 0

#define DATA_BUFFER_SIZE 1000


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
uint32_t page_count              = 0;   // 总页数
uint32_t page_idx                = 1;   // 当前页数
uint32_t page_item_count         = 100; // 每一页期望显示的数据数量
uint32_t page_item_count_current = 0;   // buffer 中实际的数据数量

TrainData train_data_buffer[DATA_BUFFER_SIZE]; // 从数据库中查询到的数据
TrainData empty_data;                          // 空数据


void
insert_data()
{
    // printf("Controller Insert data...\n");
    // printf("ticket price: %d\n", system_processing_data.ticket_price);
    int res = RailwaySystemInsertTrainData(system_processing_data);
    // printf("%d\n", res);

    system_is_fresh_processing_data = true;
    system_is_fresh_data            = true;
}

void
update_data()
{
    // printf("Controller Update data...\n");
    // printf("ticket price: %d\n", system_processing_data.ticket_price);
    RailwaySystemUpdateTrainData(system_processing_data);

    system_is_fresh_data = true;
}

void
delete_data()
{
    // printf("Controller Delete data...\n");
    RailwaySystemDelTrainData(system_processing_data.id);

    system_is_fresh_data = true;
}

void
request_data()
{
    // printf("Controller Get datas...\n");

    SearchResult res = RailwaySystemSearchTrainData(train_data_buffer, page_item_count, page_idx, &system_search_request);

    page_count = res.page_count;
    if(page_count == 0) page_count = 1;
    page_item_count_current = res.data_return_count;
}

bool
fresh_processing_data_from_buffer()
{
    if(!system_processing_data.id) return false;

    // printf("Search data from buffer...\n");
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
clear_processing_data()
{
    system_processing_data = empty_data;
}

void
SystemControlerInit()
{
    const char unkonw[] = "Unkonw";

    // 初始化空数据结构
    empty_data.id = 0;
    strcpy(empty_data.number, unkonw);
    strcpy(empty_data.start_station, unkonw);
    strcpy(empty_data.arrive_station, unkonw);
    empty_data.start_time    = 0;
    empty_data.arrive_time   = 0;
    empty_data.ticket_remain = 0;
    empty_data.ticket_price  = 0;
    empty_data.train_status  = TRAIN_STATUS_UNKNOWN;

    for(int i = 0; i < DATA_BUFFER_SIZE; i++)
    {
        train_data_buffer[i] = empty_data;
    }

    system_search_request.id       = 0;
    system_search_request.query_id = GREATER;
    system_processing_data         = empty_data;

    request_data();

    // logs.push_back(WELOME_TEXT_0);
    // logs.push_back(WELOME_TEXT_1);
    // ControllerAddLog("System Init Success");
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

        // add_train_data_log("Insert: "); // 日志
    }

    // 删除操作
    if(system_is_del)
    {
        fresh_processing_data_from_buffer(); // 刷新处理数据
        delete_data();                       // 删除当前数据

        system_is_fresh_processing_data = true;
        system_is_fresh_data            = true;

        // add_train_data_log("Delete: "); // 日志
    }

    // 更新操作
    if(system_is_update)
    {
        update_data(); // 更新当前数据

        system_is_fresh_processing_data = true; // 刷新处理数据
        system_is_fresh_data            = true;

        // add_train_data_log("Update: "); // 日志
    }

    // 清除操作
    if(system_is_clear_processing_data)
    {
        clear_processing_data();
    }

    // 取消操作
    if(system_is_cancel)
    {
        system_processing_data.id = 0;
        // *View_is_show_user_input() = false;
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
            // *View_table_to_selected() = !*View_table_to_selected();
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
}

void
SystemControllerChangePageIdx(uint32_t new_idx)
{
    page_idx = new_idx;

    if(page_idx < 1)
    {
        page_idx = 1;
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

uint32_t
SystemControllerGetPageIdx() // 获取当前页数
{
    return page_idx;
}

uint32_t
SystemControllerGetPageItemCountCurrent() // 获取buffer中实际的数据数量
{
    return page_item_count_current;
}

uint32_t
SystemControllerGetPageItemCount() // 获取每页显示的数据数量
{
    return page_item_count;
}

uint32_t
SystemControllerGetPageCount() // 获取总页数
{
    return page_count;
}

TrainData*
SystemControllerGetTrainDatas() // 获取数据缓存
{
    return train_data_buffer;
}
