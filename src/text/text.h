
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
