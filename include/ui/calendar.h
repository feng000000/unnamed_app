#ifndef __UI_CALENDAR_H__
#define __UI_CALENDAR_H__

#include "imgui.h"
#include "ui/window_base.h"
#include <cstdint>
#include <ctime>

namespace ui::calendar
{

struct DateTime
{
    time_t timestamp_s;
    uint16_t year;
    // [1, 12]
    uint8_t month;
    // [1, 31]
    uint8_t day;
    // [1, 7], start at monday
    uint8_t week;
    // [1, 7], 当前月份第一天是周几, start at Monday
    uint8_t first_mday_week;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

void
update_now_datetime();

DateTime
get_now_datetime();

// 日历时间
class Event
{
    // TODO: 指向具体任务
    void* data;
    DateTime datetime;
};

class CalendarWindow : public Window
{
public:
    CalendarWindow(const char* name, bool showing);

    ~CalendarWindow() = default;

    bool
    update(ImGuiID dock_node_id);

    static constexpr const char* week_title[7] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
};

}  // namespace ui::calendar

#endif
