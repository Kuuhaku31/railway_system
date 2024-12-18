
// view.cpp

#include "view.h"

#include "controller.h"
#include "date.h"

static ImGui_setup& imgui_setup = ImGui_setup::Instance();
static Controller&  controller  = Controller::Instance();

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
    imgui_setup.Init(APPLICATION_NAME, WINDOW_INIT_RECT, false);

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
    controller.ControllerUpdate();

    update_view_layout();

    show_train_datas_window(&is_show_train_datas);
    show_user_input_window(&is_show_user_input);
    show_console_window(&is_show_console);
    show_search_window(&is_show_search_window);
}

void
View::update_view_layout()
{
    // 应用程序显示大小
    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    // 数据窗口位置和大小
    data_window_pos    = ImVec2(0, 0);
    data_window_size.x = display_size.x;
    data_window_size.y = (is_show_user_input || is_show_console) ? display_size.y * data_window_height : display_size.y;

    // 用户输入窗口位置和大小
    input_window_pos    = ImVec2(0, display_size.y * data_window_height);
    input_window_size.x = is_show_console ? display_size.x * inuput_window_width : display_size.x;
    input_window_size.y = display_size.y * (1 - data_window_height);

    // 控制台窗口位置和大小
    console_window_pos.x  = is_show_user_input ? display_size.x * inuput_window_width : 0;
    console_window_pos.y  = display_size.y * data_window_height;
    console_window_size.x = is_show_user_input ? display_size.x * (1 - inuput_window_width) : display_size.x;
    console_window_size.y = display_size.y * (1 - data_window_height);
}
