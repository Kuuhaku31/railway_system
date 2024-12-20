
// console.cpp

extern "C" {
#include "system_controller.h"
}

#include "util_funcs.h"

#include <cstdarg>
#include <ctime>
#include <string>
#include <vector>

std::vector<std::string> system_logs; // 日志

#define PATH_LOGS_EXPORT "system_logs.txt"

void
SystemControllerAddLog(bool is_add_time, const char* fmt, ...)
{
    static char time_str[32];

    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);

    if(is_add_time)
    { // 获取当前时间
        time_t now = time(nullptr);
        tm*    t   = localtime(&now);

        strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S] ", t);

        system_logs.push_back(std::string(time_str) + std::string(buf));
    }
    else
    {
        system_logs.push_back(std::string(buf));
    }
}

void
SystemControllerAddLogForTrain(bool is_add_time, const char* label, const TrainData* data)
{
    SystemControllerAddLog(
        true,
        (label +
            std::to_string(data->id) + " " +
            std::string(data->number) + " " +
            std::string(data->start_station) + "->" +
            std::string(data->arrive_station) + " " +
            date_to_string(uint64_time_to_date(data->start_time)) + " " +
            date_to_string(uint64_time_to_date(data->arrive_time)) + " " +
            std::to_string(data->ticket_remain) + " " +
            std::to_string(data->ticket_price) + "（分） " +
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
        SystemControllerAddLog(true, "Export logs failed: %s", PATH_LOGS_EXPORT);
        return;
    }

    for(const auto& log : system_logs)
    {
        fprintf(file, "%s\n", log.c_str());
    }

    fclose(file);
    SystemControllerAddLog(true, "Export logs to %s", PATH_LOGS_EXPORT);
}
