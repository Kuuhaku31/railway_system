# railway_system

Course Design of C Programming Language

## 数据（车次）

- id
- 始发站
- 终点站
- 班次
- 发车时间
- 到达时间
- 票数
- 票价
- 是否停运

## 存储方法

- struct

## 方法

- 查询
- 预定
- 修改
  - 是否停运
- 删除

## 前后端交互

```cpp
// 车次结构体
struct TrainData
{
    int32_t train_id; // 唯一 ID

    int32_t train_ticket_count; // 票数
    float    train_ticket_price; // 价格
    int64_t train_start_time;   // 出发时间
    int64_t train_arrive_time;  // 到达时间

    char* train_number;         // 车次
    char* train_start_station;  // 始发站
    char* train_arrive_station; // 到达站

    bool is_running; // 是否有效
};

// 返回值 0 代表成功，其他失败

// 插入或修改数据
// 根据 id 值，如果该 id 已存在则修改，否则插入
int32_t RailwaySystemInsertTrainData(const TrainData& train_data)

// 删除数据，根据传入id
int32_t RailwaySystemDelTrainData(int32_t train_data_id)

// 查询
// 将结果存储到容器中
// flag=0时代表全部查询
int32_t RailwaySystemSearchTrainData(std::vector<TrainData*>& train_data, int32_t flag = 0)


```

# 前端

# 后端

- test
- lib

* src
  - entity
  - view
  - controler
  - model
  - main.c

- CMakeLists.txt
