#ifndef __CORE_CALENDAR_HPP__
#define __CORE_CALENDAR_HPP__


#include <cstdint>
#include <ctime>
#include <vulkan/vulkan_core.h>
#include <optional>

namespace core::calendar
{
struct DateTime
{
    time_t timestamp_s;
    uint16_t year;
    // [1, 12]
    uint8_t month;
    // [1, 31]
    uint8_t day;
    // [1, 7], start at monday
    uint8_t week;
    // [1, 7], 当前月份第一天是周几, start at Monday
    uint8_t first_mday_week;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

inline DateTime NOW;

std::optional<DateTime>
get_now_datetime();

void
update_now_datetime();

}  // namespace core::calendar

#endif
