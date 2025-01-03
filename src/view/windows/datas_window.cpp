
// datas_window.cpp

#include "view.h"

extern "C" {
#include "system_controller.h"
}

#include "util_funcs.h"

extern bool system_is_fresh_processing_data;

extern TrainData system_processing_data;

extern bool view_is_show_user_input;
extern bool view_table_to_selected;

extern const WindowRect view_data_window_rect;

void
View::show_train_datas_window(bool* p_open)
{
    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;            // 不显示标题栏
    window_flags |= ImGuiWindowFlags_NoResize;              // 无法调整宽度
    window_flags |= ImGuiWindowFlags_NoMove;                // 无法移动
    window_flags |= ImGuiWindowFlags_NoScrollbar;           // 没有滚动条
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;     // 无法滚动
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; // 保持在最后

    ImGui::PushFont(font_chinese); // 中文显示
    ImGui::SetNextWindowPos(get_rect_pos(view_data_window_rect));
    ImGui::SetNextWindowSize(get_rect_size(view_data_window_rect));
    ImGui::Begin("Train Datas", p_open, window_flags);

    // 临时修改样式变量，取消表格和窗口的间距
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    // 第一个元素位置为(0, 0)
    ImGui::SetCursorPos(ImVec2(0, 0));

    const TrainData* train_datas = SystemControllerGetTrainDatas();

    // 表格显示车次信息
    uint32_t table_flags = 0;

    table_flags |= ImGuiTableFlags_Resizable; // 可调整列宽
    table_flags |= ImGuiTableFlags_RowBg;     // 行背景
    table_flags |= ImGuiTableFlags_Borders;   // 显示边框
    table_flags |= ImGuiTableFlags_ScrollY;   // 纵向滚动
    table_flags |= ImGuiTableFlags_ScrollX;   // 横向滚动

    ImVec2 outer_size = ImGui::GetWindowSize();
    if(ImGui::BeginTable("TrainDatas", 10, table_flags, outer_size))
    {
        // 冻结表头
        ImGui::TableSetupScrollFreeze(0, 1);

        ImGui::TableSetupColumn("No.", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Train ID", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Train Number", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Start Station", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Arrive Station", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Start Time", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Arrive Time", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Ticket Count", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Ticket Price", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Train Status", ImGuiTableColumnFlags_WidthFixed, 100.0f);

        ImGui::TableHeadersRow();


        for(int index = 0; index < SystemControllerGetPageItemCountCurrent(); index++)
        {
            const TrainData& train_data = train_datas[index];

            ImGui::TableNextRow();

            // 如果是选中的行
            if(system_processing_data.id && system_processing_data.id == train_data.id && view_table_to_selected)
            {
                ImGui::SetScrollHereY();
                view_table_to_selected = false;
            }

            // 检测鼠标点击事件
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xcc, 0xcc, 0xcc, 0xcc));
            bool is_selected = system_processing_data.id && (system_processing_data.id == train_data.id);
            if(ImGui::TableNextColumn() && ImGui::Selectable(std::to_string(index + 1).c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns))
            {
                view_table_to_selected          = true;
                view_is_show_user_input         = true;
                system_processing_data.id       = train_data.id;
                system_is_fresh_processing_data = true;
            }
            ImGui::PopStyleColor();

            // 车次 ID
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data.id);
            // 车次
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data.number);
            // 始发站
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data.start_station);
            // 到达站
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data.arrive_station);
            // 出发时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", date_to_string(uint64_time_to_date(train_data.start_time)).c_str());
            // 到达时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", date_to_string(uint64_time_to_date(train_data.arrive_time)).c_str());
            // 票数
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data.ticket_remain);
            // 价格
            ImGui::TableNextColumn();
            ImGui::Text("%s", uint32_price_to_string(train_data.ticket_price).c_str());
            // 是否有效
            ImGui::TableNextColumn();
            ImGui::TextColored(parse_train_status_color((TrainStatus)train_data.train_status), "%s", parse_train_status((TrainStatus)train_data.train_status).c_str());
        }

        ImGui::EndTable();
    }

    // 恢复样式变量
    ImGui::PopStyleVar(2);

    ImGui::End();
    ImGui::PopFont();
}

// Kuuhaku-kazari
// 2024.12.21
