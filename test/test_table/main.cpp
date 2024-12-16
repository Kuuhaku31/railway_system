
#include "imgui_setup.h"

#include <iostream>
#include <string>
#include <vector>

static ImGui_setup& imgui = ImGui_setup::Instance();

struct Data
{
    int         id;
    std::string name;
    float       value;
};

// 模拟数据集
std::vector<Data> dataSet = {
    { 1, "Item 1", 10.0f },
    { 2, "Item 2", 20.0f },
    { 3, "Item 3", 30.0f }
};

void
DrawTable(bool& showTable)
{
    if(!showTable)
        return;

    // 创建表格
    if(ImGui::BeginTable("Data Table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        // 表格头
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Value");
        ImGui::TableSetupColumn("Actions");

        ImGui::TableHeadersRow();

        // 遍历数据并添加到表格
        for(size_t i = 0; i < dataSet.size(); ++i)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", dataSet[i].id);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", dataSet[i].name.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.2f", dataSet[i].value);

            ImGui::TableSetColumnIndex(3);
            if(ImGui::Button(("Edit##" + std::to_string(i)).c_str()))
            {
                // 修改数据
                dataSet[i].name = "Edited " + std::to_string(i);
                dataSet[i].value *= 1.5f;
            }
            if(ImGui::Button(("Delete##" + std::to_string(i)).c_str()))
            {
                // 删除数据
                dataSet.erase(dataSet.begin() + i);
                break; // 删除元素后需要跳出循环，避免下标越界
            }
        }

        ImGui::EndTable();
    }
}

int
main()
{
    printf("Hello, test_table!\n");
    imgui.Init("Table Example", { 100, 100, 800, 600 });

    Event event;
    bool  is_running = true;
    bool  showTable  = true;
    while(is_running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                is_running = false;

            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        imgui.On_frame_begin();

        ImGui::Begin("Table Example", &showTable);
        DrawTable(showTable);
        ImGui::End();

        imgui.On_frame_end();
    }

    return 0;
}
