
// controller.cpp

#include "controller.h"

#include "view.h"

#include "train_controller.h"

#include <cstdio>
#include <cstring>

static View& view = View::Instance();

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
    ControllerChangePageItemsCount(100);
    request_data();

    const char unkonw[] = "Unkonw";

    empty_data.id = 0;
    strcpy(empty_data.number, unkonw);
    strcpy(empty_data.start_station, unkonw);
    strcpy(empty_data.arrive_station, unkonw);
    empty_data.start_time    = 0;
    empty_data.arrive_time   = 0;
    empty_data.ticket_remain = 0;
    empty_data.ticket_price  = 0;
    empty_data.train_status  = TRAIN_STATUS_UNKNOWN;
}

void
Controller::ControllerUpdate()
{
    // 插入操作
    if(is_insert && !unable_insert)
    {
        // 插入数据
        insert_data();

        // 改变状态
        unable_insert = true;
        unable_del    = false;
        unable_update = false;

        is_fresh_processing_data = true;
        is_fresh_data            = true;

        add_train_data_log("Insert: "); // 日志
    }

    // 删除操作
    if(is_del && !unable_del)
    {
        fresh_processing_data_from_buffer(); // 刷新处理数据
        delete_data();                       // 删除当前数据

        unable_insert = false;
        unable_del    = true;
        unable_update = true;

        is_fresh_processing_data = true;
        is_fresh_data            = true;

        add_train_data_log("Delete: "); // 日志
    }

    // 更新操作
    if(is_update && !unable_update)
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

        unable_insert = true;
        unable_del    = true;
        unable_update = true;
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

    // 如果需要刷新编辑数据，并且有数据
    if(is_fresh_processing_data && processing_data.id)
    {
        // 设置查询条件
        search_request.id       = processing_data.id;
        search_request.query_id = IGNORE_THIS;

        is_fresh_data = true;
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
        switch(fresh_processing_data_from_buffer())
        {
        case 0: // 未找到对应数据
            unable_insert = false;
            unable_del    = true;
            unable_update = true;
            break;
        case 1: // 找到对应数据
            unable_insert = true;
            unable_del    = false;
            unable_update = false;

            view.table_to_selected = !view.table_to_selected;
            break;
        default:
        case -1: // 未选中任何数据
            unable_insert = true;
            unable_del    = true;
            unable_update = true;
            break;
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
Controller::ControllerChangePageIdx(uint32_t new_idx)
{
    page_idx = new_idx;

    is_fresh_data = true;
}

void
Controller::ControllerChangePageItemsCount(uint32_t page_item_count)
{
    train_data_buffer.clear();
    train_data_buffer.resize(page_item_count);

    is_fresh_data = true;
}

void
Controller::insert_data()
{
    printf("Insert data...\n");
    RailwaySystemInsertTrainData(processing_data);

    is_fresh_data = true;
}

void
Controller::update_data()
{
    printf("Update data...\n");
    RailwaySystemUpdateTrainData(processing_data);

    is_fresh_data = true;
}

void
Controller::delete_data()
{
    printf("Delete data...\n");
    RailwaySystemDelTrainData(processing_data.id);

    is_fresh_data = true;
}

void
Controller::request_data()
{
    printf("Get datas...\n");

    SearchResult res = RailwaySystemSearchTrainData(train_data_buffer.data(), train_data_buffer.size(), page_idx, &search_request);

    page_count = res.page_count;
}

int8_t
Controller::fresh_processing_data_from_buffer()
{
    if(!processing_data.id) return -1;

    printf("Search data from buffer...\n");
    for(auto& train_data : train_data_buffer)
    {
        if(train_data.id == processing_data.id)
        {
            processing_data = train_data;
            return 1;
        }
    }

    return 0;
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
    // 价格保留两位小数
    char buf[256];
    snprintf(buf, sizeof(buf), "%.2f", processing_data.ticket_price);

    std::string log =
        label +
        std::to_string(processing_data.id) + " " +
        std::string(processing_data.number) + " " +
        std::string(processing_data.start_station) + "->" +
        std::string(processing_data.arrive_station) + " " +
        date_to_string(uint64_time_to_date(processing_data.start_time)) + " " +
        date_to_string(uint64_time_to_date(processing_data.arrive_time)) + " " +
        std::to_string(processing_data.ticket_remain) + " " +
        buf + " " +
        parse_train_status(processing_data.train_status);
    view.ViewConsoleAddLog(log.c_str());
}
