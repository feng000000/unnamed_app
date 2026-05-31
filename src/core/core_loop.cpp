#include "core/core_loop.h"

#include <atomic>
#include <unistd.h>


#include "spdlog/spdlog.h"
#include "core/calendar/calendar_datetime.h"


std::atomic<bool> is_running{true};

void core::stop()
{
    is_running = false;
}

// TODO: 注册定时任务 (刷新时间)
void core::core_loop()
{
    spdlog::debug("core loop start");
    while (is_running)
    {
        core::calendar::update_now_datetime();

        sleep(1);
    }
}
