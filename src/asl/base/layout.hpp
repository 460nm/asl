// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/meta.hpp"

namespace asl
{

struct layout
{
    isize_t size;
    isize_t align;

    constexpr bool operator==(const layout&) const = default;

    template<is_object T>
    static constexpr layout of()
    {
        return layout{
            .size = static_cast<isize_t>(sizeof(T)),
            .align = static_cast<isize_t>(alignof(T)),
        };
    }

    template<is_object T>
    static constexpr layout array(isize_t size)
    {
        return layout{
            .size = static_cast<isize_t>(sizeof(T)) * size,
            .align = static_cast<isize_t>(alignof(T)),
        };
    }
};

} // namespace asl

