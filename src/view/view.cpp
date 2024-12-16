
// view.cpp

#include "view.h"

#include "imgui_windows.h"

static ImGui_setup& imgui_setup = ImGui_setup::Instance();

View* View::instance = nullptr;
View&
View::Instance()
{
    if(instance == nullptr) instance = new View();
    return *instance;
}

void
View::ViewInit()
{
    imgui_setup.Init("view", { 30, 40, 800, 600 }, false);
}

void
View::ViewQuit()
{
    imgui_setup.Quit();
}

void
View::ViewShowWindows()
{
    ImGuiWindowConfig();

    ImGuiWindowTrainDatas(&is_show_train_datas);

    if(is_show_demo_window) ImGui::ShowDemoWindow(&is_show_demo_window);
}
