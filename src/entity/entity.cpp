
// entity.cpp

#include "entity.h"

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
    case TrainStatus::NORMAL:
        return "NORMAL";
    case TrainStatus::DELAY:
        return "DELAY";
    case TrainStatus::STOP:
        return "STOP";
    case TrainStatus::OTHER:
        return "OTHER";
    default:
        return "UNKNOWN";
    }
}
