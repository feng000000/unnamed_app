#ifndef __UI_CALENDAR_H__
#define __UI_CALENDAR_H__

#include <ctime>

#include "imgui.h"
#include "ui/window_base.h"
#include "core/calendar/calendar_datetime.h"

namespace ui::calendar
{

// 日历时间
class Event
{
    // TODO: 业务逻辑: 日志单元格指向具体任务窗口
    void* data;
    core::calendar::DateTime datetime;
};

class CalendarWindow : public ui::WindowBase
{
public:
    CalendarWindow(const char* name, bool showing);

    ~CalendarWindow() = default;

    bool
    update(ImGuiID dock_node_id);

    static constexpr const char* week_title[7] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    };
};

}  // namespace ui::calendar

#endif
