
#include "view.h"

static ImGui_setup& imgui_setup = ImGui_setup::Instance();

static View& view = View::Instance();

TrainData train_datas[10];

void
get_train_datas(View::TrainDatas* view_train_datas)
{
    train_datas[0] = { 1, 100, 100.0, 1000, 2000, "G1001", "Beijing", "Shanghai", true };
    train_datas[1] = { 2, 200, 200.0, 2000, 3000, "G1002", "Shanghai", "Beijing", true };
    train_datas[2] = { 3, 300, 300.0, 3000, 4000, "G1003", "Beijing", "Shanghai", true };
    train_datas[3] = { 4, 400, 400.0, 4000, 5000, "G1004", "Shanghai", "Beijing", true };
    train_datas[4] = { 5, 500, 500.0, 5000, 6000, "G1005", "Beijing", "Shanghai", true };
    train_datas[5] = { 6, 600, 600.0, 6000, 7000, "G1006", "Shanghai", "Beijing", true };
    train_datas[6] = { 7, 700, 700.0, 7000, 8000, "G1007", "Beijing", "Shanghai", true };
    train_datas[7] = { 8, 800, 800.0, 8000, 9000, "G1008", "Shanghai", "Beijing", true };
    train_datas[8] = { 9, 900, 900.0, 9000, 10000, "G1009", "Beijing", "Shanghai", true };
    train_datas[9] = { 10, 1000, 1000.0, 10000, 11000, "G1010", "Shanghai", "Beijing", true };


    for(auto& train_data : train_datas)
    {
        view_train_datas->push_back(&train_data);
    }
}

int
main()
{
    printf("Hello, test!\n");

    get_train_datas(&view.train_datas);

    view.ViewInit();

    printf("Showing view...\n");

    Event event;

    bool is_running = true;
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

        imgui_setup.On_frame_end(&view.clear_color);
    }

    view.ViewQuit();

    return 0;
}
