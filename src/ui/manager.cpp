#include <iostream>
#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"

#include "ui/manager.h"
#include "ui/window.h"
#include "ui/note.h"


inline void
set_dock_layout(ImGuiID dockspace_id)
{
    std::cout << "adjust dock layout" << std::endl;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(
        dockspace_id, ImGuiDockNodeFlags_DockSpace
    );
    ImGui::DockBuilderSetNodeSize(
        dockspace_id, ImGui::GetMainViewport()->Size
    );
    ImGuiID dock_id_left, dock_id_right;

    ImGui::DockBuilderSplitNode(
        dockspace_id,
        ImGuiDir_Left,
        0.5f,
        &dock_id_left,
        &dock_id_right
    );
    std::cout << "left id: " << dock_id_left << std::endl
              << "right id: " << dock_id_right << std::endl;

    // 3. 关键：指定 "main window" 停靠在右侧区域
    ImGui::DockBuilderDockWindow("placeholder", dock_id_left);
    ImGui::DockBuilderDockWindow(
        UI::MAIN_WINDOW_NAME, dock_id_right
    );

    // 4. 完成构建
    ImGui::DockBuilderFinish(dockspace_id);
}

// TODO: submission_dockspace: remove unnecessary code
inline ImGuiID
submission_dockspace()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags =
        ImGuiDockNodeFlags_None;
    ImGuiID dockspace_id = ImGui::GetID(UI::DOCK_SPACE_NAME);

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

    ImGui::Begin(
        UI::DOCK_SPACE_WINDOW_NAME, nullptr, window_flags
    );
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
    // Show demo options and help
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be
            // moved to the front of other windows, which we can't
            // undo at the moment without finer window depth/z
            // control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem(
                    "Flag: NoDockingOverCentralNode",
                    "",
                    (dockspace_flags &
                     ImGuiDockNodeFlags_NoDockingOverCentralNode
                    ) != 0
                ))
            {
                dockspace_flags ^=
                    ImGuiDockNodeFlags_NoDockingOverCentralNode;
            }
            if (ImGui::MenuItem(
                    "Flag: NoDockingSplit",
                    "",
                    (dockspace_flags &
                     ImGuiDockNodeFlags_NoDockingSplit) != 0
                ))
            {
                dockspace_flags ^=
                    ImGuiDockNodeFlags_NoDockingSplit;
            }
            if (ImGui::MenuItem(
                    "Flag: NoUndocking",
                    "",
                    (dockspace_flags &
                     ImGuiDockNodeFlags_NoUndocking) != 0
                ))
                dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking;
            if (ImGui::MenuItem(
                    "Flag: NoResize",
                    "",
                    (dockspace_flags & ImGuiDockNodeFlags_NoResize
                    ) != 0
                ))
                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
            if (ImGui::MenuItem(
                    "Flag: AutoHideTabBar",
                    "",
                    (dockspace_flags &
                     ImGuiDockNodeFlags_AutoHideTabBar) != 0
                ))
            {
                dockspace_flags ^=
                    ImGuiDockNodeFlags_AutoHideTabBar;
            }
            if (ImGui::MenuItem(
                    "Flag: PassthruCentralNode",
                    "",
                    (dockspace_flags &
                     ImGuiDockNodeFlags_PassthruCentralNode) != 0,
                    opt_fullscreen
                ))
            {
                dockspace_flags ^=
                    ImGuiDockNodeFlags_PassthruCentralNode;
            }
            ImGui::Separator();

            // if (ImGui::MenuItem("Close", NULL, false, p_open !=
            // NULL))
            //     *p_open = false;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::TextUnformatted(
                "This demo has nothing to do with enabling "
                "docking!"
                "\n"
                "This demo only demonstrate the use of "
                "ImGui::DockSpace() which allows you to "
                "manually\ncreate a docking node _within_ "
                "another window."
                "\n"
                "Most application can simply call "
                "ImGui::DockSpaceOverViewport() and be done with "
                "it."
            );
            ImGui::Separator();
            ImGui::TextUnformatted(
                "When docking is enabled, you can ALWAYS dock "
                "MOST window into another! Try it now!"
                "\n"
                "- Drag from window title bar or their tab to "
                "dock/undock."
                "\n"
                "- Drag from window menu button (upper-left "
                "button) to undock an entire node (all windows)."
                "\n"
                "- Hold SHIFT to disable docking (if "
                "io.ConfigDockingWithShift == false, default)"
                "\n"
                "- Hold SHIFT to enable docking (if "
                "io.ConfigDockingWithShift == true)"
            );
            ImGui::Separator();
            ImGui::TextUnformatted("More details:");
            ImGui::Bullet();
            ImGui::SameLine();
            ImGui::TextLinkOpenURL(
                "Docking Wiki page",
                "https://github.com/ocornut/imgui/wiki/Docking"
            );
            ImGui::BulletText(
                "Read comments in ShowExampleAppDockSpace()"
            );
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();

    return dockspace_id;
}

void
UI::operation_before_main_loop()
{
}

void
UI::render()
{
    auto dockspace_id = submission_dockspace();

    window_list = std::vector<std::unique_ptr<Window>>();
    ImGuiWindowFlags content_window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    UI::window_list.emplace_back(
        std::make_unique<Note::NoteWindow>(
            "note",
            true,
            std::make_shared<Note::NoteContent>("data", content_window_flags)
        )
    );

    for (auto& window : UI::window_list)
    {
        if (!window->is_showing())
            continue;

        window->update(dockspace_id);
    }

    UI::FIRST_TIME = false;
}