#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "ui/manager.h"

#include "ui/note.h"

inline void set_dock_layout(
    const char* name, ImGuiID dockspace_id
)
{
    std::cout << "adjust dock layout" << std::endl;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(
        dockspace_id, ImGuiDockNodeFlags_DockSpace
    );
    ImGui::DockBuilderSetNodeSize(
        dockspace_id, ImGui::GetMainViewport()->Size
    );

    ImGuiID dock_id = ImGui::DockBuilderSplitNode(
        dockspace_id, ImGuiDir_Right, 0.5f, nullptr, nullptr
    );

    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_id);
    node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

    ImGui::DockBuilderDockWindow(name, dock_id);

    // 4. 完成构建
    ImGui::DockBuilderFinish(dockspace_id);
}

bool UI::Note::NoteWindow::show(ImGuiID dock_node_id)
{
    showing = true;

    if (UI::FIRST_TIME)
    {
        set_dock_layout(name, dock_node_id);
    }

    if (ImGui::Begin(name, &showing, flags))
    {
        id = ImGui::GetID("");

        ImGui::Text("this is Note Window");

        if (ImGui::Button("note window button"))
        {
            std::cout << "click note window button" << std::endl;
        }

        ImGui::End();

        return true;
    }
    return false;
}
