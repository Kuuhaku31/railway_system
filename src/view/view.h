
// view.h

#pragma once

#include "imgui_setup.h"

#include "entity.h"

#include <vector>

// 单例模式
class View
{
public:
    static View& Instance();

public:
    void ViewInit();
    void ViewQuit();

    void ViewShowWindows();

public:
    bool is_show_demo_window = false;
    bool is_show_train_datas = true;

public:
    int selected_id = -1;

public:
    Color clear_color = { 0x33, 0x33, 0x33, 0xff };

public:
    ImFont* font_default = nullptr;
    ImFont* font_chinese = nullptr;

private:
    View() {}
    View(const View&)            = delete;
    View& operator=(const View&) = delete;

    static View* instance;
};
