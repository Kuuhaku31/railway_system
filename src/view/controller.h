
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
    void ControlerInit();

    void InsertData(); // 插入数据，插入当前数据

    void UpdateData(); // 更新数据，更新当前数据

    void DeleteData(); // 删除数据，根据当前数据的id删除

public:
    void ControllerUpdate(); // 控制器更新

    bool UpdateProcessingData(); // 更新当前数据，根据当前数据的id更新

    void ClearProcessingData(); // 清空当前数据
    void ClearDatasBuffer();    // 清空数据缓存

public:
    // 将指定 id 的数据k拷贝到 processing_data 中
    // 返回是否找到对应数据
    bool SelectTrainData(int train_data_id);

public:
    // 控制器状态
    bool unable_insert = false;
    bool unable_del    = false;
    bool unable_update = true;

    // 用户操作
    bool is_insert = false;
    bool is_del    = false;
    bool is_update = false;
    bool is_clear  = false;
    bool is_cancel = false;

    TrainQuery search_request;

public:
    TrainData processing_data; // 用户正在编辑的数据，特别的，当 id 为 0 时时，表示没有选中任何数据

private:
    bool is_fresh_data            = false; // 是否刷新数据，从数据库中获取数据
    bool is_fresh_processing_data = false; // 是否刷新处理数据
    bool is_clear_buffer          = false; // 是否清空数据缓存
    bool is_clear_processing_data = false; // 是否清空处理数据

public:
    void ControllerChangePageIdx(uint32_t new_idx);                // 控制器改变页码
    void ControllerChangePageItemsCount(uint32_t page_item_count); // 控制器改变每页显示的数据数量

    uint32_t GetPageItemCount() const { return train_datas.size(); } // 获取每页显示的数据数量
    uint32_t GetPageCount() const { return page_count; }             // 获取页数
    uint32_t GetPageIdx() const { return page_idx; }                 // 获取当前页数

    const TrainDatas& GetTrainDatas() const { return train_datas; } // 获取数据缓存

    void ControllerFreshProcessingData() { is_fresh_processing_data = true; }
    void ControllerClearBuffer() { is_clear_buffer = true; }

private:
    // 从数据库中查询到的数据参数，每页显示的数据数量为 train_datas 的大小
    uint32_t   page_count = 0; // 总页数
    uint32_t   page_idx   = 0; // 当前页数
    TrainDatas train_datas;    // 从数据库中查询到的数据

private:
    // 向数据库请求
    void ControllerRequestData();

private:
    void add_train_data_log(std::string label, const TrainData& train_data);

private:
    Controller()                             = default;
    ~Controller()                            = default;
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;

    static Controller* instance;
};
