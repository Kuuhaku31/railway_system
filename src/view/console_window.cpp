
// console_window.cpp

#include "view.h"

#include "controller.h"

static Controller& controller = Controller::Instance();

void
View::ViewConsoleShowLog(bool* p_open)
{
    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 中文显示
    ImGui::PushFont(font_chinese);
    ImGui::SetNextWindowPos(console_window_pos);
    ImGui::SetNextWindowSize(console_window_size);
    ImGui::Begin("log window", p_open, window_flags);

    if(ImGui::Button("Clear Logs")) logs.clear();

    ImGui::SameLine();
    if(ImGui::Button("Refresh")) controller.is_fresh_data = true;

    ImGui::SameLine();
    if(ImGui::Button("Clear Buffer")) controller.is_clear_buffer = true;

    ImGui::SameLine();
    if(ImGui::Button("Config")) is_show_user_input = !is_show_user_input;

    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();                        // 分割线

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    for(const auto& log : logs)
    {
        ImGui::TextUnformatted(log.c_str());
    }

    if(console_scroll_to_bottom) ImGui::SetScrollHereY(1.0f);
    console_scroll_to_bottom = false;

    ImGui::EndChild();

    ImGui::End();

    ImGui::PopFont();
}
