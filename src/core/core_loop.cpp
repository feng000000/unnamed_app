#include "core/core_loop.h"

#include <atomic>
#include <unistd.h>

#include "ui/ui.h"

std::atomic<bool> is_running{true};

void core::stop()
{
    is_running = false;
}


void core::core_loop()
{
    while (is_running)
    {
        ui::calendar::update_now_datetime();

        sleep(1);
    }
}
