
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
    ControllerRequestData();
}

void
Controller::ControllerRequestData()
{
    printf("Get datas...\n");

    RailwaySystemSearchTrainData(train_datas.data(), train_datas.size(), page_idx, &search_request);
}

void
Controller::InsertData()
{
    printf("Insert data...\n");
    RailwaySystemInsertTrainData(processing_data);

    is_fresh_data = true;
}

void
Controller::UpdateData()
{
    printf("Update data...\n");
    RailwaySystemUpdateTrainData(processing_data);

    is_fresh_data = true;
}

void
Controller::DeleteData()
{
    printf("Delete data...\n");
    RailwaySystemDelTrainData(processing_data.id);

    is_fresh_data = true;
}

void
Controller::ControllerUpdate()
{
    if(is_cancel)
    {
        processing_data.id      = 0;
        view.is_show_user_input = false;
    }
    else if(is_insert)
    {
        InsertData();

        unable_insert = true;
        unable_del    = false;
        unable_update = false;

        ControllerFreshProcessingData();

        // 日志
        add_train_data_log("Insert: ", processing_data);
        view.console_scroll_to_bottom = true;
    }
    else if(is_del)
    {
        UpdateProcessingData();
        DeleteData();

        unable_insert = false;
        unable_del    = true;
        unable_update = true;

        ControllerFreshProcessingData();

        // 日志
        add_train_data_log("Delete: ", processing_data);
        view.console_scroll_to_bottom = true;
    }
    else if(is_update)
    {
        UpdateData();

        ControllerFreshProcessingData();

        // 日志
        add_train_data_log("Update: ", processing_data);
        view.console_scroll_to_bottom = true;
    }
    else if(is_clear)
    {
        ClearProcessingData();

        unable_insert = true;
        unable_del    = true;
        unable_update = true;
    }

    is_insert = false;
    is_del    = false;
    is_update = false;
    is_clear  = false;
    is_cancel = false;

    if(is_fresh_data)
    {
        is_fresh_data = false;
        ControllerRequestData();
    }

    if(is_clear_buffer)
    {
        is_clear_buffer = false;
        ClearDatasBuffer();
    }

    if(is_clear_processing_data)
    {
        is_clear_processing_data = false;
        ClearProcessingData();
    }

    if(is_fresh_processing_data) // 如果选中了新的车次
    {
        printf("is_fresh_processing_data\n");

        is_fresh_processing_data = false;

        search_request.id       = processing_data.id;
        search_request.query_id = IGNORE_THIS;
        is_fresh_data           = true;

        // 如果选中了新的车次，将该车次的数据显示在输入框中
        if(UpdateProcessingData())
        {
            // 如果存在该车次，禁用插入按钮，启用删除和更新按钮
            unable_insert = true;
            unable_del    = false;
            unable_update = false;

            // table_to_selected = true;
        }
        else
        {
            // 如果不存在该车次，启用插入按钮，禁用删除和更新按钮
            unable_insert = false;
            unable_del    = true;
            unable_update = true;
        }
    }
    else if(!processing_data.id) // 如果没有选中车次
    {
        // 如果没有选中车次
        unable_insert = true;
        unable_del    = true;
        unable_update = true;
    }
}

bool
Controller::UpdateProcessingData()
{
    printf("Update processing data...\n");
    for(auto& train_data : train_datas)
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
Controller::ClearProcessingData()
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
Controller::ClearDatasBuffer()
{
    train_datas.clear();
}

bool
Controller::SelectTrainData(int train_data_id)
{
    for(auto& train_data : train_datas)
    {
        if(train_data.id == train_data_id)
        {
            processing_data = train_data;
            return true;
        }
    }

    return false;
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
    train_datas.clear();
    train_datas.resize(page_item_count);
    page_count = RailwaySystemGetTrainDataPageCountWithPageItem(page_item_count);

    is_fresh_data = true;
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
