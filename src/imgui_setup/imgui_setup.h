
#pragma once

#define SDL_MAIN_HANDLED

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_ttf.h>
// #include <SDL2_gfxPrimitives.h>

#include <string>
#include <windows.h>

typedef SDL_Rect Rect;
typedef struct
{
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;
} Color;

typedef SDL_Event Event;

class ImGui_setup
{
public:
    static ImGui_setup& Instance();

public:
    ImGuiIO*      io       = nullptr;
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;

public:
    Rect window_rect_before_fullscreen; // 全屏前窗口大小

public:
    int Init(const std::string& window_title = "window_title", const Rect& window_rect = { 30, 40, 800, 600 }, bool fullscreen = false); // 0:初始化成功
    int Quit();

    void On_frame_begin();
    void On_frame_end(const Color* clear_color = nullptr);

public:
    void Enter_fullscreen();
    void Exit_fullscreen();

public:
    int Get_window_id() const { return SDL_GetWindowID(window); }

public:
    bool Make_message_box(const char* title, const char* message) const;

private:
    int init_flag = 1;

private:
    ImGui_setup()                              = default;
    ~ImGui_setup()                             = default;
    ImGui_setup(const ImGui_setup&)            = delete;
    ImGui_setup& operator=(const ImGui_setup&) = delete;

    static ImGui_setup* instance;
};
