
// controller.cpp

#include "controller.h"

#include "train_controller.h"

#include <algorithm>
#include <chrono>

std::string
date_to_string(const Date& date)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", date.year, date.month, date.day, date.hour, date.minute, date.second);
    return std::string(buf);
}

std::string
parse_train_status(TrainStatus status)
{
    switch(status)
    {
    case TRAIN_STATUS_NORMAL: return "NORMAL";
    case TRAIN_STATUS_DELAYED: return "DELAY";
    case TRAIN_STATUS_STOPPED: return "STOP";
    case TRAIN_STATUS_CANCELLED: return "CANCEL";
    default:
    case TRAIN_STATUS_UNKNOWN: return "UNKNOWN";
    }
}

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
}

void
Controller::UpdateData()
{
    printf("Update data...\n");
    RailwaySystemUpdateTrainData(processing_data);
}

void
Controller::DeleteData()
{
    printf("Delete data...\n");
    RailwaySystemDelTrainData(processing_data.id);
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
