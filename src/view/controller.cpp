
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

int32_t
Controller::RailwaySystemInsertTrainData()
{
    // for(auto& data : database)
    // {
    //     if(data.train_id == processing_data.train_id)
    //     {
    //         data = processing_data;
    //         return 0;
    //     }
    // }

    // database.push_back(processing_data);

    return 0;
}

int32_t
Controller::RailwaySystemDelTrainData(int32_t train_data_id)
{
    // auto new_end = std::remove_if(database.begin(), database.end(), [train_data_id](const TrainData& data) { return data.train_id == train_data_id; });

    // database.erase(new_end, database.end());

    return 0;
}

int32_t
Controller::RailwaySystemArrangeTrainData()
{
    // std::sort(database.begin(), database.end(), [](const TrainData& a, const TrainData& b) { return a.train_id < b.train_id; });

    // // 使用 std::remove 将所有匹配的元素移到末尾，并返回新的末尾迭代器
    // auto new_end = std::remove_if(database.begin(), database.end(), [](const TrainData& data) { return data.train_id < 0; });

    // // 使用 erase 删除无效数据
    // database.erase(new_end, database.end());

    return 0;
}

bool
Controller::SelectTrainData(int train_data_id)
{
    // for(auto& train_data : train_datas)
    // {
    //     if(train_data->train_id == train_data_id)
    //     {
    //         processing_data = *train_data;
    //         return true;
    //     }
    // }

    return false;
}
