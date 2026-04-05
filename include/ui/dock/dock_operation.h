#ifndef __UI_DOCK_OPERATION_H__
#define __UI_DOCK_OPERATION_H__

#include "imgui.h"

namespace ui::dock
{
inline constexpr const char* DOCK_SPACE_WINDOW_NAME =
    "Dock Space Window";
inline constexpr const char* DOCK_SPACE_NAME = "Dock Space";

ImGuiID
submission_dockspace();

void
set_dock_layout(const char* window_name, ImGuiID dock_id);

void
split(
    ImGuiID node_id,
    // const char* parent_window_name,
    ImGuiID& old_part_id,
    ImGuiID& new_part_id,
    ImGuiDir new_part_direction,
    float new_part_ratio = 0.5f
);

}  // namespace ui::dock

#endif
