
// console_window.cpp

#include "view.h"

extern "C" {
#include "system_controller.h"
}

#include "util_funcs.h"
#include "window_rect.h"

extern std::vector<std::string> system_logs;

extern bool system_is_fresh_data;

extern bool view_is_show_user_input;
extern bool view_is_show_search_window;

extern const WindowRect view_console_window_rect;

void
View::show_console_window(bool* p_open)
{
    // static Controller& controller = Controller::Instance();

    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 中文显示
    ImGui::PushFont(font_chinese);
    ImGui::SetNextWindowPos(get_rect_pos(view_console_window_rect));
    ImGui::SetNextWindowSize(get_rect_size(view_console_window_rect));
    ImGui::Begin("log window", p_open, window_flags);

    ImGui::SameLine();
    if(ImGui::Button("Fresh Datas", button_size_02)) system_is_fresh_data = true;

    ImGui::SameLine();
    if(ImGui::Button("Export Logs", button_size_02))
    {
        SystemControllerExportLogs();
    }

    ImGui::SameLine();
    if(ImGui::Button("Config Data", button_size_02)) view_is_show_user_input = !view_is_show_user_input;

    ImGui::SameLine();
    // 如果活动，设置按钮样式
    if(is_use_filter)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, active_normal_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, active_hovered_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_active_color);
        if(ImGui::Button("Filters ON", button_size_02)) view_is_show_search_window = !view_is_show_search_window;
        ImGui::PopStyleColor(3);
    }
    else
    {
        if(ImGui::Button("Filters OFF", button_size_02)) view_is_show_search_window = !view_is_show_search_window;
    }

    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();                        // 分割线

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    for(const auto& log : system_logs)
    {
        ImGui::TextUnformatted(log.c_str());
    }

    if(console_scroll_to_bottom) ImGui::SetScrollHereY(1.0f);
    console_scroll_to_bottom = false;

    ImGui::EndChild();

    ImGui::End();

    ImGui::PopFont();
}

// Kuuhaku-kazari
// 2024.12.21
