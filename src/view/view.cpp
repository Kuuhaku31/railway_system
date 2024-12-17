
// view.cpp

#include "view.h"

#include "controler.h"

static ImGui_setup& imgui_setup = ImGui_setup::Instance();
static Controler&   controler   = Controler::Instance();

View* View::instance = nullptr;
View&
View::Instance()
{
    if(instance == nullptr) instance = new View();
    return *instance;
}

void
View::ViewInit()
{
    static Rect window_rect = { 30, 40, 1200, 900 };
    imgui_setup.Init("view", window_rect, false);

    // 加载默认字体
    ImGuiIO& io  = ImGui::GetIO();
    font_default = io.Fonts->AddFontDefault();

    // 加载中文字体（微软雅黑）
    font_chinese = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
}

void
View::ViewQuit()
{
    imgui_setup.Quit();
}

void
View::ViewShowWindows()
{
    update_view_layout();

    show_config_window();

    show_train_datas_window(&is_show_train_datas);

    if(is_show_demo_window) ImGui::ShowDemoWindow(&is_show_demo_window);

    show_user_input_window();

    ViewConsoleShowLog();
}

void
View::show_config_window(bool* p_open)
{
    // 如果 p_open 不为空，且 p_open 为 false，则返回
    if(p_open && !*p_open) return;

    ImGui::Begin("config", p_open);
    // 显示帧率
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Show Demo Window", &is_show_demo_window);
    ImGui::Checkbox("Show Train Datas", &is_show_train_datas);

    if(ImGui::Button("print user processing data"))
    {
        TrainData& train_data = controler.processing_data;

        printf("processing data: \n");
        printf("train_id: %d\n", train_data.train_id);
        printf("train_number: %s\n", train_data.train_number.c_str());
        printf("train_start_station: %s\n", train_data.train_start_station.c_str());
        printf("train_arrive_station: %s\n", train_data.train_arrive_station.c_str());
        printf("train_start_time: %s\n", date_to_string(train_data.train_start_time).c_str());
        printf("train_arrive_time: %s\n", date_to_string(train_data.train_arrive_time).c_str());
        printf("train_ticket_count: %d\n", train_data.train_ticket_count);
        printf("train_ticket_price: %.2f\n", train_data.train_ticket_price);
        printf("train_status: %s\n", parse_train_status(train_data.train_status).c_str());
    }

    // 输入一个整数
    static int number = 0;
    if(ImGui::InputInt("input number", &number))
    {
        controler.RailwaySystemSearchTrainData(number);
    }

    ImGui::End();
}

void
View::update_view_layout()
{
    // 应用程序显示大小
    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    // 数据窗口位置和大小
    data_window_pos  = ImVec2(0, 0);
    data_window_size = ImVec2(display_size.x, display_size.y * data_window_height);

    // 用户输入窗口位置和大小
    input_window_pos  = ImVec2(0, display_size.y * data_window_height);
    input_window_size = ImVec2(display_size.x * inuput_window_width, display_size.y * (1 - data_window_height));

    // 控制台窗口位置和大小
    console_window_pos  = ImVec2(display_size.x * inuput_window_width, display_size.y * data_window_height);
    console_window_size = ImVec2(display_size.x * (1 - inuput_window_width), display_size.y * (1 - data_window_height));
}
