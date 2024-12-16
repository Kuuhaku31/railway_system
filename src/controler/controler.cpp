
// controler.cpp

#include "controler.h"

#include <algorithm>

Controler* Controler::instance = nullptr;
Controler&
Controler::Instance()
{
    if(instance == nullptr) instance = new Controler();
    return *instance;
}

void
Controler::ControlerInit()
{
    database.resize(100);
    database[0]  = { 1, "G1234", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[1]  = { 2, "G1235", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[2]  = { 3, "G1236", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[3]  = { 4, "G1237", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[4]  = { 5, "G1238", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[5]  = { 6, "G1239", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[6]  = { 7, "G1240", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[7]  = { 8, "G1241", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[8]  = { 9, "G1242", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[9]  = { 10, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[10] = { 11, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[11] = { 12, "G1235", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[12] = { 13, "G1236", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[13] = { 14, "G1237", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[14] = { 15, "G1238", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[15] = { 16, "G1239", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[16] = { 17, "G1240", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[17] = { 18, "G1241", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[18] = { 19, "G1242", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[19] = { 20, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[20] = { 21, "G1234", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[21] = { 22, "G1235", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[22] = { 23, "G1236", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[23] = { 24, "G1237", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[24] = { 25, "G1238", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[25] = { 26, "G1239", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[26] = { 27, "G1240", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[27] = { 28, "G1241", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[28] = { 29, "G1242", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[29] = { 30, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[30] = { 31, "G1234", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[31] = { 32, "G1235", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[32] = { 33, "G1236", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[33] = { 34, "G1237", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[34] = { 35, "G1238", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[35] = { 36, "G1239", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[36] = { 37, "G1240", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[37] = { 38, "G1241", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[38] = { 39, "G1242", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[39] = { 40, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[40] = { 41, "G1234", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[41] = { 42, "G1235", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[42] = { 43, "G1236", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[43] = { 44, "G1237", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[44] = { 45, "G1238", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[45] = { 46, "G1239", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[46] = { 47, "G1240", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[47] = { 48, "G1241", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[48] = { 49, "G1242", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[49] = { 50, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    database[50] = { 51, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
}

int32_t
Controler::RailwaySystemSearchTrainData(int32_t flag)
{
    train_datas.clear();
    if(flag < 0)
    {
        flag = database.size();
    }
    else if(flag > database.size())
    {
        flag = database.size();
    }

    for(int32_t i = 0; i < flag; ++i)
    {
        train_datas.push_back(&database[i]);
    }

    // 排序
    std::sort(train_datas.begin(), train_datas.end(), [](const TrainData* a, const TrainData* b) { return a->train_id < b->train_id; });

    return 0;
}

int32_t
Controler::RailwaySystemArrangeTrainData()
{
    std::sort(database.begin(), database.end(), [](const TrainData& a, const TrainData& b) { return a.train_id < b.train_id; });

    // 使用 std::remove 将所有匹配的元素移到末尾，并返回新的末尾迭代器
    auto new_end = std::remove_if(database.begin(), database.end(), [](const TrainData& data) { return data.train_id < 0; });

    // 使用 erase 删除无效数据
    database.erase(new_end, database.end());

    return 0;
}

void
Controler::SelectTrainData(int train_data_id)
{
    for(auto& train_data : train_datas)
    {
        if(train_data->train_id == train_data_id)
        {
            processing_data = *train_data;
            break;
        }
    }
}
