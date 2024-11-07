#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"

namespace asl
{

template<typename T>
inline constexpr isize_t size_of = static_cast<isize_t>(sizeof(T));

template<typename T>
inline constexpr isize_t align_of = static_cast<isize_t>(alignof(T));

struct layout
{
    isize_t size;
    isize_t align;

    constexpr bool operator==(const layout&) const = default;

    template<is_object T>
    static constexpr layout of()
    {
        return layout{ size_of<T>, align_of<T> };
    }
};

} // namespace asl
