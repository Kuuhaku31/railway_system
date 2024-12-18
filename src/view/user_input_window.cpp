
// user_input_window.cpp

// #include "train_controller.h"
#include "view.h"

#include "controller.h"

#include "date.h"

#define MAX_SIZE 128

#define TITLE "Railway System Application Made By: Kuuhaku-kazari & Mike, 2024.12"

static Controller& controller      = Controller::Instance();
static TrainData&  processing_data = controller.processing_data; // 引用控制器中的数据

// 设置按钮的颜色
static ImVec4 normal_color  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f); // 正常颜色
static ImVec4 hovered_color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // 悬停颜色
static ImVec4 active_color  = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 按下颜色
static ImVec4 disable_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // 禁用颜色

void
View::show_user_input_window(bool* p_open)
{
    if(p_open && !*p_open) return;

    bool is_insert = false;
    bool is_del    = false;
    bool is_update = false;
    bool is_clear  = false;
    bool is_cancel = false;

    if(controller.is_fresh_processing_data) // 如果选中了新的车次
    {
        controller.is_fresh_processing_data = false;

        // 如果选中了新的车次，将该车次的数据显示在输入框中
        if(controller.UpdateProcessingData())
        {
            // 如果存在该车次，禁用插入按钮，启用删除和更新按钮
            controller.unable_insert = true;
            controller.unable_del    = false;
            controller.unable_update = false;

            table_to_selected = true;
        }
        else
        {
            // 如果不存在该车次，启用插入按钮，禁用删除和更新按钮
            controller.unable_insert = false;
            controller.unable_del    = true;
            controller.unable_update = true;
        }
    }
    else if(!processing_data.id) // 如果没有选中车次
    {
        // 如果没有选中车次
        controller.unable_insert = true;
        controller.unable_del    = true;
        controller.unable_update = true;
    }

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushFont(font_chinese); // 使用中文字体
    ImGui::SetNextWindowPos(input_window_pos);
    ImGui::SetNextWindowSize(input_window_size);
    ImGui::Begin("User Input", p_open, window_flags);

    ImGui::SetCursorPosY(5);
    ImGui::Text(TITLE);
    // 显示帧率
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text("|  is processing data: %s", processing_data.id ? "true" : "false");

    // 分割线 //=================================================================================
    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();

    // Train ID 输入框
    if(ImGui::InputScalar("Train ID", ImGuiDataType_U32, &processing_data.id))
    {
        controller.is_fresh_processing_data = true;
    }

    ImGui::InputText("Train Number", processing_data.number, MAX_SIZE);

    ImGui::InputText("Start Station", processing_data.start_station, MAX_SIZE);
    ImGui::InputText("Arrive Station", processing_data.arrive_station, MAX_SIZE);

    // 出发时间
    Date start_time = uint64_time_to_date(processing_data.start_time);
    InputTime("Start Time", &start_time);
    processing_data.start_time = date_to_uint64_time(start_time);
    // 到达时间
    Date arrive_time = uint64_time_to_date(processing_data.arrive_time);
    InputTime("Arrive Time", &arrive_time);
    processing_data.arrive_time = date_to_uint64_time(arrive_time);

    ImGui::InputScalar("Ticket Count", ImGuiDataType_U32, &processing_data.ticket_remain);
    ImGui::InputScalar("Ticket Price", ImGuiDataType_Float, &processing_data.ticket_price);

    // 下拉框选择车次状态
    ImGui::Text("Train Status");
    const char* items[] = { "NORMAL", "DELAY", "STOP", "CANCEL", "UNKNOWN" };
    ImGui::Combo("##Train Status", (int*)&processing_data.train_status, items, IM_ARRAYSIZE(items));

    // 分割线 //=================================================================================
    ImGui::Separator();

    // 按钮
    // 插入按钮
    if(controller.unable_insert)
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

    // 删除按钮
    ImGui::SameLine();
    if(controller.unable_del)
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

    // 更新按钮
    ImGui::SameLine();
    if(controller.unable_update)
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

    // 清空按钮
    ImGui::SameLine();
    is_clear = ImGui::Button("Clear", ImVec2(100, 0));

    // 取消按钮
    ImGui::SameLine();
    is_cancel = ImGui::Button("Cancel", ImVec2(100, 0));

    ImGui::End();
    ImGui::PopFont();

    if(is_cancel)
    {
        processing_data.id = 0;
        is_show_user_input = false;
    }
    else if(is_insert)
    {
        controller.InsertData();

        controller.unable_insert = true;
        controller.unable_del    = false;
        controller.unable_update = false;

        controller.ControllerFreshProcessingData();

        // 日志
        add_train_data_log("Insert: ", processing_data);
        console_scroll_to_bottom = true;
    }
    else if(is_del)
    {
        controller.UpdateProcessingData();
        controller.DeleteData();

        controller.unable_insert = false;
        controller.unable_del    = true;
        controller.unable_update = true;

        controller.ControllerFreshProcessingData();

        // 日志
        add_train_data_log("Delete: ", processing_data);
        console_scroll_to_bottom = true;
    }
    else if(is_update)
    {
        controller.UpdateData();

        controller.ControllerFreshProcessingData();

        // 日志
        add_train_data_log("Update: ", processing_data);
        console_scroll_to_bottom = true;
    }
    else if(is_clear)
    {
        controller.ClearProcessingData();

        controller.unable_insert = true;
        controller.unable_del    = true;
        controller.unable_update = true;
    }
}
