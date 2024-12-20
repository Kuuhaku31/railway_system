
// view.cpp

#include "system_view.h"
#include "view.h"

// #include "controller.h"
#include "date.h"
extern "C" {
#include "system_controller.h"
}

extern bool system_is_running;

extern bool view_is_show_train_datas;
extern bool view_is_show_user_input;
extern bool view_is_show_console;
extern bool view_is_show_search_window;

extern uint8_t view_clear_color[4];

static ImGui_setup& imgui_setup = ImGui_setup::Instance();

void
SystemViewInit()
{
    View::Instance().ViewInit();
}

void
SystemQuitView()
{
    View::Instance().ViewQuit();
}

void
SystemViewOnFrameBegin()
{
    imgui_setup.On_frame_begin();
}

void
SystemViewOnFrameEnd()
{
    static Color clear_color = { VIEW_DEFAULT_CLEAR_COLOR };
    imgui_setup.On_frame_end(&clear_color);
}

void
SystemProcessEvent()
{
    static Event e;
    while(SDL_PollEvent(&e))
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if(e.type == SDL_QUIT)
        {
            system_is_running = false;
        }
    }
}

void
SystemViewShowWindows()
{
    View::Instance().ViewShowWindows();
}

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
    update_view_layout();

    show_train_datas_window(&view_is_show_train_datas);
    show_user_input_window(&view_is_show_user_input);
    show_console_window(&view_is_show_console);
    show_search_window(&view_is_show_search_window);
}

void
View::update_view_layout()
{
    // 应用程序显示大小
    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    // 数据窗口位置和大小
    data_window_pos    = ImVec2(0, 0);
    data_window_size.x = display_size.x;
    data_window_size.y = (view_is_show_user_input || view_is_show_console) ? display_size.y * data_window_height : display_size.y;

    // 用户输入窗口位置和大小
    input_window_pos    = ImVec2(0, display_size.y * data_window_height);
    input_window_size.x = view_is_show_console ? display_size.x * inuput_window_width : display_size.x;
    input_window_size.y = display_size.y * (1 - data_window_height);

    // 控制台窗口位置和大小
    console_window_pos.x  = view_is_show_user_input ? display_size.x * inuput_window_width : 0;
    console_window_pos.y  = display_size.y * data_window_height;
    console_window_size.x = view_is_show_user_input ? display_size.x * (1 - inuput_window_width) : display_size.x;
    console_window_size.y = display_size.y * (1 - data_window_height);
}
