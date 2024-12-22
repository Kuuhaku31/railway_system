
// console.cpp

extern "C" {
#include "system_controller.h"
}
#include "view.h"

#include "util_funcs.h"

#include <cstdarg>
#include <ctime>
#include <string>
#include <vector>

static View& view = View::Instance();

std::vector<std::string> system_logs; // 日志

#define PATH_LOGS_EXPORT "system_logs.txt"

void
SystemControllerAddLog(bool is_add_time, const char* log)
{
    static char time_str[32];

    if(is_add_time)
    { // 获取当前时间
        time_t now = time(nullptr);
        tm*    t   = localtime(&now);

        strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S] ", t);

        system_logs.push_back(std::string(time_str) + std::string(log));
    }
    else
    {
        system_logs.push_back(std::string(log));
    }

    view.console_scroll_to_bottom = true;
}

void
SystemControllerAddLogForTrain(bool is_add_time, const char* label, const TrainData* data)
{
    SystemControllerAddLog(
        true,
        (
            std::string(label) +
            std::string(data->number) + " | " +
            std::string(data->start_station) + "->" +
            std::string(data->arrive_station) + " | " +
            date_to_string(uint64_time_to_date(data->start_time)) + " | " +
            date_to_string(uint64_time_to_date(data->arrive_time)) + " | " +
            std::to_string(data->ticket_remain) + " | " +
            uint32_price_to_string(data->ticket_price) + " | " +
            parse_train_status((TrainStatus)data->train_status))
            .c_str());
    // 添加日志
}

void
SystemControllerExportLogs()
{
    // 打开日志文件
    FILE* file = fopen(PATH_LOGS_EXPORT, "w");
    if(file == nullptr)
    {
        SystemControllerAddLog(true, (std::string("Export logs failed: ") + PATH_LOGS_EXPORT).c_str());
        return;
    }

    for(const auto& log : system_logs)
    {
        fprintf(file, "%s\n", log.c_str());
    }

    fclose(file);
    SystemControllerAddLog(true, (std::string("Export logs to ") + PATH_LOGS_EXPORT).c_str());
}
