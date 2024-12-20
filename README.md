# railway_system

### 项目信息

。。。。。

## 数据结构（车次）

### 车次信息

| 数据类型 | 字段           | 描述             |
| -------- | -------------- | ---------------- |
| int32_t  | id             | 唯一标识         |
| char[]   | number         | 列车班次         |
| char[]   | start_station  | 列车始发站       |
| char[]   | arrive_station | 列车终点站       |
| time_t   | start_time     | 列车发车时间     |
| time_t   | arrive_time    | 列车到达时间     |
| int32_t  | ticket_remain  | 剩余票数         |
| int32_t  | ticket_price   | 票价（单位：分） |
| int      | train_status   | 列车状态         |

### 列车状态

| 字段      | 描述 |
| --------- | ---- |
| NORMAL    | 正常 |
| DELAYED   | 延误 |
| STOPPED   | 停运 |
| CANCELLED | 取消 |
| UNKNOWN   | 未知 |

## 支持的操作

### 查询

### 添加

### 修改

### 删除

## 程序架构

### 前端

### 后端

### 前后端交互

## 参考资料及第三方库

- imgui
- sdl2
- sqlite
