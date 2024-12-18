
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
}

void
Controller::ControllerUpdate()
{
    // 插入操作
    if(is_insert && !unable_insert)
    {
        insert_data();

        unable_insert = true;
        unable_del    = false;
        unable_update = false;

        is_fresh_data = true;

        add_train_data_log("Insert: ", processing_data); // 日志
    }

    // 删除操作
    if(is_del && !unable_del)
    {
        fresh_processing_data(); // 刷新处理数据
        delete_data();           // 删除当前数据

        unable_insert = false;
        unable_del    = true;
        unable_update = true;

        is_fresh_data = true;

        add_train_data_log("Delete: ", processing_data); // 日志
    }

    // 更新操作
    if(is_update && !unable_update)
    {
        update_data(); // 更新当前数据

        is_fresh_data = true;

        add_train_data_log("Update: ", processing_data); // 日志
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

    // 刷新处理数据
    if(is_fresh_processing_data && processing_data.id)
    {
        // 设置查询条件
        search_request.id       = processing_data.id;
        search_request.query_id = IGNORE_THIS;
        is_fresh_data           = true;

        // 如果选中了新的车次，将该车次的数据显示在输入框中
        if(fresh_processing_data())
        {
            // 如果存在该车次，禁用插入按钮，启用删除和更新按钮
            unable_insert = true;
            unable_del    = false;
            unable_update = false;

            view.table_to_selected = true;
        }
        else
        {
            // 如果不存在该车次，启用插入按钮，禁用删除和更新按钮
            unable_insert = false;
            unable_del    = true;
            unable_update = true;
        }
    }

    // 刷新数据
    if(is_fresh_data)
    {
        request_data();
    }

    // 清空数据缓存
    if(is_clear_buffer)
    {
        clear_datas_buffer();
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

bool
Controller::fresh_processing_data()
{
    printf("Update processing data...\n");
    for(auto& train_data : train_data_buffer)
    {
        if(train_data.id == processing_data.id)
        {
            processing_data = train_data;
            return true;
        }
    }

    return false;
}

void
Controller::clear_processing_data()
{
    static const char unkonw[] = "Unkonw";
    static TrainData  empty_data;
    static bool       is_init = false;

    if(!is_init)
    {
        is_init = true;

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

    processing_data = empty_data;
}

void
Controller::clear_datas_buffer()
{
    train_data_buffer.clear();
}

void
Controller::add_train_data_log(std::string label, const TrainData& train_data)
{
    // 价格保留两位小数
    char buf[256];
    snprintf(buf, sizeof(buf), "%.2f", train_data.ticket_price);

    std::string log =
        label +
        std::to_string(train_data.id) + " " +
        std::string(train_data.number) + " " +
        std::string(train_data.start_station) + "->" +
        std::string(train_data.arrive_station) + " " +
        date_to_string(uint64_time_to_date(train_data.start_time)) + " " +
        date_to_string(uint64_time_to_date(train_data.arrive_time)) + " " +
        std::to_string(train_data.ticket_remain) + " " +
        buf + " " +
        parse_train_status(train_data.train_status);
    view.ViewConsoleAddLog(log.c_str());
}
