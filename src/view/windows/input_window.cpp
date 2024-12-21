
// input_window.cpp

#include "view.h"

extern "C" {
#include "system_controller.h"
}

#include "date.h"
#include "util_funcs.h"

#define MAX_SIZE 128

extern bool system_is_insert;
extern bool system_is_del;
extern bool system_is_update;
extern bool system_is_cancel;
extern bool system_is_clear_processing_data;
extern bool system_is_fresh_processing_data;

extern TrainData system_processing_data;

// 窗口参数
extern const WindowRect view_input_window_rect;

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
    ImGui::SetNextWindowPos(get_rect_pos(view_input_window_rect));
    ImGui::SetNextWindowSize(get_rect_size(view_input_window_rect));
    ImGui::Begin(INPUT_WINDOW_TITLE, p_open, window_flags);

    ImGui::Indent(10); // 缩进
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

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

    ImGui::Unindent(10); // 取消缩进

    ImGui::SetCursorPosY(first_separator_pos); // 调整光标位置
    ImGui::Separator();                        // 分割线 //=================================================================================
    ImGui::Indent(10);                         // 缩进
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    { // 调整页码和每页显示数量
        // 设置宽度
        ImGui::PushItemWidth(100);

        uint32_t page_page_count = SystemControllerGetPageCount();     // 总页数
        uint32_t page_item_count = SystemControllerGetPageItemCount(); // 每页期望显示的数据数量
        int      page_idx_curr   = SystemControllerGetPageIdx();       // 当前页数

        if(ImGui::InputInt("##Page Index", &page_idx_curr))
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
        ImGui::Text("/ %d Page", page_page_count);

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
        ImGui::InputText(INPUT_WINDOW_LABEL_TRAIN_ID, text, sizeof(text), ImGuiInputTextFlags_ReadOnly);
        ImGui::EndDisabled();
    }
    else if(ImGui::InputScalar(INPUT_WINDOW_LABEL_TRAIN_ID, ImGuiDataType_U32, &system_processing_data.id))
    {
        system_is_fresh_processing_data = true;
    }

    ImGui::SameLine();
    ImGui::Checkbox("Inserting", &is_inserting);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Unindent(10);
    ImGui::Separator(); // 分割线 //=================================================================================
    ImGui::Indent(10);  // 缩进
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    // 如果当前正在编辑的数据不在缓存中，禁用以下输入框
    ImGui::BeginDisabled(!is_inserting && !SystemControllerIsDataInBuffer());

    { // 用户输入框
        ImGui::InputText(INPUT_WINDOW_TITLE_TRAIN_NUMBER, system_processing_data.number, MAX_SIZE);

        ImGui::InputText(INPUT_WINDOW_TITLE_START_STATION, system_processing_data.start_station, MAX_SIZE);
        ImGui::InputText(INPUT_WINDOW_TITLE_ARRIVE_STATION, system_processing_data.arrive_station, MAX_SIZE);

        // 出发时间
        static Date start_time = uint64_time_to_date(system_processing_data.start_time);
        InputTime(INPUT_WINDOW_TITLE_START_TIME, &start_time);
        system_processing_data.start_time = date_to_uint64_time(start_time);

        // 到达时间
        static Date arrive_time = uint64_time_to_date(system_processing_data.arrive_time);
        InputTime(INPUT_WINDOW_TITLE_ARRIVE_TIME, &arrive_time);
        system_processing_data.arrive_time = date_to_uint64_time(arrive_time);

        // 票数
        ImGui::InputScalar(INPUT_WINDOW_TITLE_TICKET_REMAIN, ImGuiDataType_U32, &system_processing_data.ticket_remain);

        // 票价
        float ticket_price = uint32_price_to_float(system_processing_data.ticket_price);
        ImGui::InputScalarN(INPUT_WINDOW_TITLE_TICKET_PRICE, ImGuiDataType_Float, &ticket_price, 1, nullptr, nullptr, "%.2f");
        system_processing_data.ticket_price = float_to_uint32_price(ticket_price);

        // 下拉框选择车次状态
        static const char* items[] = {
            TRAIN_STATUS_NORMAL_TEXT,
            TRAIN_STATUS_DELAYED_TEXT,
            TRAIN_STATUS_STOPPED_TEXT,
            TRAIN_STATUS_CANCELLED_TEXT,
            TRAIN_STATUS_UNKNOWN_TEXT
        };
        static int item_current = 0;

        switch(system_processing_data.train_status)
        {
        case TRAIN_STATUS_NORMAL: item_current = 0; break;
        case TRAIN_STATUS_DELAYED: item_current = 1; break;
        case TRAIN_STATUS_STOPPED: item_current = 2; break;
        case TRAIN_STATUS_CANCELLED: item_current = 3; break;
        default:
        case TRAIN_STATUS_UNKNOWN: item_current = 4; break;
        }
        ImGui::Combo(INPUT_WINDOW_LABEL_TRAIN_STATUS, (int*)&item_current, items, IM_ARRAYSIZE(items));
        ImGui::SameLine();
        ImGui::Text(INPUT_WINDOW_TITLE_TRAIN_STATUS);
        switch(item_current)
        {
        case 0: system_processing_data.train_status = TRAIN_STATUS_NORMAL; break;
        case 1: system_processing_data.train_status = TRAIN_STATUS_DELAYED; break;
        case 2: system_processing_data.train_status = TRAIN_STATUS_STOPPED; break;
        case 3: system_processing_data.train_status = TRAIN_STATUS_CANCELLED; break;
        default:
        case 4: system_processing_data.train_status = TRAIN_STATUS_UNKNOWN; break;
        }
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Unindent(10);
    ImGui::Separator(); // 分割线 //=================================================================================
    ImGui::Indent(10);  // 缩进
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    { // 按钮
        // 插入按钮
        if(!is_inserting)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Insert", ImVec2(100, 35));
            ImGui::PopStyleColor(3);
        }
        else
        {
            system_is_insert |= ImGui::Button("Insert", ImVec2(100, 35));
        }

        // 删除按钮
        ImGui::SameLine();
        if(is_inserting || !system_processing_data.id)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Delete", ImVec2(100, 35));
            ImGui::PopStyleColor(3);
        }
        else
        {
            system_is_del |= ImGui::Button("Delete", ImVec2(100, 35));
        }

        // 更新按钮
        ImGui::SameLine();
        if(is_inserting || !system_processing_data.id)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disable_color);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, disable_color);
            ImGui::Button("Update", ImVec2(100, 35));
            ImGui::PopStyleColor(3);
        }
        else
        {
            system_is_update |= ImGui::Button("Update", ImVec2(100, 35));
        }

        // 清空按钮
        ImGui::SameLine();
        system_is_clear_processing_data |= ImGui::Button("Clear", ImVec2(100, 35));
    }

    ImGui::EndDisabled();

    // 取消按钮
    ImGui::SameLine();
    // 调整元素外边距
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    system_is_cancel = ImGui::Button("Cancel", ImVec2(150, 35));

    ImGui::End();
    ImGui::PopFont();
}
