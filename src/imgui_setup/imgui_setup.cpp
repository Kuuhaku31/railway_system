
// imgui_seutp.cpp

#include "imgui_setup.h"

#define DEFAULT_CLEAR_COLOR 0x0, 0x0, 0x0, 0xFF

ImGui_setup* ImGui_setup::instance = nullptr;

ImGui_setup&
ImGui_setup::Instance()
{
    if(instance == nullptr) instance = new ImGui_setup();
    return *instance;
}

int
ImGui_setup::Init(const std::string& window_title, const Rect& window_rect, bool is_fullscreen)
{
    if(!init_flag) return 0;

    // Setup SDL
    init_flag = SDL_Init(SDL_INIT_EVERYTHING);
    if(init_flag != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return init_flag;
    }

    // IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    // Mix_Init(MIX_INIT_MP3);
    // TTF_Init();

    // Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // 打开音频设备

    // From 2.0.18: Enable native IME.
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if(is_fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(
        window_title.c_str(),
        window_rect.x,
        window_rect.y,
        window_rect.w,
        window_rect.h,
        flags

    );

    window_rect_before_fullscreen = window_rect;

    if(window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        init_flag = -1;
        return init_flag;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        init_flag = -1;
        return init_flag;
    }


    // 初始化 Dear ImGui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return init_flag;
}

int
ImGui_setup::Quit()
{
    if(init_flag) return 1;

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Mix_CloseAudio(); // 关闭音频设备

    // TTF_Quit(); // 退出TTF
    // Mix_Quit(); // 退出Mixer
    // IMG_Quit(); // 退出IMG
    SDL_Quit(); // 退出SDL

    return 0;
}


void
ImGui_setup::On_frame_begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
}

void
ImGui_setup::On_frame_end(const Color* clear_color)
{
    // Rendering
    ImGui::Render();

    SDL_RenderSetScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);

    // 清空屏幕
    if(clear_color)
        SDL_SetRenderDrawColor(renderer, clear_color->r, clear_color->g, clear_color->b, clear_color->a);
    else
        SDL_SetRenderDrawColor(renderer, DEFAULT_CLEAR_COLOR);
    SDL_RenderClear(renderer);

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}

void
ImGui_setup::Enter_fullscreen()
{
    SDL_GetWindowPosition(window, &window_rect_before_fullscreen.x, &window_rect_before_fullscreen.y);
    SDL_GetWindowSize(window, &window_rect_before_fullscreen.w, &window_rect_before_fullscreen.h);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void
ImGui_setup::Exit_fullscreen()
{
    SDL_SetWindowFullscreen(window, 0);

    SDL_SetWindowSize(window, window_rect_before_fullscreen.w, window_rect_before_fullscreen.h);
    SDL_SetWindowPosition(window, window_rect_before_fullscreen.x, window_rect_before_fullscreen.y);
}

bool
ImGui_setup::Make_message_box(const char* title, const char* message) const
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, window);
}
