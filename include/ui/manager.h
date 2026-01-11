#ifndef __UI_MAIN_WINDOW_H__
#define __UI_MAIN_WINDOW_H__

#include <vector>
#include <memory>

#include "ui/window.h"

namespace UI
{
inline bool FIRST_TIME = true;
inline constexpr const char* DOCK_SPACE_WINDOW_NAME =
    "Dock Space Window";
inline constexpr const char* DOCK_SPACE_NAME = "Dock Space";
inline constexpr const char* MAIN_WINDOW_NAME = "Main Window";

inline std::vector<std::unique_ptr<Window>> window_list;

void operation_before_main_loop();
void render();
}  // namespace UI

#endif
