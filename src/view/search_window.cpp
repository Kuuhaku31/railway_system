
// search_window.cpp

#include "view.h"

#include "controller.h"

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

static Controller& controller     = Controller::Instance();
static TrainQuery& search_request = controller.search_request;

void
View::show_search_window(bool* p_open)
{
    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoTitleBar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;
    // window_flags |= ImGuiWindowFlags_NoDocking;
    // window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushFont(font_chinese);
    ImGui::Begin("Search Window", p_open, window_flags);

    ImGui::Text("Search Window...");

    ImGui::Checkbox("Search All", &is_search_all);
    if(is_search_all)
    {
        // 调整请求
        // 将 search_request 中的所有查询条件设置为 0

        search_request.query_id             = 0;
        search_request.query_ticket_remain  = 0;
        search_request.query_ticket_price   = 0;
        search_request.query_start_time     = 0;
        search_request.query_arrive_time    = 0;
        search_request.query_number         = 0;
        search_request.query_start_station  = 0;
        search_request.query_arrive_station = 0;
        search_request.query_is_running     = 0;

        return;
    }


    ImGui::Indent(10); // 将以下输入框缩进

    // 利用 ID 搜索
    ImGui::Checkbox("Search By ID", &is_search_by_id);
    if(is_search_by_id)
    {
        static int current_item = 1;

        ImGui::Indent(10);

        bool is_update = false;
        is_update |= ImGui::InputScalarN("ID", ImGuiDataType_U32, &search_id, 1, nullptr, nullptr, "%u");
        is_update |= select_comparison_operators("ID", &current_item); // 下拉选择框

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            search_request.query_id = search_id;
            search_request.query_id = current_item;
            printf("update search request id: %d, %d\n", search_id, current_item);
        }

        ImGui::Unindent(10);
    }

    // 利用车次关键词搜索
    ImGui::Checkbox("Search By Number", &is_search_by_number_keyword);
    if(is_search_by_number_keyword)
    {
        ImGui::Indent(10);

        bool is_update = ImGui::InputText("Number", search_number, IM_ARRAYSIZE(search_number));

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            // 字符串赋值
            strcpy(search_request.number, search_number);
            search_request.query_number = EQUAL;

            printf("update search request number: %s\n", search_number);
        }

        ImGui::Unindent(10);
    }

    // 利用始发站关键词搜索
    ImGui::Checkbox("Search By Start Station", &is_search_by_start_station_keyword);
    if(is_search_by_start_station_keyword)
    {
        ImGui::Indent(10);

        bool is_update = ImGui::InputText("Start Station", search_start, IM_ARRAYSIZE(search_start));

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            // 字符串赋值
            strcpy(search_request.start_station, search_start);
            search_request.query_start_station = EQUAL;

            printf("update search request start station: %s\n", search_start);
        }

        ImGui::Unindent(10);
    }

    // 利用到达站关键词搜索
    ImGui::Checkbox("Search By Arrive Station", &is_search_by_arrive_station_keyword);
    if(is_search_by_arrive_station_keyword)
    {
        ImGui::Indent(10);

        bool is_update = ImGui::InputText("Arrive Station", search_arrive, IM_ARRAYSIZE(search_arrive));

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            // 字符串赋值
            strcpy(search_request.arrive_station, search_arrive);
            search_request.query_arrive_station = EQUAL;

            printf("update search request arrive station: %s\n", search_arrive);
        }

        ImGui::Unindent(10);
    }

    // 利用出发时间搜索
    ImGui::Checkbox("Search By Start Time", &is_search_by_start_time);
    if(is_search_by_start_time)
    {
        static int current_item = 1;

        ImGui::Indent(10);

        bool is_update = false;
        is_update |= InputTime("Start Time", &search_start_time);
        is_update |= select_comparison_operators("Start Time", &current_item); // 下拉选择框

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            search_request.start_time       = date_to_uint64_time(search_start_time);
            search_request.query_start_time = current_item;

            printf("update search request start time: %s\n", date_to_string(search_start_time).c_str());
        }

        ImGui::Unindent(10);
    }

    // 利用到达时间搜索
    ImGui::Checkbox("Search By Arrive Time", &is_search_by_arrive_time);
    if(is_search_by_arrive_time)
    {
        static int current_item = 1;

        ImGui::Indent(10);

        bool is_update = false;
        is_update |= InputTime("Arrive Time", &search_arrive_time);
        is_update |= select_comparison_operators("Arrive Time", &current_item); // 下拉选择框

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            search_request.arrive_time       = date_to_uint64_time(search_arrive_time);
            search_request.query_arrive_time = current_item;

            printf("update search request arrive time: %s\n", date_to_string(search_arrive_time).c_str());
        }

        ImGui::Unindent(10);
    }

    // 利用票数搜索
    ImGui::Checkbox("Search By Ticket Remain", &is_search_by_ticket_remain);
    if(is_search_by_ticket_remain)
    {
        static int current_item = 1;

        ImGui::Indent(10);

        bool is_update = false;
        is_update |= ImGui::InputScalarN("Ticket Remain", ImGuiDataType_U32, &search_ticket_remain, 1, nullptr, nullptr, "%u");
        is_update |= select_comparison_operators("Ticket Remain", &current_item); // 下拉选择框

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            search_request.ticket_remain       = search_ticket_remain;
            search_request.query_ticket_remain = current_item;

            printf("update search request ticket remain: %d, %d\n", search_ticket_remain, current_item);
        }

        ImGui::Unindent(10);
    }

    // 利用票价搜索
    ImGui::Checkbox("Search By Ticket Price", &is_search_by_ticket_price);
    if(is_search_by_ticket_price)
    {
        static int current_item = 1;

        ImGui::Indent(10);

        bool is_update = false;
        // 保留两位小数
        is_update |= ImGui::InputScalarN("Ticket Price", ImGuiDataType_Float, &search_ticket_price, 1, nullptr, nullptr, "%.2f");
        is_update |= select_comparison_operators("Ticket Price", &current_item); // 下拉选择框

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            search_request.ticket_price       = float_to_uint32_price(search_ticket_price);
            search_request.query_ticket_price = current_item;

            // 保留两位小数
            printf("update search request ticket price: %s, %d\n", uint32_price_to_string(search_request.ticket_price).c_str(), current_item);
        }

        ImGui::Unindent(10);
    }

    // 利用车次状态搜索
    ImGui::Checkbox("Search By Train Status", &is_search_by_train_status);
    if(is_search_by_train_status)
    {
        ImGui::Indent(10);

        bool is_update = false;

        // 下拉框选择车次状态
        ImGui::Text("Train Status");
        const char* items[] = { "NORMAL", "DELAY", "STOP", "CANCEL", "UNKNOWN" };
        is_update |= ImGui::Combo("##Train Status", (int*)&search_train_status, items, IM_ARRAYSIZE(items));

        // 如果更新了查询条件，调整请求
        if(is_update)
        {
            search_request.train_status     = search_train_status;
            search_request.query_is_running = EQUAL;

            printf("update search request train status: %s\n", parse_train_status(search_train_status).c_str());
        }

        ImGui::Unindent(10);
    }

    ImGui::End();
    ImGui::PopFont();
}
