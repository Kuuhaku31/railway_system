
// main.cpp

// #include "controller.h"
#include "utils.h"
#include "view.h"

extern "C" {
#include "system_controller.h"
}


#include <stdio.h>

static ImGui_setup& imgui_setup = ImGui_setup::Instance();
static View&        view        = View::Instance();

extern uint8_t view_clear_color[4];

int
main()
{
    initConfig();
    initDb();
    printf("Hello, main!\n");

    SystemInitControler();
    view.ViewInit();

    printf("Showing view...\n");

    Event event;
    bool  is_running = true;
    while(is_running)
    {
        imgui_setup.On_frame_begin();

        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if(event.type == SDL_QUIT)
            {
                is_running = false;
            }
        }

        view.ViewShowWindows();

        Color clear_color = { view_clear_color[0], view_clear_color[1], view_clear_color[2], view_clear_color[3] };
        imgui_setup.On_frame_end(&clear_color);
    }

    view.ViewQuit();
    finalizeDb();
    return 0;
}
