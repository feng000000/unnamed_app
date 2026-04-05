#include <cstdint>
#include <mutex>
#include <string>
#include <ctime>
#include <vulkan/vulkan_core.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "spdlog/spdlog.h"

#include "ui/ui.h"


namespace ui::calendar
{
static std::mutex now_date_mtx;
static DateTime NOW;

void
update_now_datetime()
{
    static double lastupdate = 0;

    if (lastupdate && ImGui::GetTime() - lastupdate < 1)
        return;

    spdlog::debug("lastupdate: {}", lastupdate);

    time_t now = time(nullptr);
    struct tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif

    // 计算当月1号是星期几
    uint8_t first_mday_week;
    {
        struct tm tm_info = {};
        tm_info.tm_year = local_tm.tm_year;
        tm_info.tm_mon = local_tm.tm_mon;
        tm_info.tm_mday = 1;
        mktime(&tm_info);
        first_mday_week = tm_info.tm_wday;
    }

    std::lock_guard<std::mutex> lock(now_date_mtx);
    NOW = DateTime{
        .timestamp_s = now,
        .year = static_cast<uint16_t>(1900 + local_tm.tm_year),
        .month = static_cast<uint8_t>(1 + local_tm.tm_mon),
        .day = static_cast<uint8_t>(local_tm.tm_mday),
        .week =
            static_cast<uint8_t>((local_tm.tm_wday + 6) % 7 + 1),
        .first_mday_week =
            static_cast<uint8_t>((first_mday_week + 6) % 7 + 1),
        .hour = static_cast<uint8_t>(local_tm.tm_hour),
        .minute = static_cast<uint8_t>(local_tm.tm_min),
        .second = static_cast<uint8_t>(local_tm.tm_sec),
    };
    lastupdate = ImGui::GetTime();
}

DateTime
get_now_datetime()
{
    std::lock_guard<std::mutex> lock(now_date_mtx);
    return NOW;

    // DEBUG: 1号周一
    // return DateTime{
    //     .year=2025,
    //     .month=12,
    //     .day=1,
    //     .week=1,
    //     .first_mday_week=1,
    //     .hour=0,
    //     .minute=0,
    //     .second=0,
    // };
    // DEBUG: 31号周日
    // return DateTime{
    //     .year=2026,
    //     .month=5,
    //     .day=31,
    //     .week=7,
    //     .first_mday_week=1,
    //     .hour=0,
    //     .minute=0,
    //     .second=0,
    // };
}

// 获取当前年月份的天数
inline int
get_days_in_month(int year, int month)
{
    static constexpr int days_in_month[12] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 &&
        ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        return 29;
    return days_in_month[month - 1];
}

CalendarWindow::CalendarWindow(const char* name, bool showing)
    : ui::WindowBase(name, showing)
{
}

void
draw_day_text(int day, int today)
{
    std::string day_text = std::to_string(day);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6.0f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
    if (day != today)
    {
        ImGui::TextDisabled("%s", day_text.c_str());
        return;
    }

    // 获取当前绝对屏幕位置
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImVec2 text_size = ImGui::CalcTextSize(day_text.c_str());

    float fontSize = ImGui::GetFontSize();
    spdlog::debug("font size: {}", fontSize);
    float radius =
    (std::max(text_size.x, text_size.y) / 2.0f) + 0.1 * fontSize;
    spdlog::debug("radius: {}", radius);

    ImVec2 center = ImVec2(pos.x + text_size.x / 2 + 2, pos.y + text_size.y / 2 + 2);

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
    // DEBUG: draw_day_event is not implemented yet
    // spdlog::error("draw_day_event is not implemented yet");
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

// 渲染表头 (星期)
inline void
render_header(float height)
{
    for (int i = 0; i < 7; i++)
        ImGui::TableSetupColumn(CalendarWindow::week_title[i]);

    ImGui::TableNextRow(ImGuiTableRowFlags_Headers, height);
    for (int i = 0; i < 7; i++)
    {
        ImGui::TableNextColumn();
        // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6.0f);
        // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
        ImGui::TableHeader(CalendarWindow::week_title[i]);
    }
}

inline void
render_cell(int day, int today, int col_idx, float cell_height)
{
    ImGui::TableNextColumn();

    // 设置当前行的高度
    float start_y = ImGui::GetCursorPosY();

    if (day > 0)
        draw_day_text(day, today);
    draw_day_event(day);

    // 确保每个方格有最小高度（填充剩余空间）
    float current_y = ImGui::GetCursorPosY();
    if (current_y - start_y < cell_height)
    {
        ImGui::Dummy(
            ImVec2(0, cell_height - (current_y - start_y))
        );
    }

    if (day <= 0)
    {
        // spdlog::debug("skip render cell");
        return;
    }

    // highlight
    ImRect cell_rect = ImGui::TableGetCellBgRect(
        ImGui::GetCurrentTable(), col_idx
    );
    // 检查鼠标是否在刚才获取的单元格矩形内
    if (ImGui::IsMouseHoveringRect(cell_rect.Min, cell_rect.Max))
    {
        spdlog::debug("mouse in the day {}", day);
        ImU32 hover_color = IM_COL32(128, 128, 128, 50);

        // 在最上层绘制一个覆盖整个单元格的矩形
        ImGui::GetWindowDrawList()->AddRectFilled(
            cell_rect.Min, cell_rect.Max, hover_color
        );
    }
}

bool
CalendarWindow::update(ImGuiID dock_node_id)
{
    if (!this->showing)
        return true;

    if (ui::FIRST_TIME)
        update_now_datetime();

    auto date = get_now_datetime();

    ImGui::Begin(name);

    // ImGui::DockSpace(dock_node_id, ImVec2(0, 0), this->flags);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

    // 设置 Table：7列，带边框
    if (ImGui::BeginTable(
            "CalendarTable", 7, ImGuiTableFlags_Borders
        ))
    {

        // float header_height = 25.0f;
        float header_height = ImGui::GetFontSize();
        // ImVec2 avail_size = ImGui::GetContentRegionAvail();
        ImVec2 avail_size_max =
            ImGui::GetWindowContentRegionMax();
        ImVec2 avail_size_min =
            ImGui::GetWindowContentRegionMin();
        float avail_height = avail_size_max.y - avail_size_min.y;

        // 高度设置为能显示五行
        float cell_height =
            (avail_height - header_height - 20.0f) / 5;

        render_header(header_height);

        int32_t day = 1;
        int32_t _w_idx = date.first_mday_week - 1;
        // 每一轮渲染一行
        auto max_day = get_days_in_month(date.year, date.month);
        while (day <= max_day)
        {
            ImGui::TableNextRow(
                ImGuiTableRowFlags_None, cell_height
            );
            if (day == 1)
                for (size_t i = 0; i < _w_idx; ++i)
                    render_cell(-1, 0, 0, 0);

            while (day <= max_day)
            {
                render_cell(day, date.day, _w_idx, cell_height);
                ++day;
                _w_idx = (_w_idx + 1) % 7;

                // 一行渲染一周
                if (_w_idx == 0)
                    break;
            }  // render row loop

        }  // while (day <= 30)

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    ImGui::End();

    return true;
}

}  // namespace ui::calendar
