#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"

namespace asl
{

template<typename T>
inline constexpr int64_t size_of = static_cast<int64_t>(sizeof(T));

template<typename T>
inline constexpr int64_t align_of = static_cast<int64_t>(alignof(T));

struct layout
{
    int64_t size;
    int64_t align;

    constexpr bool operator==(const layout&) const = default;

    template<is_object T>
    static constexpr layout of()
    {
        return layout{ size_of<T>, align_of<T> };
    }
};

} // namespace asl
