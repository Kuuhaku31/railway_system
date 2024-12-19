
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

    { // 标题
        ImGui::SetCursorPosY(5);
        ImGui::Text(TITLE);
        // 显示帧率
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SameLine();
        ImGui::Text("|  is processing data: %s", processing_data.id ? "true" : "false");
    }

    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();                        // 分割线 //=================================================================================

    { // 调整页码和每页显示数量
        // 设置宽度
        ImGui::PushItemWidth(150);

        uint32_t page_page_count = controller.ControllerGetPageCount();
        uint32_t page_item_count = controller.ControllerGetPageItemCount();
        uint32_t page_idx        = controller.ControllerGetPageIdx() + 1;

        ImGui::SameLine();
        if(ImGui::InputInt("Page Index", (int*)&page_idx))
        {
            if(page_idx < 1)
            {
                page_idx = 1;
            }
            else if(page_idx > page_page_count)
            {
                page_idx = page_page_count;
            }

            controller.ControllerChangePageIdx(page_idx - 1);
        }

        ImGui::SameLine();
        if(ImGui::InputInt("Page Item Count", (int*)&page_item_count))
        {
            if(page_item_count < 0) page_item_count = 0;

            controller.ControllerChangePageItemsCount(page_item_count);
        }

        ImGui::PopItemWidth();
    }

    { // 用户输入框
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

        float ticket_price = uint32_price_to_float(processing_data.ticket_price);
        ImGui::InputScalarN("Ticket Price", ImGuiDataType_Float, &ticket_price, 1, nullptr, nullptr, "%.2f");
        processing_data.ticket_price = float_to_uint32_price(ticket_price);

        // 下拉框选择车次状态
        ImGui::Text("Train Status");
        const char* items[] = { "NORMAL", "DELAY", "STOP", "CANCEL", "UNKNOWN" };
        ImGui::Combo("##Train Status", (int*)&processing_data.train_status, items, IM_ARRAYSIZE(items));
    }

    ImGui::Separator(); // 分割线 //=================================================================================

    { // 按钮
        // 插入按钮
        if(controller.ControllerUnableInsert())
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Insert", ImVec2(100, 0));
            ImGui::PopStyleColor(3);
        }
        else
        {
            controller.is_insert = ImGui::Button("Insert", ImVec2(100, 0));
        }

        // 删除按钮
        ImGui::SameLine();
        if(controller.ControllerUnableDel())
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Delete", ImVec2(100, 0));
            ImGui::PopStyleColor(3);
        }
        else
        {
            controller.is_del = ImGui::Button("Delete", ImVec2(100, 0));
        }

        // 更新按钮
        ImGui::SameLine();
        if(controller.ControllerUnableUpdate())
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Update", ImVec2(100, 0));
            ImGui::PopStyleColor(3);
        }
        else
        {
            controller.is_update = ImGui::Button("Update", ImVec2(100, 0));
        }

        // 清空按钮
        ImGui::SameLine();
        controller.is_clear_processing_data = ImGui::Button("Clear", ImVec2(100, 0));

        // 取消按钮
        ImGui::SameLine();
        controller.is_cancel = ImGui::Button("Cancel", ImVec2(100, 0));
    }

    ImGui::End();
    ImGui::PopFont();
}
