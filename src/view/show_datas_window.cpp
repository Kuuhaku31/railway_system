
// show_datas_window.cpp

#include "imgui_windows.h"

#include "controler.h"
#include "view.h"

static View&      view      = View::Instance();
static Controler& controler = Controler::Instance();

void
ImGuiWindowTrainDatas(bool* p_open)
{
    if(p_open && !*p_open) return;

    uint32_t window_flags = 0;

    // 不显示标题栏
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    // 无法调整宽度
    window_flags |= ImGuiWindowFlags_NoResize;
    // 无法移动
    window_flags |= ImGuiWindowFlags_NoMove;
    // 无法滚动
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    // 无法滚动
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    // 无法Dock
    window_flags |= ImGuiWindowFlags_NoDocking;
    // 保持在最后
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    // 位置在左上角（只一次有效）
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImVec2 window_size = ImGui::GetIO().DisplaySize;
    window_size.y *= 0.65;

    // 设置宽度为程序窗口宽度
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

    // 中文显示
    ImGui::PushFont(view.font_chinese);
    ImGui::Begin("Train Datas", p_open, window_flags);

    // 临时修改样式变量，取消表格和窗口的间距
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    // 第一个元素位置为(0, 0)
    ImGui::SetCursorPos(ImVec2(0, 0));

    const Controler::TrainDatas& train_datas = controler.train_datas;

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
        ImGui::TableSetupColumn("Is Running", ImGuiTableColumnFlags_WidthFixed, 100.0f);

        ImGui::TableHeadersRow();

        int index = 0;
        for(auto& train_data : train_datas)
        {
            index++;

            ImGui::TableNextRow();

            // 序号
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", index);
            // 车次 ID
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data->train_id);
            // 车次
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->train_number.c_str());
            // 始发站
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->train_start_station.c_str());
            // 到达站
            ImGui::TableNextColumn();
            ImGui::Text("%s", train_data->train_arrive_station.c_str());
            // 出发时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", date_to_string(train_data->train_start_time).c_str());
            // 到达时间
            ImGui::TableNextColumn();
            ImGui::Text("%s", date_to_string(train_data->train_arrive_time).c_str());
            // 票数
            ImGui::TableNextColumn();
            ImGui::Text("%d", train_data->train_ticket_count);
            // 价格
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", train_data->train_ticket_price);
            // 是否有效
            ImGui::TableNextColumn();

            ImVec4 color = ImVec4(0, 255, 0, 255);

            switch(train_data->train_status)
            {
            case TrainStatus::NORMAL: color = ImVec4(0, 255, 0, 255); break;
            case TrainStatus::DELAY: color = ImVec4(255, 255, 0, 255); break;
            case TrainStatus::STOP: color = ImVec4(255, 0, 0, 255); break;
            case TrainStatus::OTHER: color = ImVec4(0, 0, 255, 255); break;
            default: break;
            }
            ImGui::TextColored(color, "%s", parse_train_status(train_data->train_status).c_str());

            // 如果是选中的行，改变背景颜色
            if(view.selected_id == train_data->train_id)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(255, 0, 0, 255));
            }
        }

        ImGui::EndTable();
    }

    // 恢复样式变量
    ImGui::PopStyleVar(2);

    ImGui::End();
    ImGui::PopFont();
}
