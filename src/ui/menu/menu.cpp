
#include "imgui.h"
#include "ui/ui.h"

namespace ui::menu
{

bool
MenuWindow::update(ImGuiID dock_node_id)
{
    if (ImGui::Begin("Menu"))
    {
        // TODO:
    }
    ImGui::End();

    return true;
}

}  // namespace ui::menu
