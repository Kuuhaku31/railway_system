
// controller.cpp

#include "controller.h"

#include "train_controller.h"

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
    train_datas.resize(RailwaySystemGetTrainDataCount());
}

void
Controller::Getdatas()
{
    printf("Get datas...\n");

    train_datas.resize(RailwaySystemGetTrainDataCount());
    RailwaySystemSearchTrainData(train_datas.data(), train_datas.size(), nullptr);
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
    if(is_fresh_data)
    {
        is_fresh_data = false;
        Getdatas();
    }

    if(is_fresh_processing_data)
    {
        is_fresh_processing_data = false;
        UpdateProcessingData();
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
