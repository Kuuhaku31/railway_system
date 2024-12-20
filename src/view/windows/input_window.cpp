
// input_window.cpp

#include "view.h"

// #include "controller.h"
#include "date.h"
extern "C" {
#include "system_controller.h"
}

#define MAX_SIZE 128

extern bool system_is_insert;
extern bool system_is_del;
extern bool system_is_update;
extern bool system_is_cancel;
extern bool system_is_fresh_processing_data;

extern TrainData system_processing_data;

void
View::show_user_input_window(bool* p_open)
{
    static bool is_inserting = false;

    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
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
        ImGui::Text("|  is processing data: %s", system_processing_data.id ? "true" : "false");
        ImGui::SameLine();
        ImGui::Text("%d datas in buffer", SystemControllerGetPageItemCountCurrent());
    }

    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();                        // 分割线 //=================================================================================

    { // 调整页码和每页显示数量
        // 设置宽度
        ImGui::PushItemWidth(150);

        uint32_t page_page_count = SystemControllerGetPageCount();     // 总页数
        uint32_t page_item_count = SystemControllerGetPageItemCount(); // 每页期望显示的数据数量
        int      page_idx_curr   = SystemControllerGetPageIdx();       // 当前页数

        if(ImGui::InputInt("Page Index", &page_idx_curr))
        {
            if(page_idx_curr < 1)
            {
                page_idx_curr = 1;
            }
            else if(page_idx_curr > page_page_count)
            {
                page_idx_curr = page_page_count;
            }

            SystemControllerChangePageIdx(page_idx_curr);
        }
        ImGui::SameLine();
        ImGui::Text("/ %d", page_page_count);

        ImGui::SameLine();
        int new_page_item_count = page_item_count;
        if(ImGui::InputInt("Page Item Count", &new_page_item_count))
        {
            SystemControllerChangePageItemsCount(new_page_item_count);
        }

        ImGui::PopItemWidth();
    }

    // Train ID 输入框
    if(is_inserting)
    {
        static char text[] = "You Are Inserting Data";
        ImGui::BeginDisabled(true);
        ImGui::InputText("Train ID", text, sizeof(text), ImGuiInputTextFlags_ReadOnly);
        ImGui::EndDisabled();
    }
    else if(ImGui::InputScalar("Train ID", ImGuiDataType_U32, &system_processing_data.id))
    {
        system_is_fresh_processing_data = true;
    }

    ImGui::SameLine();
    ImGui::Checkbox("Inserting", &is_inserting);

    // 如果当前正在编辑的数据不在缓存中，禁用用户输入框
    ImGui::BeginDisabled(!is_inserting && !SystemControllerIsDataInBuffer());
    ImGui::Separator(); // 分割线 //=================================================================================

    { // 用户输入框
        ImGui::InputText("Train Number", system_processing_data.number, MAX_SIZE);

        ImGui::InputText("Start Station", system_processing_data.start_station, MAX_SIZE);
        ImGui::InputText("Arrive Station", system_processing_data.arrive_station, MAX_SIZE);

        // 出发时间
        Date start_time = uint64_time_to_date(system_processing_data.start_time);
        InputTime("Start Time", &start_time);
        system_processing_data.start_time = date_to_uint64_time(start_time);
        // 到达时间
        Date arrive_time = uint64_time_to_date(system_processing_data.arrive_time);
        InputTime("Arrive Time", &arrive_time);
        system_processing_data.arrive_time = date_to_uint64_time(arrive_time);

        // 票数
        ImGui::InputScalar("Ticket Count", ImGuiDataType_U32, &system_processing_data.ticket_remain);

        // 票价
        float ticket_price = uint32_price_to_float(system_processing_data.ticket_price);
        ImGui::InputScalarN("Ticket Price", ImGuiDataType_Float, &ticket_price, 1, nullptr, nullptr, "%.2f");
        system_processing_data.ticket_price = float_to_uint32_price(ticket_price);

        // 下拉框选择车次状态
        ImGui::Text("Train Status");
        const char* items[] = { "NORMAL", "DELAY", "STOP", "CANCEL", "UNKNOWN" };
        ImGui::Combo("##Train Status", (int*)&system_processing_data.train_status, items, IM_ARRAYSIZE(items));
    }

    ImGui::Separator(); // 分割线 //=================================================================================

    { // 按钮
        // 插入按钮
        // if(controller.ControllerUnableInsert())
        if(!is_inserting)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Insert", ImVec2(100, 0));
            ImGui::PopStyleColor(3);
        }
        else
        {
            system_is_insert = ImGui::Button("Insert", ImVec2(100, 0));
        }

        // 删除按钮
        ImGui::SameLine();
        // if(controller.ControllerUnableDel())
        if(is_inserting || !system_processing_data.id)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Delete", ImVec2(100, 0));
            ImGui::PopStyleColor(3);
        }
        else
        {
            system_is_del = ImGui::Button("Delete", ImVec2(100, 0));
        }

        // 更新按钮
        ImGui::SameLine();
        // if(controller.ControllerUnableUpdate())
        if(is_inserting || !system_processing_data.id)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Update", ImVec2(100, 0));
            ImGui::PopStyleColor(3);
        }
        else
        {
            system_is_update = ImGui::Button("Update", ImVec2(100, 0));
        }

        // 清空按钮
        // ImGui::SameLine();
        // controller.is_clear_processing_data = ImGui::Button("Clear", ImVec2(100, 0));
    }

    ImGui::EndDisabled();

    // 取消按钮
    ImGui::SameLine();
    // 调整元素外边距
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    system_is_cancel = ImGui::Button("Cancel", ImVec2(150, 0));

    ImGui::End();
    ImGui::PopFont();
}
