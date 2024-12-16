
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
    ImGui::Text("Train Datas");
    ImGui::End();
}
