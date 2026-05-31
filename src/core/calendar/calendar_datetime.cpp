#include <chrono>

#include <cstdint>
#include <mutex>
#include <ctime>
#include <optional>
#include <vulkan/vulkan_core.h>

#include "spdlog/spdlog.h"
#include "core/calendar/calendar_datetime.h"

namespace core::calendar
{

bool datetime_init = false;
std::mutex now_date_mtx;

std::optional<DateTime>
get_now_datetime()
{
    std::lock_guard<std::mutex> lock(now_date_mtx);
    if (!datetime_init) return std::nullopt;
    return NOW;

    // DEBUG: 1号周一
    // return DateTime{
    //     .year=2025,
    //     .month=12,
    //     .day=1,
    //     .week=1,
    //     .first_mday_week=1,
    //     .hour=0,
    //     .minute=0,
    //     .second=0,
    // };
    // DEBUG: 31号周日
    // return DateTime{
    //     .year=2026,
    //     .month=5,
    //     .day=31,
    //     .week=7,
    //     .first_mday_week=1,
    //     .hour=0,
    //     .minute=0,
    //     .second=0,
    // };
}

void
update_now_datetime()
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;
    using std::chrono::steady_clock;

    static auto lastupdate = steady_clock::time_point{};

    auto elapsed = steady_clock::now() - lastupdate;
    if (lastupdate != steady_clock::time_point{} && elapsed < seconds(1))
        return;

    spdlog::debug(
        "Time since last update: {}ms",
        duration_cast<milliseconds>(elapsed).count()
    );

    time_t now = time(nullptr);
    struct tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif

    // 计算当月1号是星期几
    uint8_t first_mday_week;
    {
        struct tm tm_info = {};
        tm_info.tm_year = local_tm.tm_year;
        tm_info.tm_mon = local_tm.tm_mon;
        tm_info.tm_mday = 1;
        mktime(&tm_info);
        first_mday_week = tm_info.tm_wday;
    }

    std::lock_guard<std::mutex> lock(now_date_mtx);
    NOW = DateTime{
        .timestamp_s = now,
        .year = static_cast<uint16_t>(1900 + local_tm.tm_year),
        .month = static_cast<uint8_t>(1 + local_tm.tm_mon),
        .day = static_cast<uint8_t>(local_tm.tm_mday),
        .week = static_cast<uint8_t>((local_tm.tm_wday + 6) % 7 + 1),
        .first_mday_week = static_cast<uint8_t>((first_mday_week + 6) % 7 + 1),
        .hour = static_cast<uint8_t>(local_tm.tm_hour),
        .minute = static_cast<uint8_t>(local_tm.tm_min),
        .second = static_cast<uint8_t>(local_tm.tm_sec),
    };
    datetime_init = true;
    lastupdate = steady_clock::now();
}

}  // namespace core::calendar
