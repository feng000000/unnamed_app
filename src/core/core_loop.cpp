#include "spdlog/spdlog.h"
#include "core/core_loop.h"

#include <atomic>
#include <unistd.h>


std::atomic<bool> is_running{true};

void core::stop()
{
    is_running = false;
}

void core::core_loop()
{
    spdlog::error("core loop not implemented yet");
    while (is_running)
    {
        spdlog::debug("[CORE LOOP]do something");
        sleep(10);
    }
}