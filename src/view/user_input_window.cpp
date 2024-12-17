
// user_input_window.cpp

// #include "train_controller.h"
#include "controller.h"
#include "view.h"

#define MAX_SIZE 128

void
DrawTimeInput(const char* label, Date& time)
{
    ImGui::Text(label);
    ImGui::PushItemWidth(100);

    // 输入年
    ImGui::InputScalar((std::string("年##") + label).c_str(), ImGuiDataType_U32, &time.year);
    if(time.year <= 0) time.year = 1;

    ImGui::SameLine();

    // 输入月
    ImGui::InputScalar((std::string("月##") + label).c_str(), ImGuiDataType_U32, &time.month);
    if(time.month <= 0) time.month = 12;
    if(time.month > 12) time.month = 1;

    ImGui::SameLine();

    // 输入日
    ImGui::InputScalar((std::string("日##") + label).c_str(), ImGuiDataType_U32, &time.day);
    if(time.day <= 0) time.day = 31;
    if(time.day > 31) time.day = 1;

    // 输入小时
    ImGui::InputScalar((std::string("时##") + label).c_str(), ImGuiDataType_U32, &time.hour);
    if(time.hour < 0) time.hour = 23;
    if(time.hour > 23) time.hour = 0;

    ImGui::SameLine();

    // 输入分钟
    ImGui::InputScalar((std::string("分##") + label).c_str(), ImGuiDataType_U32, &time.minute);
    if(time.minute < 0) time.minute = 59;
    if(time.minute > 59) time.minute = 0;

    ImGui::SameLine();

    // 输入秒
    ImGui::InputScalar((std::string("秒##") + label).c_str(), ImGuiDataType_U32, &time.second);
    if(time.second < 0) time.second = 59;

    ImGui::PopItemWidth();
}

static Controller& controller = Controller::Instance();

void
View::show_user_input_window(bool* p_open)
{
    static int  train_id = 0;           // 车次 ID
    static char train_number[MAX_SIZE]; // 车次

    static char train_start_station[MAX_SIZE];  // 始发站
    static char train_arrive_station[MAX_SIZE]; // 到达站
    static Date train_start_time;               // 出发时间
    static Date train_arrive_time;              // 到达时间

    static int   train_ticket_count = 0;    // 票数
    static float train_ticket_price = 0.0f; // 价格

    static TrainStatus train_status = TrainStatus::OTHER; // 是否有效

    // 设置按钮的颜色
    static ImVec4 normal_color  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f); // 正常颜色
    static ImVec4 hovered_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // 悬停颜色
    static ImVec4 active_color  = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 按下颜色
    static ImVec4 disable_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 禁用颜色

    static bool unable_insert = false;
    static bool unable_del    = false;
    static bool unable_update = true;

    if(p_open && !*p_open) return;

    bool is_insert = false;
    bool is_del    = false;
    bool is_update = false;

    if(is_selected_new) // 如果选中了新的车次
    {
        is_selected_new = false;

        controller.RailwaySystemSearchTrainData();

        // 如果选中了新的车次，将该车次的数据显示在输入框中
        if(controller.SelectTrainData(selected_id))
        {
            // 如果存在该车次，禁用插入按钮，启用删除和更新按钮
            unable_insert = true;
            unable_del    = false;
            unable_update = false;

            table_to_selected = true;
        }
        else
        {
            // 如果不存在该车次，启用插入按钮，禁用删除和更新按钮
            unable_insert = false;
            unable_del    = true;
            unable_update = true;
        }
    }

    // 将数据拷贝到输入框中
    train_id = controller.processing_data.train_id;
    strncpy(train_number, controller.processing_data.train_number.c_str(), MAX_SIZE);

    strncpy(train_start_station, controller.processing_data.train_start_station.c_str(), MAX_SIZE);
    strncpy(train_arrive_station, controller.processing_data.train_arrive_station.c_str(), MAX_SIZE);

    train_start_time  = controller.processing_data.train_start_time;
    train_arrive_time = controller.processing_data.train_arrive_time;

    train_ticket_count = controller.processing_data.train_ticket_count;
    train_ticket_price = controller.processing_data.train_ticket_price;

    train_status = controller.processing_data.train_status;


    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushFont(font_chinese); // 使用中文字体
    ImGui::SetNextWindowPos(input_window_pos);
    ImGui::SetNextWindowSize(input_window_size);
    ImGui::Begin("Input Text", p_open, window_flags);
    ImGui::Text("processing data:");

    // 如果选中了某个车次，将该车次的数据显示在输入框中
    if(ImGui::InputScalar("Train ID", ImGuiDataType_U32, &train_id))
    {
        is_selected_new = true;
    }
    ImGui::InputText("Train Number", train_number, MAX_SIZE);

    ImGui::InputText("Start Station", train_start_station, MAX_SIZE);
    ImGui::InputText("Arrive Station", train_arrive_station, MAX_SIZE);

    // 出发时间
    DrawTimeInput("Start Time", train_start_time);
    // 到达时间
    DrawTimeInput("Arrive Time", train_arrive_time);

    ImGui::InputScalar("Ticket Count", ImGuiDataType_U32, &train_ticket_count);
    ImGui::InputScalar("Ticket Price", ImGuiDataType_Float, &train_ticket_price);

    // 下拉框选择车次状态
    ImGui::Text("Train Status");
    const char* items[] = { "NORMAL", "DELAY", "STOP", "OTHER" };
    ImGui::Combo("##Train Status", (int*)&train_status, items, IM_ARRAYSIZE(items));

    // 分割线
    ImGui::Separator();

    // 按钮
    if(unable_insert)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);

        ImGui::Button("Insert", ImVec2(100, 0));

        ImGui::PopStyleColor(3);
    }
    else
    {
        is_insert = ImGui::Button("Insert", ImVec2(100, 0));
    }

    ImGui::SameLine();

    if(unable_del)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);

        ImGui::Button("Delete", ImVec2(100, 0));

        ImGui::PopStyleColor(3);
    }
    else
    {
        is_del = ImGui::Button("Delete", ImVec2(100, 0));
    }

    ImGui::SameLine();

    if(unable_update)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);

        ImGui::Button("Update", ImVec2(100, 0));

        ImGui::PopStyleColor(3);
    }
    else
    {
        is_update = ImGui::Button("Update", ImVec2(100, 0));
    }

    ImGui::End();
    ImGui::PopFont();

    // 将输入框中的数据拷贝到 controller.processing_data 中
    controller.processing_data.train_id = train_id;

    controller.processing_data.train_number = train_number[0] == '\0' ? "UNKNOWN" : train_number;

    controller.processing_data.train_start_station  = train_start_station[0] == '\0' ? "UNKNOWN" : train_start_station;
    controller.processing_data.train_arrive_station = train_arrive_station[0] == '\0' ? "UNKNOWN" : train_arrive_station;
    controller.processing_data.train_start_time     = train_start_time;
    controller.processing_data.train_arrive_time    = train_arrive_time;

    controller.processing_data.train_ticket_count = train_ticket_count;
    controller.processing_data.train_ticket_price = train_ticket_price;

    controller.processing_data.train_status = train_status;

    selected_id = train_id;

    if(is_insert)
    {
        controller.RailwaySystemInsertTrainData();

        unable_insert = true;
        unable_del    = false;
        unable_update = false;

        is_selected_new = true;

        // 日志
        ViewConsoleAddLog("Insert train data: %d, %s, %s, %s, %s, %d, %.2f, %s",
            train_id,
            train_number,
            train_start_station,
            train_arrive_station,
            date_to_string(train_start_time).c_str(),
            train_ticket_count,
            train_ticket_price,
            parse_train_status(train_status).c_str());
        console_scroll_to_bottom = true;
    }

    if(is_del)
    {
        controller.RailwaySystemDelTrainData(selected_id);

        unable_insert = false;
        unable_del    = true;
        unable_update = true;

        is_selected_new = true;

        // 日志
        ViewConsoleAddLog("Delete train data: %d, %s, %s, %s, %s, %d, %.2f, %s",
            train_id,
            train_number,
            train_start_station,
            train_arrive_station,
            date_to_string(train_start_time).c_str(),
            train_ticket_count,
            train_ticket_price,
            parse_train_status(train_status).c_str());
        console_scroll_to_bottom = true;
    }

    if(is_update)
    {
        controller.RailwaySystemInsertTrainData();

        is_selected_new = true;

        // 日志
        ViewConsoleAddLog("Update train data: %d, %s, %s, %s, %s, %d, %.2f, %s",
            train_id,
            train_number,
            train_start_station,
            train_arrive_station,
            date_to_string(train_start_time).c_str(),
            train_ticket_count,
            train_ticket_price,
            parse_train_status(train_status).c_str());
        console_scroll_to_bottom = true;
    }
}
