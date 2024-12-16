
// imgui_windows.cpp

#include "imgui_windows.h"

#include "view.h"

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

    const View::TrainDatas& train_datas = view.GetTrainDatas();

    // 遍历显示车次信息
    for(auto& train_data : train_datas)
    {
        if(train_data == nullptr) continue;

        ImGui::Text("Train ID: %d", train_data->train_id);
        ImGui::Text("Train Ticket Count: %d", train_data->train_ticket_count);
        ImGui::Text("Train Ticket Price: %.2f", train_data->train_ticket_price);
        ImGui::Text("Train Start Time: %llu", train_data->train_start_time);
        ImGui::Text("Train Arrive Time: %llu", train_data->train_arrive_time);
        ImGui::Text("Train Number: %s", train_data->train_number);
        ImGui::Text("Train Start Station: %s", train_data->train_start_station);
        ImGui::Text("Train Arrive Station: %s", train_data->train_arrive_station);
        ImGui::Text("Is Running: %s", train_data->is_running ? "true" : "false");
    }

    ImGui::End();
}
