
// controller.cpp

#include "controller.h"

#include "train_controller.h"

#include <algorithm>
#include <chrono>

std::string
date_to_string(const Date& date)
{
    std::stringstream ss;
    ss << date.year << "-"
       << std::setw(2) << std::setfill('0') << date.month << "-"
       << std::setw(2) << std::setfill('0') << date.day << " "
       << std::setw(2) << std::setfill('0') << date.hour << ":"
       << std::setw(2) << std::setfill('0') << date.minute << ":"
       << std::setw(2) << std::setfill('0') << date.second;
    return ss.str();
}

std::string
parse_train_status(TrainStatus status)
{
    switch(status)
    {
    case TRAIN_STATUS_RUNNING: return "NORMAL";
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
Controller::Getdata()
{
    RailwaySystemSearchTrainData(train_datas.data(), train_datas.size(), nullptr);
}

void
Controller::InsertData()
{
    RailwaySystemInsertTrainData(processing_data);
}

void
Controller::UpdateData()
{
    RailwaySystemUpdateTrainData(processing_data);
}

void
Controller::DeleteData()
{
    RailwaySystemDelTrainData(processing_data.id);
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
