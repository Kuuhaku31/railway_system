
// view.cpp

#include "view.h"

#include "controller.h"

#include "date.h"

static ImGui_setup& imgui_setup = ImGui_setup::Instance();
static Controller&  controller  = Controller::Instance();

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
    static Rect window_rect = { 30, 40, 1100, 1400 };
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
    controller.ControllerUpdate();

    // is_show_user_input = (bool)controller.processing_data.id;

    update_view_layout();

    show_config_window();

    show_train_datas_window(&is_show_train_datas);

    if(is_show_demo_window) ImGui::ShowDemoWindow(&is_show_demo_window);

    show_user_input_window(&is_show_user_input);

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
        TrainData& train_data = controller.processing_data;

        printf("processing data: \n");
        printf("train_id: %d\n", train_data.id);
        printf("ticket_remain: %d\n", train_data.ticket_remain);
        printf("ticket_price: %.2f\n", train_data.ticket_price);
        printf("start_time: %s\n", date_to_string(uint64_time_to_date(train_data.start_time)).c_str());
        printf("arrive_time: %s\n", date_to_string(uint64_time_to_date(train_data.arrive_time)).c_str());
        printf("number: %s\n", train_data.number);
        printf("start_station: %s\n", train_data.start_station);
        printf("arrive_station: %s\n", train_data.arrive_station);
        printf("train_status: %s\n", parse_train_status(train_data.train_status).c_str());
    }

    // 读取数据库
    if(ImGui::Button("Get datas"))
    {
        controller.Getdatas();
    }

    ImGui::End();
}

void
View::update_view_layout()
{
    // 应用程序显示大小
    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    // 数据窗口位置和大小
    data_window_pos    = ImVec2(0, 0);
    data_window_size.x = display_size.x;
    data_window_size.y = (is_show_user_input || is_show_console) ? display_size.y * data_window_height : display_size.y;

    // 用户输入窗口位置和大小
    input_window_pos    = ImVec2(0, display_size.y * data_window_height);
    input_window_size.x = is_show_console ? display_size.x * inuput_window_width : display_size.x;
    input_window_size.y = display_size.y * (1 - data_window_height);

    // 控制台窗口位置和大小
    console_window_pos.x  = is_show_user_input ? display_size.x * inuput_window_width : 0;
    console_window_pos.y  = display_size.y * data_window_height;
    console_window_size.x = is_show_user_input ? display_size.x * (1 - inuput_window_width) : display_size.x;
    console_window_size.y = display_size.y * (1 - data_window_height);
}

void
View::ViewConsoleAddLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);
    logs.push_back(std::string(buf));
}


void
View::add_train_data_log(std::string label, const TrainData& train_data)
{
    std::string log =
        label +
        std::to_string(train_data.id) + " " +
        std::string(train_data.number) + " " +
        std::string(train_data.start_station) + " " +
        std::string(train_data.arrive_station) + " " +
        date_to_string(uint64_time_to_date(train_data.start_time)) + " " +
        date_to_string(uint64_time_to_date(train_data.arrive_time)) + " " +
        std::to_string(train_data.ticket_remain) + " " +
        std::to_string(train_data.ticket_price) + " " +
        parse_train_status(train_data.train_status);
    ViewConsoleAddLog(log.c_str());
}
