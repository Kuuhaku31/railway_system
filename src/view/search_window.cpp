
// search_window.cpp

#include "view.h"

// #include "controller.h"
extern "C" {
#include "system_controller.h"
}

#define TEXT_SEARCH_WINDOW_FILTERS "Filters" // 过滤器

extern bool is_use_filter;
extern bool is_search_by_id;
extern bool is_search_by_number_keyword;
extern bool is_search_by_start_station_keyword;
extern bool is_search_by_arrive_station_keyword;
extern bool is_search_by_start_time;
extern bool is_search_by_arrive_time;
extern bool is_search_by_ticket_remain;
extern bool is_search_by_ticket_price;
extern bool is_search_by_train_status;

extern uint32_t    search_id;
extern char        search_number[16];
extern char        search_start[64];
extern char        search_arrive[64];
extern Date        search_start_time;
extern Date        search_arrive_time;
extern uint32_t    search_ticket_remain;
extern uint32_t    search_ticket_price;
extern TrainStatus search_train_status;

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

// static Controller& controller     = Controller::Instance();
// static TrainQuery& search_request = controller.search_request;

extern bool system_is_fresh_data;

extern TrainQuery system_search_request;
static TrainQuery search_request = system_search_request;

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

    if(ImGui::Checkbox(TEXT_SEARCH_WINDOW_FILTERS, &is_use_filter))
    {
        system_is_fresh_data = true;
    }
    if(!is_use_filter)
    {
        // 查询所有数据
        search_request.id       = 0;
        search_request.query_id = GREATER;

        ImGui::End();
        ImGui::PopFont();
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
            search_request.id       = search_id;
            search_request.query_id = current_item;

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
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

            system_is_fresh_data = true;
        }

        ImGui::Unindent(10);
    }

    ImGui::End();
    ImGui::PopFont();
}
