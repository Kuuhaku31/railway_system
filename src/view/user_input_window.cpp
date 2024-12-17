
// user_input_window.cpp

// #include "train_controller.h"
#include "view.h"

#include "controller.h"

#include "date.h"

#define MAX_SIZE 128

void
DrawTimeInput(const char* label, Date* time)
{
    ImGui::Text(label);
    ImGui::PushItemWidth(100);
    // 输入年
    ImGui::InputScalar((std::string("年##") + label).c_str(), ImGuiDataType_U8, &time->year), ImGui::SameLine();
    // 输入月
    ImGui::InputScalar((std::string("月##") + label).c_str(), ImGuiDataType_U8, &time->month), ImGui::SameLine();
    // 输入日
    ImGui::InputScalar((std::string("日##") + label).c_str(), ImGuiDataType_U8, &time->day);
    // 输入小时
    ImGui::InputScalar((std::string("时##") + label).c_str(), ImGuiDataType_U8, &time->hour), ImGui::SameLine();
    // 输入分钟
    ImGui::InputScalar((std::string("分##") + label).c_str(), ImGuiDataType_U8, &time->minute), ImGui::SameLine();
    // 输入秒
    ImGui::InputScalar((std::string("秒##") + label).c_str(), ImGuiDataType_U8, &time->second);
    ImGui::PopItemWidth();
}

static Controller& controller = Controller::Instance();

void
View::show_user_input_window(bool* p_open)
{
    // 设置按钮的颜色
    static ImVec4 normal_color  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f); // 正常颜色
    static ImVec4 hovered_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // 悬停颜色
    static ImVec4 active_color  = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 按下颜色
    static ImVec4 disable_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 禁用颜色

    static bool unable_insert = false;
    static bool unable_del    = false;
    static bool unable_update = true;

    if(p_open && !*p_open) return;

    bool is_insert = false;
    bool is_del    = false;
    bool is_update = false;

    if(is_selected_new) // 如果选中了新的车次
    {
        is_selected_new = false;

        controller.Getdata();

        // 如果选中了新的车次，将该车次的数据显示在输入框中
        if(controller.SelectTrainData(selected_id))
        {
            // 如果存在该车次，禁用插入按钮，启用删除和更新按钮
            unable_insert = true;
            unable_del    = false;
            unable_update = false;

            table_to_selected = true;
        }
        else
        {
            // 如果不存在该车次，启用插入按钮，禁用删除和更新按钮
            unable_insert = false;
            unable_del    = true;
            unable_update = true;
        }
    }

    // 引用控制器中的数据
    TrainData& train_data = controller.processing_data;

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushFont(font_chinese); // 使用中文字体
    ImGui::SetNextWindowPos(input_window_pos);
    ImGui::SetNextWindowSize(input_window_size);
    ImGui::Begin("Input Text", p_open, window_flags);
    ImGui::Text("processing data:");

    // 如果选中了某个车次，将该车次的数据显示在输入框中
    if(ImGui::InputScalar("Train ID", ImGuiDataType_U32, &train_data.id))
    {
        is_selected_new = true;
    }
    ImGui::InputText("Train Number", train_data.number, MAX_SIZE);

    ImGui::InputText("Start Station", train_data.start_station, MAX_SIZE);
    ImGui::InputText("Arrive Station", train_data.arrive_station, MAX_SIZE);

    // 出发时间
    Date start_time = uint64_time_to_date(train_data.start_time);
    DrawTimeInput("Start Time", &start_time);
    train_data.start_time = date_to_uint64_time(start_time);
    // 到达时间
    Date arrive_time = uint64_time_to_date(train_data.arrive_time);
    DrawTimeInput("Arrive Time", &arrive_time);
    train_data.arrive_time = date_to_uint64_time(arrive_time);

    ImGui::InputScalar("Ticket Count", ImGuiDataType_U32, &train_data.ticket_remain);
    ImGui::InputScalar("Ticket Price", ImGuiDataType_Float, &train_data.ticket_price);

    // 下拉框选择车次状态
    ImGui::Text("Train Status");
    const char* items[] = { "NORMAL", "DELAY", "STOP", "CANCEL", "OTHER" };
    ImGui::Combo("##Train Status", (int*)&train_data.train_status, items, IM_ARRAYSIZE(items));

    // 分割线
    ImGui::Separator();

    // 按钮
    if(unable_insert)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);

        ImGui::Button("Insert", ImVec2(100, 0));

        ImGui::PopStyleColor(3);
    }
    else
    {
        is_insert = ImGui::Button("Insert", ImVec2(100, 0));
    }

    ImGui::SameLine();

    if(unable_del)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);

        ImGui::Button("Delete", ImVec2(100, 0));

        ImGui::PopStyleColor(3);
    }
    else
    {
        is_del = ImGui::Button("Delete", ImVec2(100, 0));
    }

    ImGui::SameLine();

    if(unable_update)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);

        ImGui::Button("Update", ImVec2(100, 0));

        ImGui::PopStyleColor(3);
    }
    else
    {
        is_update = ImGui::Button("Update", ImVec2(100, 0));
    }

    ImGui::End();
    ImGui::PopFont();

    selected_id = train_data.id;

    if(is_insert)
    {
        controller.RailwaySystemInsertTrainData();

        unable_insert = true;
        unable_del    = false;
        unable_update = false;

        is_selected_new = true;

        // 日志
        add_log("Insert: ", train_data);
        console_scroll_to_bottom = true;
    }

    if(is_del)
    {
        controller.RailwaySystemDelTrainData(selected_id);

        unable_insert = false;
        unable_del    = true;
        unable_update = true;

        is_selected_new = true;

        // 日志
        add_log("Delete: ", train_data);
        console_scroll_to_bottom = true;
    }

    if(is_update)
    {
        controller.RailwaySystemInsertTrainData();

        is_selected_new = true;

        // 日志
        add_log("Update: ", train_data);
        console_scroll_to_bottom = true;
    }
}
