
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
    static Rect window_rect = { 30, 40, 1200, 900 };
    imgui_setup.Init("view", window_rect, false);

    // 加载默认字体
    ImGuiIO& io  = ImGui::GetIO();
    font_default = io.Fonts->AddFontDefault();

    // 加载中文字体（微软雅黑）
    font_chinese = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
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

    ImGuiInputText();
}
