#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include "imgui.h"
#include "ui/window.h"

namespace ui::calendar
{

// the window that render the content node tree
class CalendarWindow : public Window
{
public:
    CalendarWindow(const char* name, bool showing);

    ~CalendarWindow() = default;

    bool
    update(ImGuiID dock_node_id);

private:
    static constexpr const char* week_title[7] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
};

}  // namespace ui::calendar

#endif
