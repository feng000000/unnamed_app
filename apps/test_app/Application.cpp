
#include "imgui_app.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "core/core_loop.h"
#include "ui/render.h"
#include "ui/ui.h"


int main()
{
    imgui_app::context::Context lifespan("app context");
    lifespan.push_enter_func(
        []()
        {
            auto log_level = spdlog::level::debug;
            spdlog::cfg::load_env_levels();
            spdlog::set_level(log_level);
            spdlog::info("log level: {}", static_cast<int>(log_level));
        }
    );
    lifespan.push_enter_func(
        []()
        {
            spdlog::info("create core loop thread");
            std::thread core_loop_thread(core::core_loop);
            core_loop_thread.detach();
        }
    );
    lifespan.push_exit_func([]() { core::stop(); });

    lifespan.enter();

    imgui_app::ImGuiApp app{
        // std::move(lifespan),
        imgui_app::context::Context{"placeholder"},
        ui::render,
    };

    app.run();

    return 0;
}
