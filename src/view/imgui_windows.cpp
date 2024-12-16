
// imgui_windows.cpp

#include "imgui_windows.h"

#include "controler.h"
#include "view.h"

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

        printf("processing data: \n");
        printf("train_id: %d\n", train_data.train_id);
        printf("train_number: %s\n", train_data.train_number.c_str());
        printf("train_start_station: %s\n", train_data.train_start_station.c_str());
        printf("train_arrive_station: %s\n", train_data.train_arrive_station.c_str());
        printf("train_start_time: %s\n", date_to_string(train_data.train_start_time).c_str());
        printf("train_arrive_time: %s\n", date_to_string(train_data.train_arrive_time).c_str());
        printf("train_ticket_count: %d\n", train_data.train_ticket_count);
        printf("train_ticket_price: %.2f\n", train_data.train_ticket_price);
        printf("train_status: %s\n", parse_train_status(train_data.train_status).c_str());
    }

    // 输入一个整数
    static int number = 0;
    if(ImGui::InputInt("input number", &number))
    {
        controler.RailwaySystemSearchTrainData(number);
    }

    ImGui::End();
}
