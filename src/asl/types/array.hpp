// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/assert.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/support.hpp"
#include "asl/types/span.hpp"

namespace asl
{

template<is_object T, isize_t kSize>
requires (kSize > 0)
struct array
{
    T m_data[kSize];

    [[nodiscard]] constexpr bool is_empty() const { return false; }

    [[nodiscard]] constexpr isize_t size() const { return kSize; }

    constexpr auto data(this auto&& self)
    {
        using return_type = copy_const_t<remove_ref_t<decltype(self)>, T>*;
        return static_cast<return_type>(self.m_data);
    }

    constexpr auto begin(this auto&& self)
    {
        return contiguous_iterator{self.data()};
    }

    constexpr auto end(this auto&& self)
    {
        return contiguous_iterator{self.data() + kSize};
    }

    template<isize_t kSpanSize>
    requires (kSpanSize == kSize || kSpanSize == dynamic_size)
    constexpr operator span<const T, kSpanSize>() const // NOLINT(*explicit*)
    {
        return as_span();
    }

    template<isize_t kSpanSize>
    requires (kSpanSize == kSize || kSpanSize == dynamic_size)
    constexpr operator span<T, kSpanSize>() // NOLINT(*explicit*)
    {
        return as_span();
    }

    constexpr auto as_span(this auto&& self)
    {
        using type = copy_const_t<remove_ref_t<decltype(self)>, T>;
        return span<type, kSize>{self.data(), self.size()};
    }

    constexpr auto&& operator[](this auto&& self, isize_t i)
    {
        ASL_ASSERT(i >= 0 && i <= self.size());
        return std::forward_like<decltype(self)>(std::forward<decltype(self)>(self).data()[i]);
    }
};


} // namespace asl

