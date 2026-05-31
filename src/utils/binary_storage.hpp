#ifndef __BINARY_STORAGE_HPP__
#define __BINARY_STORAGE_HPP__

#include <bit>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>


// NOTICE: deprecated
namespace utils::BinaryStorage
{
using byte = unsigned char;

struct IBinarySerializable
{
    virtual size_t
    serialize_self(
        std::vector<byte>& target, size_t pos_start
    ) = 0;
};

template <typename T>
size_t
serialize(
    std::vector<byte>& target, size_t pos_start, const T& data
)
{
    // TODO: 数据头拼接
    throw std::runtime_error("function not implemented");

    if constexpr (std::is_base_of_v<IBinarySerializable, T>)
        return data.write_self_bytes(target);

    // 确保是 POD 或平铺类型
    static_assert(
        std::is_standard_layout<T>::value,
        "Only POD types are supported."
    );

    static_assert(sizeof(T) <= 64, "size of T must less than 64");
    static_assert(
        std::is_trivially_copyable<T>::value,
        "T must be trivially copyable, or implemented "
        "IBinarySerializable"
    );

    if (target.size() < pos_start + sizeof(T))
        target.resize(pos_start + sizeof(T));

    // 强行视为字节数组处理
    const byte* _data = reinterpret_cast<const byte*>(&data);

    for (size_t i = 0; i < sizeof(T); ++i)
        if constexpr (std::endian::native == std::endian::big)
            target[pos_start + i] = _data[i];
        else
            target[pos_start + i] = _data[sizeof(T) - i - 1];

    return sizeof(T);
}


}  // namespace BinaryStorage

#endif
