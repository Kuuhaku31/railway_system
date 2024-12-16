
// imgui_windows.cpp

#include "imgui_windows.h"

#include "controler.h"
#include "view.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string
parse_train_status(TrainStatus status)
{
    switch(status)
    {
    case TrainStatus::NORMAL:
        return "NORMAL";
    case TrainStatus::DELAY:
        return "DELAY";
    case TrainStatus::STOP:
        return "STOP";
    case TrainStatus::OTHER:
        return "OTHER";
    default:
        return "UNKNOWN";
    }
}

std::string
date_to_string(const Date& date)
{
    std::stringstream ss;
    ss << date.year << "-"
       << std::setw(2) << std::setfill('0') << date.month << "-"
       << std::setw(2) << std::setfill('0') << date.day << " "
       << std::setw(2) << std::setfill('0') << date.hour << ":"
       << std::setw(2) << std::setfill('0') << date.minute << ":"
       << std::setw(2) << std::setfill('0') << date.second;
    return ss.str();
}

static View&      view      = View::Instance();
static Controler& controler = Controler::Instance();

void
ImGuiWindowConfig(bool* p_open)
{
    // 如果 p_open 不为空，且 p_open 为 false，则返回
    if(p_open && !*p_open) return;

    ImGui::Begin("config", p_open);
    // 显示帧率
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Show Demo Window", &view.is_show_demo_window);
    ImGui::Checkbox("Show Train Datas", &view.is_show_train_datas);

    if(ImGui::Button("print user processing data"))
    {
        TrainData& train_data = controler.processing_data;
        std::cout << "processing data: " << std::endl;
        std::cout << "train_id: " << train_data.train_id << std::endl;
        std::cout << "train_number: " << train_data.train_number << std::endl;
        std::cout << "train_start_station: " << train_data.train_start_station << std::endl;
        std::cout << "train_arrive_station: " << train_data.train_arrive_station << std::endl;
        std::cout << "train_start_time: " << date_to_string(train_data.train_start_time) << std::endl;
        std::cout << "train_arrive_time: " << date_to_string(train_data.train_arrive_time) << std::endl;
        std::cout << "train_ticket_count: " << train_data.train_ticket_count << std::endl;
        std::cout << "train_ticket_price: " << train_data.train_ticket_price << std::endl;
        std::cout << "train_status: " << parse_train_status(train_data.train_status) << std::endl;
    }

    ImGui::End();
}

void
ImGuiWindowTrainDatas(bool* p_open)
{
    if(p_open && !*p_open) return;

    // 中文显示
    ImGui::PushFont(view.font_chinese);
    ImGui::Begin("Train Datas", p_open);
    ImGui::Text("Train Datas:");

    const Controler::TrainDatas& train_datas = controler.train_datas;

    // 表格显示车次信息
    uint32_t flags      = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
    ImVec2   outer_size = ImGui::GetWindowSize();
    float    inner_size = outer_size.x - 20.0f;
    if(ImGui::BeginTable("TrainDatas", 9, flags, outer_size, inner_size))
    {
        ImGui::TableSetupColumn("Train ID", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Train Number", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Start Station", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Arrive Station", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Start Time", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Arrive Time", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Ticket Count", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Ticket Price", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Is Running", ImGuiTableColumnFlags_WidthFixed, 100.0f);

        ImGui::TableHeadersRow();

        for(auto& train_data : train_datas)
        {
            ImGui::TableNextRow();
            // 车次 ID
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data->train_id);
            // 车次
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->train_number.c_str());
            // 始发站
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->train_start_station.c_str());
            // 到达站
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->train_arrive_station.c_str());
            // 出发时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", date_to_string(train_data->train_start_time).c_str());
            // 到达时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", date_to_string(train_data->train_arrive_time).c_str());
            // 票数
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data->train_ticket_count);
            // 价格
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", train_data->train_ticket_price);
            // 是否有效
            ImGui::TableNextColumn();
            ImGui::Text("%s", parse_train_status(train_data->train_status).c_str());
        }

        ImGui::EndTable();
    }

    ImGui::End();
    ImGui::PopFont();
}


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

void
ImGuiInputText(bool* p_open)
{
    static const int MAX_SIZE = 256;

    TrainData& train_data = controler.processing_data;

    static uint32_t train_id = 0;           // 车次 ID
    static char     train_number[MAX_SIZE]; // 车次

    static char train_start_station[MAX_SIZE];  // 始发站
    static char train_arrive_station[MAX_SIZE]; // 到达站
    static Date train_start_time;               // 出发时间
    static Date train_arrive_time;              // 到达时间

    static uint32_t train_ticket_count = 0;    // 票数
    static float    train_ticket_price = 0.0f; // 价格

    static TrainStatus train_status = TrainStatus::OTHER; // 是否有效

    // 将数据拷贝到输入框中
    train_id = train_data.train_id;
    strncpy(train_number, train_data.train_number.c_str(), MAX_SIZE);

    strncpy(train_start_station, train_data.train_start_station.c_str(), MAX_SIZE);
    strncpy(train_arrive_station, train_data.train_arrive_station.c_str(), MAX_SIZE);

    train_start_time  = train_data.train_start_time;
    train_arrive_time = train_data.train_arrive_time;

    train_ticket_count = train_data.train_ticket_count;
    train_ticket_price = train_data.train_ticket_price;

    train_status = train_data.train_status;

    // 使用中文字体
    ImGui::PushFont(view.font_chinese);

    ImGui::Begin("Input Text");
    ImGui::Text("processing data:");

    ImGui::InputScalar("Train ID", ImGuiDataType_U32, &train_id);
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

    ImGui::End();

    ImGui::PopFont();

    // 将输入框中的数据拷贝到 train_data 中
    train_data.train_id = train_id;

    train_data.train_number = train_number[0] == '\0' ? "UNKNOWN" : train_number;

    train_data.train_start_station  = train_start_station[0] == '\0' ? "UNKNOWN" : train_start_station;
    train_data.train_arrive_station = train_arrive_station[0] == '\0' ? "UNKNOWN" : train_arrive_station;
    train_data.train_start_time     = train_start_time;
    train_data.train_arrive_time    = train_arrive_time;

    train_data.train_ticket_count = train_ticket_count;
    train_data.train_ticket_price = train_ticket_price;

    train_data.train_status = train_status;
}
