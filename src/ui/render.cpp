#include "ui/render.h"
#include <cassert>
#include <csignal>
#include <memory>
#include <vector>
#include <thread>

#include "imgui.h"
#include "imgui_internal.h"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

#include "ui/ui.h"
#include "core/core_loop.h"
#include "utils/context.hpp"

// TODO: 动态刷新率
// app lifespan (RAII)
utils::ctx::Context
ui::new_app_context()
{
    utils::ctx::Context ctx("app context");
    ctx.push_enter_func(
        []()
        {
            auto log_level = spdlog::level::info;
            spdlog::cfg::load_env_levels();
            spdlog::set_level(log_level);
            spdlog::info("log level: {}", static_cast<int>(log_level));
        }
    );
    ctx.push_enter_func(
        []()
        {
            spdlog::info("create core loop thread");
            std::thread core_loop_thread(core::core_loop);
            core_loop_thread.detach();
        }
    );
    ctx.push_exit_func([]() { core::stop(); });

    ctx.enter();

    return std::move(ctx);
}

void
ui::render()
{
    // DEBUG: ShowMetricsWindow
    ImGui::ShowMetricsWindow();

    auto dockspace_id = ui::dock::submission_dockspace();

    window_list = std::vector<std::unique_ptr<WindowBase>>();

    // DEBUG: test CalendarWindow
    ui::window_list.emplace_back(
        std::make_unique<calendar::CalendarWindow>("calendar", true)
    );

    // DEBUG: test NoteWindow
    ui::window_list.emplace_back(std::make_unique<note::NoteWindow>(
        "note",
        true,
        std::make_shared<note::NoteContent>(
            "data", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
        )
    ));

    // DEBUG: test split
    static ImGuiID new_part_id = 0, old_part_id = 0;
    if (ui::FIRST_TIME)
    {
        spdlog::debug("update window {}", ui::window_list[0]->name);
        spdlog::debug("update window {}", ui::window_list[1]->name);

        ui::dock::split(
            dockspace_id,
            // ui::MAIN_WINDOW_NAME,
            // ui::window_list[0]->name,
            old_part_id,
            new_part_id,
            ImGuiDir::ImGuiDir_Left,
            0.2f
        );

        spdlog::info("mount {} to {:X}", ui::window_list[0]->name, old_part_id);
        ImGui::DockBuilderDockWindow(ui::window_list[0]->name, old_part_id);

        spdlog::info("mount {} to {:X}", ui::window_list[1]->name, old_part_id);
        ImGui::DockBuilderDockWindow(ui::window_list[1]->name, new_part_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ui::window_list[0]->update(old_part_id);
    ui::window_list[1]->update(new_part_id);

    // for (auto& window : ui::window_list)
    // {
    //     // ImGuiID window_dock_id = dockspace_id;
    //     // if (ui::FIRST_TIME)
    //     // {
    //     //     spdlog::debug("update window {}", window->name);
    //     //     window_dock_id = ui::dock::split(
    //     //         dockspace_id,
    //     //         // ui::MAIN_WINDOW_NAME,
    //     //         window->name,
    //     //         ImGuiDir::ImGuiDir_Right,
    //     //         0.8f
    //     //     );
    //     //     // ui::dock::set_dock_layout(window->name,
    //     dockspace_id);
    //     //     // ui::dock::set_dock_layout(window->name,
    //     window_dock_id);
    //     //     ImGui::DockBuilderFinish(dockspace_id);
    //     // }

    //     window->update(0);
    // }

    ui::FIRST_TIME = false;
}
