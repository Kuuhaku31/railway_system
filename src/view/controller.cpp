
// controller.cpp

#include "controller.h"

#include "text.h"
#include "view.h"

extern "C" {
#include "train_controller.h"
}

#include <cstdio>
#include <cstring>

Controller* Controller::instance = nullptr;
Controller&
Controller::Instance()
{
    if(instance == nullptr) instance = new Controller();
    return *instance;
}

void
Controller::ControlerInit()
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

    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        train_data_buffer[i] = empty_data;
    }

    search_request.id       = 0;
    search_request.query_id = GREATER;
    processing_data         = empty_data;

    request_data();

    logs.push_back(WELOME_TEXT_0);
    logs.push_back(WELOME_TEXT_1);
}

void
Controller::ControllerUpdate()
{
    static View& view = View::Instance();

    // 插入操作
    if(is_insert)
    {
        // 插入数据
        insert_data();

        is_fresh_processing_data = true;
        is_fresh_data            = true;

        add_train_data_log("Insert: "); // 日志
    }

    // 删除操作
    if(is_del)
    {
        fresh_processing_data_from_buffer(); // 刷新处理数据
        delete_data();                       // 删除当前数据

        is_fresh_processing_data = true;
        is_fresh_data            = true;

        add_train_data_log("Delete: "); // 日志
    }

    // 更新操作
    if(is_update)
    {
        update_data(); // 更新当前数据

        is_fresh_processing_data = true; // 刷新处理数据
        is_fresh_data            = true;

        add_train_data_log("Update: "); // 日志
    }

    // 清除操作
    if(is_clear_processing_data)
    {
        clear_processing_data();
    }

    // 取消操作
    if(is_cancel)
    {
        processing_data.id      = 0;
        view.is_show_user_input = false;
    }

    // 清空数据缓存
    if(is_clear_buffer)
    {
        clear_datas_buffer();
    }

    // 请求数据
    if(is_fresh_data)
    {
        request_data();
    }

    // 如果需要刷新编辑数据
    if(is_fresh_processing_data)
    {
        // 从数据缓存中获取数据
        if(fresh_processing_data_from_buffer())
        {
            view.table_to_selected = !view.table_to_selected;
        }
    }

    // 重置操作状态
    is_insert                = false;
    is_del                   = false;
    is_update                = false;
    is_clear_processing_data = false;
    is_cancel                = false;
    is_fresh_processing_data = false;
    is_fresh_data            = false;
    is_clear_buffer          = false;
}

void
Controller::ControllerConsoleAddLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);
    logs.push_back(std::string(buf));
}


void
Controller::ControllerChangePageIdx(uint32_t new_idx)
{
    page_idx = new_idx;

    if(page_idx < 1)
    {
        page_idx = 1;
    }

    is_fresh_data = true;
}

void
Controller::ControllerChangePageItemsCount(int new_count)
{
    page_item_count = new_count;

    if(page_item_count < 1)
    {
        page_item_count = 1;
    }
    else if(page_item_count > BUFFER_SIZE)
    {
        page_item_count = BUFFER_SIZE;
    }

    is_fresh_data = true;
}

bool
Controller::ControllerIsDataInBuffer()
{
    if(!processing_data.id) return false;

    for(int i = 0; i < page_item_count; i++)
    {
        if(train_data_buffer[i].id == processing_data.id)
        {
            return true;
        }
    }

    return false;
}

void
Controller::insert_data()
{
    printf("Controller Insert data...\n");
    printf("ticket price: %d\n", processing_data.ticket_price);
    int res = RailwaySystemInsertTrainData(processing_data);
    printf("%d\n", res);

    is_fresh_processing_data = true;
    is_fresh_data            = true;
}

void
Controller::update_data()
{
    printf("Controller Update data...\n");
    printf("ticket price: %d\n", processing_data.ticket_price);
    RailwaySystemUpdateTrainData(processing_data);

    is_fresh_data = true;
}

void
Controller::delete_data()
{
    printf("Controller Delete data...\n");
    RailwaySystemDelTrainData(processing_data.id);

    is_fresh_data = true;
}

void
Controller::request_data()
{
    printf("Controller Get datas...\n");

    SearchResult res = RailwaySystemSearchTrainData(train_data_buffer, page_item_count, page_idx, &search_request);

    page_count = res.page_count;
    if(page_count == 0) page_count = 1;
    page_item_count_current = res.data_return_count;
}

bool
Controller::fresh_processing_data_from_buffer()
{
    if(!processing_data.id) return false;

    printf("Search data from buffer...\n");
    for(int i = 0; i < page_item_count; i++)
    {
        if(train_data_buffer[i].id == processing_data.id)
        {
            processing_data = train_data_buffer[i];
            return true;
        }
    }

    return false;
}

void
Controller::clear_processing_data()
{
    processing_data = empty_data;
}

void
Controller::clear_datas_buffer()
{
    for(auto& train_data : train_data_buffer)
    {
        train_data = empty_data;
    }
}

void
Controller::add_train_data_log(std::string label)
{
    ControllerConsoleAddLog(
        (label +
            std::to_string(processing_data.id) + " " +
            std::string(processing_data.number) + " " +
            std::string(processing_data.start_station) + "->" +
            std::string(processing_data.arrive_station) + " " +
            date_to_string(uint64_time_to_date(processing_data.start_time)) + " " +
            date_to_string(uint64_time_to_date(processing_data.arrive_time)) + " " +
            std::to_string(processing_data.ticket_remain) + " " +
            std::to_string(processing_data.ticket_price) + "（分） " +
            parse_train_status(processing_data.train_status))
            .c_str());
    // 添加日志
}
