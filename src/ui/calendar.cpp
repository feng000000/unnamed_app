
#include "imgui.h"
#include "imgui_internal.h"
#include "spdlog/spdlog.h"

#include "ui/calendar.h"
#include "ui/window.h"
#include "ui/manager.h"
#include <string>

using namespace ui::calendar;

CalendarWindow::CalendarWindow(const char* name, bool showing)
    : Window(name, showing)
{
}

inline void
set_dock_layout(const char* name, ImGuiID dockspace_id)
{
    spdlog::debug("adjust dock layout");

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(
        dockspace_id, ImGuiDockNodeFlags_DockSpace
    );
    ImGui::DockBuilderSetNodeSize(
        dockspace_id, ImGui::GetMainViewport()->Size
    );

    // ImGuiID dock_id = ImGui::DockBuilderSplitNode(
    //     dockspace_id, ImGuiDir_Right, 0.5f, nullptr, nullptr
    // );
    ImGuiID dock_id = dockspace_id;

    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_id);
    node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

    ImGui::DockBuilderDockWindow(name, dock_id);

    // 4. 完成构建
    ImGui::DockBuilderFinish(dockspace_id);
}

void
draw_day_text(int day)
{
    std::string day_text = std::to_string(day);

    // TODO: check day != current day

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
    if (day != 9)
    {
        ImGui::TextDisabled("%s", day_text.c_str());
        return;
    }

    // 获取当前绝对屏幕位置
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImVec2 text_size = ImGui::CalcTextSize(day_text.c_str());

    float radius =
        (std::max(text_size.x, text_size.y) / 2.0f) + 4;
    ImVec2 center = ImVec2(pos.x + 8, pos.y + 6);

    // draw red circle
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircleFilled(
        center,
        radius,
        IM_COL32(230, 40, 40, 255),
        128
    );  // 红色

    // 将光标移动到圆圈中心（需减去文字一半大小以对齐）
    ImGui::SetCursorScreenPos(ImVec2(
        center.x - text_size.x * 0.5f,
        center.y - text_size.y * 0.5f
    ));

    // 渲染白色数字
    ImGui::TextColored(
        ImVec4(1, 1, 1, 1), "%s", day_text.c_str()
    );

    // 修正光标位置
    // ImGui::SetCursorScreenPos(
    //     ImVec2(pos.x, pos.y + radius * 2.0f + 2.0f)
    // );
    // ImGui::Dummy(ImVec2(radius * 2.0f, 0.0f));
}

void
draw_day_event(int day)
{
    spdlog::error("draw_day_event is not implemented yet");

    // --- 在方格内显示多行标题 ---
    // // 模拟该日期下的多个事件
    // static std::vector<std::pair<const char*, ImVec4>>
    //     mock_events = {
    //         // {"Meeting", ImVec4(0.2f, 0.5f,
    //         // 0.9f, 1.0f)},
    //         // {"Gym", ImVec4(0.2f, 0.8f, 0.2f, 1.0f)},
    //         // {"Dinner", ImVec4(0.9f, 0.5f, 0.2f, 1.0f)}
    //     };
    // // 限制显示区域，防止超出单元格
    // ImGui::BeginGroup();
    // for (const auto& ev : mock_events)
    // {
    //     // 简单的彩色背景标签样式
    //     ImGui::PushStyleColor(
    //         ImGuiCol_Text, ImVec4(1, 1, 1, 1)
    //     );
    //     ImGui::PushStyleColor(ImGuiCol_Button, ev.second);
    //     ImGui::Button(ev.first, ImVec2(-FLT_MIN, 0.0f));
    //     ImGui::PopStyleColor(2);
    // }
    // ImGui::EndGroup();
}

bool
CalendarWindow::update(ImGuiID dock_node_id)
{
    if (!this->showing)
        return true;

    if (ui::FIRST_TIME)
        set_dock_layout(name, dock_node_id);

    ImGui::Begin(name);

    float header_height = 25.0f;
    auto avail_size = ImGui::GetContentRegionAvail();
    float cell_height =
        (avail_size[1] - header_height - 20.0f) / 5;

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
    // 设置 Table：7列，带边框
    if (ImGui::BeginTable(
            "CalendarTable",
            7,
            ImGuiTableFlags_Borders
        ))
    {

        // 1. 渲染表头 (星期)
        for (int i = 0; i < 7; i++)
            ImGui::TableSetupColumn(week_title[i]);

        ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 20.0f);
        for (int i = 0; i < 7; i++)
        {
            ImGui::TableNextColumn();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6.0f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
            ImGui::TableHeader(week_title[i]);
        }

        // TODO: 模拟渲染 30 天, 应该是实际的月份对应天数
        auto day = 1;
        auto max_day = 30;
        while (day <= max_day)
        {
            ImGui::TableNextRow(
                ImGuiTableRowFlags_None, cell_height
            );
            auto tmp_day = day;
            for (int i = 0; i < 7 && day <= max_day; ++i, ++day)
            {
                ImGui::TableNextColumn();

                // 设置当前行的高度
                float start_y = ImGui::GetCursorPosY();

                draw_day_text(day);
                draw_day_event(day);

                // 确保每个方格有最小高度（填充剩余空间）

                auto t_column_idx = ImGui::TableGetColumnIndex();
                ImRect t_cell_rect = ImGui::TableGetCellBgRect(
                    ImGui::GetCurrentTable(), t_column_idx
                );

                float current_y = ImGui::GetCursorPosY();
                if (current_y - start_y < cell_height)
                {
                    ImGui::Dummy(ImVec2(
                        0, cell_height - (current_y - start_y)
                    ));
                }

                // highlight
                ImRect cell_rect = ImGui::TableGetCellBgRect(
                    ImGui::GetCurrentTable(), i
                );

                // 检查鼠标是否在刚才获取的单元格矩形内
                if (ImGui::IsMouseHoveringRect(
                        cell_rect.Min, cell_rect.Max
                    ))
                {
                    spdlog::debug("mouse in the day {}", day);
                    ImU32 hover_color =
                        IM_COL32(128, 128, 128, 50);

                    // 在最上层绘制一个覆盖整个单元格的矩形
                    ImGui::GetWindowDrawList()->AddRectFilled(
                        cell_rect.Min, cell_rect.Max, hover_color
                    );
                }
            }  // render row loop

        }  // while (day <= 30)

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    ImGui::End();

    return true;
}