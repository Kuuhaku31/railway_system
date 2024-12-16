
// view.h

#pragma once

#include "imgui_setup.h"

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
    bool is_show_demo_window = true;
    bool is_show_train_datas = true;

public:
    Color clear_color = { 0xcc, 0xcc, 0xcc, 0xff };

private:
    View() {}
    View(const View&)            = delete;
    View& operator=(const View&) = delete;

    static View* instance;
};
