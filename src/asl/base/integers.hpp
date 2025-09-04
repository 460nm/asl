// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/config.hpp"
#include "asl/base/meta.hpp"

// [support.arith.types] [cstdint.syn]

using int8_t  = signed char;
using int16_t = signed short int;
using int32_t = int;
#ifdef ASL_OS_WINDOWS
    using int64_t = signed long long int;
#elifdef ASL_OS_LINUX
    using int64_t = signed long int;
#endif

using uint8_t  = unsigned char;
using uint16_t = unsigned short int;
using uint32_t = unsigned int;
#ifdef ASL_OS_WINDOWS
    using uint64_t = unsigned long long int;
#elifdef ASL_OS_LINUX
    using uint64_t = unsigned long int;
#endif

using uintptr_t = uint64_t;

struct uint128_t
{
    uint64_t high;
    uint64_t low;
};

template<>
struct asl::has_unique_object_representations<uint128_t> : true_type {};

namespace asl
{

template<typename T> struct integer_traits {};

#define ASL_INTEGER_TRAITS(T, MIN, MAX)               \
    template<> struct integer_traits<T>               \
    {                                                 \
        static constexpr T kMin{static_cast<T>(MIN)}; \
        static constexpr T kMax{static_cast<T>(MAX)}; \
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


