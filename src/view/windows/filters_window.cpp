
// filters_window.cpp

#include "view.h"

#include "system_controller.h"

#include "text.h"
#include "util_funcs.h"

extern bool       system_is_fresh_data;
extern TrainQuery system_search_request;

bool
select_comparison_operators(const char* label, int* current_item)
{
    static const char* items[] = { ">", "<", "<=", ">=", "=", "!=" };

    int idex = *current_item - 1;

    std::string combo_label = "##combo" + std::string(label);
    ImGui::Combo(combo_label.c_str(), &idex, items, IM_ARRAYSIZE(items));

    bool is_update = (idex + 1) != *current_item;

    *current_item = idex + 1;

    return is_update;
}

void
View::show_search_window(bool* p_open)
{
    static bool is_search_by_id                     = false; // 是否通过id过滤
    static bool is_search_by_number_keyword         = false; // 是否通过车次关键字过滤
    static bool is_search_by_start_station_keyword  = false; // 是否通过始发站关键字过滤
    static bool is_search_by_arrive_station_keyword = false; // 是否通过到达站关键字过滤
    static bool is_search_by_start_time             = false; // 是否通过出发时间过滤
    static bool is_search_by_arrive_time            = false; // 是否通过到达时间过滤
    static bool is_search_by_ticket_remain          = false; // 是否通过票数过滤
    static bool is_search_by_ticket_price           = false; // 是否通过票价过滤
    static bool is_search_by_train_status           = false; // 是否通过列车状态过滤

    if(p_open && !*p_open) return;

    ImGui::PushFont(font_chinese);
    ImGui::Begin(TEXT_SEARCH_WINDOW_FILTERS, p_open);

    system_is_fresh_data |= ImGui::Checkbox(TEXT_SEARCH_WINDOW_FILTERS, &is_use_filter);
    if(is_use_filter)
    {
        ImGui::Indent(10); // 将以下输入框缩进

        // 利用 ID 搜索
        if(ImGui::Checkbox(FILTER_ID_TITLE, &is_search_by_id))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_id)
        {
            static int  operator_idx = 1;
            static bool is_update    = false;

            ImGui::Indent(10);

            // 用户编辑 ID 过滤器
            is_update |= ImGui::InputScalarN(FILTER_ID_ITEM, ImGuiDataType_U32, &system_search_request.id, 1, nullptr, nullptr, "%u");
            is_update |= select_comparison_operators(FILTER_ID_ITEM, &operator_idx); // 下拉选择框
            system_search_request.query_id = operator_idx;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略 ID 搜索
            system_search_request.query_id = IGNORE_THIS;
        }

        // 利用车次关键词搜索
        if(ImGui::Checkbox(FILTER_NUMBER_TITLE, &is_search_by_number_keyword))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_number_keyword)
        {
            static bool is_update = false;

            ImGui::Indent(10);

            // 用户编辑车次关键词过滤器
            is_update |= ImGui::InputText(FILTER_NUMBER_ITEM, system_search_request.number, IM_ARRAYSIZE(system_search_request.number));

            system_search_request.query_number = EQUAL;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略车次关键词搜索
            system_search_request.query_ticket_price = IGNORE_THIS;
        }

        // 利用始发站关键词搜索
        if(ImGui::Checkbox(FILTER_START_STATION_TITLE, &is_search_by_start_station_keyword))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_start_station_keyword)
        {
            static bool is_update = false;

            ImGui::Indent(10);

            // 用户编辑始发站关键词过滤器
            is_update |= ImGui::InputText(FILTER_START_STATION_ITEM, system_search_request.start_station, IM_ARRAYSIZE(system_search_request.start_station));

            system_search_request.query_start_station = EQUAL;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略始发站关键词搜索
            system_search_request.query_start_station = IGNORE_THIS;
        }

        // 利用到达站关键词搜索
        if(ImGui::Checkbox(FILTER_ARRIVE_STATION_TITLE, &is_search_by_arrive_station_keyword))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_arrive_station_keyword)
        {
            static bool is_update = false;

            ImGui::Indent(10);

            // 用户编辑到达站关键词过滤器
            is_update |= ImGui::InputText(FILTER_ARRIVE_STATION_ITEM, system_search_request.arrive_station, IM_ARRAYSIZE(system_search_request.arrive_station));

            system_search_request.query_arrive_station = EQUAL;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略到达站关键词搜索
            system_search_request.query_arrive_station = IGNORE_THIS;
        }

        // 利用出发时间搜索
        if(ImGui::Checkbox(FILTER_START_TIME_TITLE, &is_search_by_start_time))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_start_time)
        {
            static Date search_start_time = { 0 };
            static int  current_item      = 1;
            static bool is_update         = false;

            ImGui::Indent(10);

            // 用户编辑出发时间过滤器
            is_update |= InputTime(FILTER_START_TIME_ITEM, &search_start_time);
            is_update |= select_comparison_operators(FILTER_START_TIME_ITEM, &current_item); // 下拉选择框

            system_search_request.start_time       = date_to_uint64_time(search_start_time);
            system_search_request.query_start_time = current_item;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略出发时间搜索
            system_search_request.query_start_time = IGNORE_THIS;
        }

        // 利用到达时间搜索
        if(ImGui::Checkbox(FILTER_ARRIVE_TIME_TITLE, &is_search_by_arrive_time))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_arrive_time)
        {
            static Date search_arrive_time = { 0 };
            static int  current_item       = 1;
            static bool is_update          = false;

            ImGui::Indent(10);

            is_update |= InputTime(FILTER_ARRIVE_TIME_ITEM, &search_arrive_time);
            is_update |= select_comparison_operators(FILTER_ARRIVE_TIME_ITEM, &current_item); // 下拉选择框

            system_search_request.arrive_time       = date_to_uint64_time(search_arrive_time);
            system_search_request.query_arrive_time = current_item;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略到达时间搜索
            system_search_request.query_arrive_time = IGNORE_THIS;
        }

        // 利用票数搜索
        if(ImGui::Checkbox(FILTER_TICKET_REMAIN_TITLE, &is_search_by_ticket_remain))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_ticket_remain)
        {
            static int  current_item = 1;
            static bool is_update    = false;

            ImGui::Indent(10);

            is_update |= ImGui::InputScalarN(FILTER_TICKET_REMAIN_ITEM, ImGuiDataType_U32, &system_search_request.ticket_remain, 1, nullptr, nullptr, "%u");
            is_update |= select_comparison_operators(FILTER_TICKET_REMAIN_ITEM, &current_item); // 下拉选择框

            system_search_request.query_ticket_remain = current_item;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略票数搜索
            system_search_request.query_ticket_remain = IGNORE_THIS;
        }

        // 利用票价搜索
        if(ImGui::Checkbox(FILTER_TICKET_PRICE_TITLE, &is_search_by_ticket_price))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_ticket_price)
        {
            static float search_ticket_price = 0;
            static int   current_item        = 1;
            static bool  is_update           = false;

            ImGui::Indent(10);

            // 保留两位小数
            is_update |= ImGui::InputScalarN(FILTER_TICKET_PRICE_ITEM, ImGuiDataType_Float, &search_ticket_price, 1, nullptr, nullptr, "%.2f");
            is_update |= select_comparison_operators(FILTER_TICKET_PRICE_ITEM, &current_item); // 下拉选择框

            system_search_request.ticket_price       = float_to_uint32_price(search_ticket_price);
            system_search_request.query_ticket_price = current_item;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略票价搜索
            system_search_request.query_ticket_price = IGNORE_THIS;
        }

        // 利用车次状态搜索
        if(ImGui::Checkbox(FILTER_TRAIN_STATUS_TITLE, &is_search_by_train_status))
        {
            system_is_fresh_data = true;
        }
        if(is_search_by_train_status)
        {
            static const char* items[]   = { "NORMAL", "DELAY", "STOP", "CANCEL", "UNKNOWN" };
            static bool        is_update = false;

            ImGui::Indent(10);

            // 下拉框选择车次状态
            is_update |= ImGui::Combo(FILTER_TRAIN_STATUS_ITEM, (int*)&system_search_request.train_status, items, IM_ARRAYSIZE(items));

            system_search_request.query_is_running = EQUAL;

            // 如果更新了查询条件，调整请求
            if(is_update)
            {
                is_update            = false;
                system_is_fresh_data = true;
            }

            ImGui::Unindent(10);
        }
        else
        {
            // 忽略车次状态搜索
            system_search_request.query_is_running = IGNORE_THIS;
        }
    }
    else
    {
        // 查询所有数据
        system_search_request.id       = 0;
        system_search_request.query_id = GREATER;
    }

    ImGui::End();
    ImGui::PopFont();
}
