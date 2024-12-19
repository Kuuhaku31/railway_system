
// controller.h

#pragma once

#include "train.h"

#include <string>
#include <vector>

#define BUFFER_SIZE 10000

// 单例
class Controller
{
public:
    typedef std::vector<std::string> SystemLogs;

public:
    static Controller& Instance();

public:
    void ControlerInit();    // 初始化
    void ControllerUpdate(); // 控制器更新逻辑

    void ControllerAddLog(const char* fmt, ...); // IM_FMTARGS(2); // 添加日志
    void ControllerExportLogs();                 // 导出日志

public:
    // 用户操作
    bool is_insert                = false; // 插入当前编辑的数据
    bool is_del                   = false; // 删除当前数据
    bool is_update                = false; // 更新当前数据
    bool is_clear_processing_data = false; // 清空当前编辑的数据
    bool is_cancel                = false; // 取消操作
    bool is_clear_buffer          = false; // 是否清空数据缓存

    bool is_fresh_data            = false; // 是否刷新数据，从数据库中获取数据
    bool is_fresh_processing_data = false; // 是否刷新处理数据，先添加查询 id ，查询数据库

    TrainQuery search_request;  // 查询条件（用户编辑）
    TrainData  processing_data; // 用户正在编辑的数据，特别的，当 id 为 0 时时，表示没有选中任何数据

public:
    void ControllerChangePageIdx(uint32_t new_idx);           // 改变页码
    void ControllerChangePageItemsCount(int page_item_count); // 改变每页显示的数据数量

    uint32_t ControllerGetPageIdx() const { return page_idx; }                             // 获取当前页数
    uint32_t ControllerGetPageItemCountCurrent() const { return page_item_count_current; } // 获取buffer中实际的数据数量
    uint32_t ControllerGetPageItemCount() const { return page_item_count; }                // 获取每页显示的数据数量
    uint32_t ControllerGetPageCount() const { return page_count; }                         // 获取总页数

    const TrainData*  ControllerGetTrainDatas() const { return train_data_buffer; } // 获取数据缓存
    const SystemLogs& ControllerGetLogs() const { return logs; }                    // 获取日志

    bool ControllerIsDataInBuffer(); // 在数据缓存中搜索数据

private:
    // 从数据库中查询到的数据参数
    uint32_t  page_count              = 0;    // 总页数
    uint32_t  page_idx                = 1;    // 当前页数
    uint32_t  page_item_count         = 100;  // 每一页期望显示的数据数量
    uint32_t  page_item_count_current = 0;    // buffer 中实际的数据数量
    TrainData train_data_buffer[BUFFER_SIZE]; // 从数据库中查询到的数据

private:
    TrainData empty_data; // 空数据

private:
    SystemLogs logs; // 日志

private:
    // 私有函数
    void insert_data();  // 插入数据，插入当前数据
    void update_data();  // 更新数据，更新当前数据
    void delete_data();  // 删除数据，根据当前数据的id删除
    void request_data(); // 向数据库请求

    bool fresh_processing_data_from_buffer(); // 将指定 id 的数据拷贝到 processing_data 中，返回是否找到对应数据
    void clear_processing_data();             // 清空当前数据
    void clear_datas_buffer();                // 清空数据缓存

    void add_train_data_log(std::string label); // 添加日志

private:
    Controller()                             = default;
    ~Controller()                            = default;
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;

    static Controller* instance;
};
