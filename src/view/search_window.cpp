
// search_window.cpp

#include "view.h"

void
View::show_search_window(bool* p_open)
{
    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoTitleBar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    // window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushFont(font_chinese);
    ImGui::Begin("Search Window", p_open, window_flags);

    ImGui::Text("Search Window...");

    ImGui::End();
    ImGui::PopFont();
}
