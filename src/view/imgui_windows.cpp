
// imgui_windows.cpp

#include "imgui_windows.h"

#include "view.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string
uint64ToTextTime(uint64_t timestamp)
{
    // 将时间戳转换为 time_t 类型
    std::time_t time = static_cast<std::time_t>(timestamp);
    // 将 time_t 转换为 tm 结构
    std::tm* tm = std::localtime(&time);
    // 使用字符串流格式化时间
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

static View& view = View::Instance();

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

    ImGui::End();
}

void
ImGuiWindowTrainDatas(bool* p_open)
{
    if(p_open && !*p_open) return;

    ImGui::Begin("Train Datas", p_open);
    ImGui::Text("Train Datas:");

    const View::TrainDatas& train_datas = view.train_datas;

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
            ImGui::Text("%s", uint64ToTextTime(train_data->train_start_time).c_str());
            // 到达时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", uint64ToTextTime(train_data->train_arrive_time).c_str());
            // 票数
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data->train_ticket_count);
            // 价格
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", train_data->train_ticket_price);
            // 是否有效
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->is_running ? "true" : "false");
        }

        ImGui::EndTable();
    }


    ImGui::End();
}
