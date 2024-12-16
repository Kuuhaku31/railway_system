
#include "controler.h"
#include "view.h"


static ImGui_setup& imgui_setup = ImGui_setup::Instance();

static View&      view      = View::Instance();
static Controler& controler = Controler::Instance();

TrainData train_datas[10];

void
get_train_datas(Controler::TrainDatas* view_train_datas)
{
    train_datas[0] = { 1, "G1234", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[1] = { 2, "G1235", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[2] = { 3, "G1236", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[3] = { 4, "G1237", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[4] = { 5, "G1238", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[5] = { 6, "G1239", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[6] = { 7, "G1240", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[7] = { 8, "G1241", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[8] = { 9, "G1242", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };
    train_datas[9] = { 10, "G1243", "北京", "上海", { 2021, 1, 1, 8, 0, 0 }, { 2021, 1, 1, 12, 0, 0 }, 100, 100.0f, TrainStatus::DELAY };

    for(auto& train_data : train_datas)
    {
        view_train_datas->push_back(&train_data);
    }
}

int
main()
{
    // utf8
    SetConsoleOutputCP(CP_UTF8);

    printf("Hello, test!\n");

    get_train_datas(&controler.train_datas);

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
