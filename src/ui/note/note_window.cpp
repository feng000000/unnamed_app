#include <cmath>
#include <memory>
#include <sys/types.h>
#include <utility>

#include "imgui.h"
#include "spdlog/spdlog.h"

#include "ui/ui.h"

namespace ui::note
{

bool
NoteWindow::update(ImGuiID dock_node_id)
{
    if (!this->showing)
        return false;

    if (ui::FIRST_TIME)
    {
    }

    if (ImGui::Begin(name, &(this->showing), flags))
    {
        id = ImGui::GetID("");

        ImGui::Text("this is Note Window");

        if (ImGui::Button("note window button"))
            spdlog::debug("click note window button");

        ImGui::End();

        return true;
    }
    return false;
}

NoteWindow::NoteWindow(
    const char* name,
    bool showing,
    std::shared_ptr<NoteContent> data
)
    : WindowBase(name, showing), data(std::move(data))
{
}

}  // namespace ui::note
