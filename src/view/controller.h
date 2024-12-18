
// controller.h

#pragma once

#include "train.h"

#include <string>
#include <vector>

// 单例
class Controller
{
public:
    typedef std::vector<TrainData> TrainDatas;

public:
    static Controller& Instance();

public:
    void ControlerInit();    // 初始化
    void ControllerUpdate(); // 控制器更新逻辑

public:
    // 用户操作
    bool is_insert                = false; // 插入当前编辑的数据
    bool is_del                   = false; // 删除当前数据
    bool is_update                = false; // 更新当前数据
    bool is_clear_processing_data = false; // 清空当前编辑的数据
    bool is_cancel                = false; // 取消操作

    bool is_fresh_processing_data = false; // 是否刷新处理数据
    bool is_fresh_data            = false; // 是否刷新数据，从数据库中获取数据
    bool is_clear_buffer          = false; // 是否清空数据缓存

    TrainQuery search_request;  // 查询条件
    TrainData  processing_data; // 用户正在编辑的数据，特别的，当 id 为 0 时时，表示没有选中任何数据

public:
    void ControllerChangePageIdx(uint32_t new_idx);                // 改变页码
    void ControllerChangePageItemsCount(uint32_t page_item_count); // 改变每页显示的数据数量

    const TrainDatas& GetTrainDatas() const { return train_data_buffer; } // 获取数据缓存

    uint32_t ControllerGetPageIdx() const { return page_idx; }                       // 获取当前页数
    uint32_t ControllerGetPageItemCount() const { return train_data_buffer.size(); } // 获取每页显示的数据数量
    uint32_t ControllerGetPageCount() const { return page_count; }                   // 获取总页数

    bool ControllerUnableInsert() const { return unable_insert; } // 是否禁用插入按钮
    bool ControllerUnableDel() const { return unable_del; }       // 是否禁用删除按钮
    bool ControllerUnableUpdate() const { return unable_update; } // 是否禁用更新按钮

private:
    // 从数据库中查询到的数据参数，每页显示的数据数量为 train_datas 的大小
    uint32_t   page_count = 0;    // 总页数
    uint32_t   page_idx   = 0;    // 当前页数
    TrainDatas train_data_buffer; // 从数据库中查询到的数据

private:
    // 控制器状态
    bool unable_insert = false;
    bool unable_del    = false;
    bool unable_update = true;

private:
    // 私有函数
    void insert_data();  // 插入数据，插入当前数据
    void update_data();  // 更新数据，更新当前数据
    void delete_data();  // 删除数据，根据当前数据的id删除
    void request_data(); // 向数据库请求

    bool fresh_processing_data(); // 将指定 id 的数据拷贝到 processing_data 中，返回是否找到对应数据
    void clear_processing_data(); // 清空当前数据
    void clear_datas_buffer();    // 清空数据缓存

    void add_train_data_log(std::string label, const TrainData& train_data); // 添加日志

private:
    Controller()                             = default;
    ~Controller()                            = default;
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;

    static Controller* instance;
};
