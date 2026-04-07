

#include "imgui.h"
#include "imgui_internal.h"

#include "spdlog/spdlog.h"
#include "ui/ui.h"

using namespace ui::dock;

// 提交一个新的 Dock Space
ImGuiID
ui::dock::submission_dockspace()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoTabBar;
    ImGuiID dockspace_id = ImGui::GetID(DOCK_SPACE_NAME);

    // We are using the ImGuiWindowFlags_NoDocking flag to make
    // the parent window not dockable into, because it would be
    // confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoBringToFrontOnFocus |
                        ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &=
            ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode,
    // DockSpace() will render our background and handle the
    // pass-thru hole, so we ask Begin() to not render a
    // background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns
    // false (aka window is collapsed). This is because we want to
    // keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent
    // and become undocked. We cannot preserve the docking
    // relationship between an active window and an inactive
    // docking, otherwise any change of dockspace/settings would
    // lead to windows being stuck in limbo and never being
    // visible.
    if (!opt_padding)
    {
        ImGui::PushStyleVar(
            ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)
        );
    }

    ImGui::Begin(DOCK_SPACE_WINDOW_NAME, nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGui::DockSpace(
            dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags
        );
    }

    ImGui::End();

    return dockspace_id;
}


// 切分 Dock Space
// - node_id: 待切分的 dockspace node ID
// - parent_window_name: 原 window name, 用于挂载到新位置
// - new_part_direction: 切分方向
// - new_part_ratio: 切分方向占比
// - Return: 切分方向 node id
void
ui::dock::split(
    ImGuiID node_id,
    ImGuiID& old_part_id,
    ImGuiID& new_part_id,
    ImGuiDir new_part_direction,
    float new_part_ratio
)
{
    ImGui::DockBuilderSplitNode(
        node_id,
        new_part_direction,
        new_part_ratio,
        &new_part_id,
        &old_part_id
    );
    spdlog::info("[split()] new_part_id: {:X}", new_part_id);
    spdlog::info("[split()] old_part_id: {:X}", old_part_id);

    return;
}
