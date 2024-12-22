
// view.cpp

#include "view.h"

#include "system_controller.h"
#include "system_view.h"

#include "date.h"
#include "window_rect.h"


extern bool system_is_running;

extern bool view_is_show_train_datas;
extern bool view_is_show_user_input;
extern bool view_is_show_console;
extern bool view_is_show_search_window;

extern WindowRect view_app_window_rect;

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

    // 更新窗口布局
    view_app_window_rect.w = ImGui::GetIO().DisplaySize.x;
    view_app_window_rect.h = ImGui::GetIO().DisplaySize.y;
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

    ImGuiIO& io = ImGui::GetIO();

    // 加载默认字体
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
    show_train_datas_window(&view_is_show_train_datas);
    show_user_input_window(&view_is_show_user_input);
    show_console_window(&view_is_show_console);
    show_filters_window(&view_is_show_search_window);
}

// Kuuhaku-kazari
// 2024.12.21
