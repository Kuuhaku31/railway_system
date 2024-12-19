
// console_window.cpp

#include "view.h"

#include "controller.h"


void
View::show_console_window(bool* p_open)
{
    static Controller& controller = Controller::Instance();

    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 中文显示
    ImGui::PushFont(font_chinese);
    ImGui::SetNextWindowPos(console_window_pos);
    ImGui::SetNextWindowSize(console_window_size);
    ImGui::Begin("log window", p_open, window_flags);

    ImGui::SameLine();
    if(ImGui::Button("Fresh Datas", button_size_02)) controller.is_fresh_data = true;

    ImGui::SameLine();
    if(ImGui::Button("Export Logs", button_size_02)) controller.ControllerExportLogs();

    ImGui::SameLine();
    if(ImGui::Button("Config Data", button_size_02)) is_show_user_input = !is_show_user_input;

    ImGui::SameLine();
    // 如果活动，设置按钮样式
    if(is_use_filter)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, active_normal_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, active_hovered_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active_active_color);
        if(ImGui::Button("Filters ON", button_size_02)) is_show_search_window = !is_show_search_window;
        ImGui::PopStyleColor(3);
    }
    else
    {
        if(ImGui::Button("Filters OFF", button_size_02)) is_show_search_window = !is_show_search_window;
    }

    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();                        // 分割线

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    for(const auto& log : controller.ControllerGetLogs())
    {
        ImGui::TextUnformatted(log.c_str());
    }

    if(console_scroll_to_bottom) ImGui::SetScrollHereY(1.0f);
    console_scroll_to_bottom = false;

    ImGui::EndChild();

    ImGui::End();

    ImGui::PopFont();
}
