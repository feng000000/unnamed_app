#ifndef __UI_WINDOW_MANAGER_H__
#define __UI_WINDOW_MANAGER_H__

#include <vector>
#include <memory>

#include "ui/window_base.h"
#include "utils/context.hpp"

namespace ui
{
inline bool FIRST_TIME = true;
inline constexpr const char* DOCK_SPACE_WINDOW_NAME =
    "Dock Space Window";
inline constexpr const char* DOCK_SPACE_NAME = "Dock Space";
inline constexpr const char* MAIN_WINDOW_NAME = "Main Window";

inline std::vector<std::unique_ptr<Window>> window_list;


utils::ctx::Context get_context();

void
render();

}  // namespace ui

#endif
