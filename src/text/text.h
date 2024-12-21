
// text.h

#pragma once

#define APPLICATION_NAME "Railway System"                                                // 应用程序名称
#define TITLE "Railway System Application Made By: Kuuhaku-kazari & Mike-Solar, 2024.12" // 标题
#define WELOME_TEXT_0 "Welcome to Railway System Application"                            // 欢迎文本
#define WELOME_TEXT_1 "Made By: Kuuhaku-kazari & Mike-Solar, 2024.12"                    //

#define PATH_LOGS_EXPORT "system_logs.txt" // 日志导出路径

#define WINDOW_INIT_RECT { 30, 40, 1200, 1450 }

// 组件颜色
#define BUTTON_COLOR_DEFAULT_NORMAL 0.26, 0.59, 0.98, 0.40
#define BUTTON_COLOR_DEFAULT_HOVERED 0.26, 0.59, 0.98, 1.00
#define BUTTON_COLOR_DEFAULT_ACTIVE 0.06, 0.53, 0.98, 1.00

#define BUTTON_COLOR_NORMAL_NORMAL 0.4f, 0.4f, 0.4f, 1.0f
#define BUTTON_COLOR_NORMAL_HOVERED 0.7f, 0.7f, 0.7f, 1.0f
#define BUTTON_COLOR_NORMAL_ACTIVE 0.2f, 0.2f, 0.2f, 1.0f

// 背景颜色
#define VIEW_DEFAULT_CLEAR_COLOR 0x33, 0x33, 0x33, 0xff

#define BUTTON_COLOR_ACTIVE_NORMAL BUTTON_COLOR_DEFAULT_HOVERED
#define BUTTON_COLOR_ACTIVE_HOVERED BUTTON_COLOR_DEFAULT_ACTIVE
#define BUTTON_COLOR_ACTIVE_ACTIVE BUTTON_COLOR_DEFAULT_ACTIVE

// 状态
#define TRAIN_STATUS_NORMAL_TEXT "Normal"
#define TRAIN_STATUS_DELAYED_TEXT "Delayed"
#define TRAIN_STATUS_STOPPED_TEXT "Stopped"
#define TRAIN_STATUS_CANCELLED_TEXT "Cancelled"
#define TRAIN_STATUS_UNKNOWN_TEXT "Unknown"

#define TEXT_SEARCH_WINDOW_FILTERS "Filters" // 过滤器

#define FILTER_ID_TITLE "Filter By ID"
#define FILTER_ID_ITEM "ID"

#define FILTER_NUMBER_TITLE "Filter By Number"
#define FILTER_NUMBER_ITEM "Number"

#define FILTER_START_STATION_TITLE "Filter By Start Station"
#define FILTER_START_STATION_ITEM "Start Station"

#define FILTER_ARRIVE_STATION_TITLE "Filter By Arrive Station"
#define FILTER_ARRIVE_STATION_ITEM "Arrive Station"

#define FILTER_START_TIME_TITLE "Filter By Start Time"
#define FILTER_START_TIME_ITEM "Start Time"

#define FILTER_ARRIVE_TIME_TITLE "Filter By Arrive Time"
#define FILTER_ARRIVE_TIME_ITEM "Arrive Time"

#define FILTER_TICKET_REMAIN_TITLE "Filter By Ticket Remain"
#define FILTER_TICKET_REMAIN_ITEM "Ticket Remain"

#define FILTER_TICKET_PRICE_TITLE "Filter By Ticket Price"
#define FILTER_TICKET_PRICE_ITEM "Ticket Price"

#define FILTER_TRAIN_STATUS_TITLE "Filter By Train Status"
#define FILTER_TRAIN_STATUS_ITEM "Train Status"

// input_window
#define INPUT_WINDOW_TITLE "Input Window"

#define INPUT_WINDOW_LABEL_TRAIN_ID "##Train ID"
#define INPUT_WINDOW_LABEL_TRAIN_NUMBER "##Train Number"
#define INPUT_WINDOW_LABEL_START_STATION "##Start Station"
#define INPUT_WINDOW_LABEL_ARRIVE_STATION "##Arrive Station"
#define INPUT_WINDOW_LABEL_START_TIME "##Start Time"
#define INPUT_WINDOW_LABEL_ARRIVE_TIME "##Arrive Time"
#define INPUT_WINDOW_LABEL_TICKET_REMAIN "##Ticket Remain"
#define INPUT_WINDOW_LABEL_TICKET_PRICE "##Ticket Price"
#define INPUT_WINDOW_LABEL_TRAIN_STATUS "##Train Status"

#define INPUT_WINDOW_TITLE_TRAIN_ID "Train ID"
#define INPUT_WINDOW_TITLE_TRAIN_NUMBER "Train Number"
#define INPUT_WINDOW_TITLE_START_STATION "Start Station"
#define INPUT_WINDOW_TITLE_ARRIVE_STATION "Arrive Station"
#define INPUT_WINDOW_TITLE_START_TIME "Start Time"
#define INPUT_WINDOW_TITLE_ARRIVE_TIME "Arrive Time"
#define INPUT_WINDOW_TITLE_TICKET_REMAIN "Ticket Remain"
#define INPUT_WINDOW_TITLE_TICKET_PRICE "Ticket Price"
#define INPUT_WINDOW_TITLE_TRAIN_STATUS "Train Status"
