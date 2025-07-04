// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/config.hpp"

using int8_t  = signed char;
using int16_t = signed short;
using int32_t = signed int;
#if ASL_OS_WINDOWS
    using int64_t = signed long long;
#elif ASL_OS_LINUX
    using int64_t = signed long;
#endif

using uint8_t  = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
#if ASL_OS_WINDOWS
    using uint64_t = unsigned long long;
#elif ASL_OS_LINUX
    using uint64_t = unsigned long;
#endif

struct uint128_t
{
    uint64_t high;
    uint64_t low;
};

using size_t  = uint64_t;
using isize_t = int64_t;

using uintptr_t = size_t;

namespace asl
{

enum class byte : uint8_t {};

template<typename T> struct integer_traits {};

#define ASL_INTEGER_TRAITS(T, MIN, MAX) \
    template<> struct integer_traits<T> \
    {                                   \
        static constexpr T kMin = MIN;  \
        static constexpr T kMax = MAX;  \
    }

ASL_INTEGER_TRAITS(uint8_t,   0,                     0xff);
ASL_INTEGER_TRAITS(uint16_t,  0,                     0xffff);
ASL_INTEGER_TRAITS(uint32_t,  0,                     0xffff'ffff);
ASL_INTEGER_TRAITS(uint64_t,  0,                     0xffff'ffff'ffff'ffff);
ASL_INTEGER_TRAITS(int8_t,   -0x80,                  0x7f);
ASL_INTEGER_TRAITS(int16_t,  -0x8000,                0x7fff);
ASL_INTEGER_TRAITS(int32_t,  -0x8000'0000,           0x7fff'ffff);
ASL_INTEGER_TRAITS(int64_t,  -0x8000'0000'0000'0000, 0x7fff'ffff'ffff'ffff);

#undef ASL_INTEGER_TRAITS

} // namespace asl

