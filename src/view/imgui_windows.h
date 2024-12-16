
// imgui_windows.h

#pragma once

#include <string>

// config window
void
ImGuiWindowConfig(bool* p_open = nullptr);

// 显示车次信息
void
ImGuiWindowTrainDatas(bool* p_open = nullptr);

// 获取用户输入的字符串
void
ImGuiInputText(bool* p_open = nullptr);
