#ifndef __UI_MENU_H__
#define __UI_MENU_H__

#include "imgui.h"
#include "ui/window_base.h"

namespace ui::menu
{

class MenuWindow : WindowBase
{
public:
    MenuWindow(const char* name, bool showing);

    ~MenuWindow() = default;

    bool
    update(ImGuiID dock_node_id);
};

}  // namespace ui::menu

#endif
