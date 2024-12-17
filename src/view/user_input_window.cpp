
// user_input_window.cpp

#include "controler.h"
#include "view.h"

void
DrawTimeInput(const char* label, Date& time)
{
    ImGui::Text(label);
    ImGui::PushItemWidth(100);

    // 输入年
    ImGui::InputInt((std::string("年##") + label).c_str(), &time.year, 1, 10);
    if(time.year <= 0) time.year = 1;

    ImGui::SameLine();

    // 输入月
    ImGui::InputInt((std::string("月##") + label).c_str(), &time.month, 1, 10);
    if(time.month <= 0) time.month = 12;
    if(time.month > 12) time.month = 1;

    ImGui::SameLine();

    // 输入日
    ImGui::InputInt((std::string("日##") + label).c_str(), &time.day, 1, 10);
    if(time.day <= 0) time.day = 31;
    if(time.day > 31) time.day = 1;

    ImGui::SameLine();

    // 输入小时
    ImGui::InputInt((std::string("时##") + label).c_str(), &time.hour, 1, 10);
    if(time.hour < 0) time.hour = 23;
    if(time.hour > 23) time.hour = 0;

    ImGui::SameLine();

    // 输入分钟
    ImGui::InputInt((std::string("分##") + label).c_str(), &time.minute, 1, 10);
    if(time.minute < 0) time.minute = 59;
    if(time.minute > 59) time.minute = 0;

    ImGui::SameLine();

    // 输入秒
    ImGui::InputInt((std::string("秒##") + label).c_str(), &time.second, 1, 10);
    if(time.second < 0) time.second = 59;

    ImGui::PopItemWidth();
}

static Controler& controler = Controler::Instance();

void
View::show_user_input_window(bool* p_open)
{
    static const int MAX_SIZE = 256;

    static uint32_t train_id = 0;           // 车次 ID
    static char     train_number[MAX_SIZE]; // 车次

    static char train_start_station[MAX_SIZE];  // 始发站
    static char train_arrive_station[MAX_SIZE]; // 到达站
    static Date train_start_time;               // 出发时间
    static Date train_arrive_time;              // 到达时间

    static uint32_t train_ticket_count = 0;    // 票数
    static float    train_ticket_price = 0.0f; // 价格

    static TrainStatus train_status = TrainStatus::OTHER; // 是否有效

    // 设置按钮的颜色
    static ImVec4 normal_color  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f); // 正常颜色
    static ImVec4 hovered_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // 悬停颜色
    static ImVec4 active_color  = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 按下颜色
    static ImVec4 disable_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 禁用颜色

    static bool is_selected_new = false;

    if(p_open && !*p_open) return;

    bool unable_insert = false;
    bool unable_del    = false;
    bool unable_update = true;

    bool is_insert = false;
    bool is_del    = false;
    bool is_update = false;

    if(is_selected_new)
    {
        // 如果选中了新的车次，将该车次的数据显示在输入框中
        controler.SelectTrainData(selected_id);
        is_selected_new = false;
    }

    // 将数据拷贝到输入框中
    train_id = controler.processing_data.train_id;
    strncpy(train_number, controler.processing_data.train_number.c_str(), MAX_SIZE);

    strncpy(train_start_station, controler.processing_data.train_start_station.c_str(), MAX_SIZE);
    strncpy(train_arrive_station, controler.processing_data.train_arrive_station.c_str(), MAX_SIZE);

    train_start_time  = controler.processing_data.train_start_time;
    train_arrive_time = controler.processing_data.train_arrive_time;

    train_ticket_count = controler.processing_data.train_ticket_count;
    train_ticket_price = controler.processing_data.train_ticket_price;

    train_status = controler.processing_data.train_status;

    ImVec2 display_size = ImGui::GetIO().DisplaySize;
    ImVec2 window_pos;
    ImVec2 window_size = display_size;

    window_pos.y = display_size.y * data_window_height;
    window_size.x *= inuput_window_width;
    window_size.y *= (1 - data_window_height);

    uint32_t window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 使用中文字体
    ImGui::PushFont(font_chinese);

    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowSize(window_size);
    ImGui::Begin("Input Text", p_open, window_flags);
    ImGui::Text("processing data:");

    // 如果选中了某个车次，将该车次的数据显示在输入框中
    if(ImGui::InputScalar("Train ID", ImGuiDataType_S32, &train_id))
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

    // 将输入框中的数据拷贝到 controler.processing_data 中
    controler.processing_data.train_id = train_id;

    controler.processing_data.train_number = train_number[0] == '\0' ? "UNKNOWN" : train_number;

    controler.processing_data.train_start_station  = train_start_station[0] == '\0' ? "UNKNOWN" : train_start_station;
    controler.processing_data.train_arrive_station = train_arrive_station[0] == '\0' ? "UNKNOWN" : train_arrive_station;
    controler.processing_data.train_start_time     = train_start_time;
    controler.processing_data.train_arrive_time    = train_arrive_time;

    controler.processing_data.train_ticket_count = train_ticket_count;
    controler.processing_data.train_ticket_price = train_ticket_price;

    controler.processing_data.train_status = train_status;

    selected_id = train_id;

    if(is_insert)
    {
        controler.RailwaySystemInsertTrainData();
    }
}
