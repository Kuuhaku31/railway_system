
// console_window.cpp

#include "view.h"

void
View::ViewConsoleAddLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);
    logs.push_back(std::string(buf));
}

void
View::ViewConsoleShowLog(bool* p_open)
{
    ImVec2 display_size = ImGui::GetIO().DisplaySize;
    ImVec2 window_pos;
    ImVec2 window_size = display_size;

    window_pos.x = display_size.x * inuput_window_width;
    window_pos.y = display_size.y * data_window_height;

    window_size.x *= (1 - inuput_window_width);
    window_size.y *= (1 - data_window_height);

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowSize(window_size);

    // 中文显示
    ImGui::PushFont(font_chinese);
    ImGui::Begin("log window", p_open, window_flags);

    if(ImGui::Button("Clear")) logs.clear();

    ImGui::SameLine();
    bool copy_to_clipboard = ImGui::Button("Copy");

    ImGui::SameLine();
    console_scroll_to_bottom = ImGui::Button("Scroll to bottom");

    ImGui::Separator(); // 分割线

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if(copy_to_clipboard)
    {
        ImGui::LogToClipboard();
    }
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

void
View::ViewScrollToBottom()
{
    console_scroll_to_bottom = true;
}
