#ifndef __UI_UTILS_HPP__
#define __UI_UTILS_HPP__

#include <atomic>

namespace utils
{

// get a uint64_t id, it will incrementally generated in a circle
inline std::atomic<uint64_t> _ID_COUNTER = 0;

inline uint64_t
gen_id()
{
    return _ID_COUNTER++;
}

}  // namespace ui::Utils

#endif
