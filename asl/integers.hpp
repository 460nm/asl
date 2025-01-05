#pragma once

#include "asl/config.hpp"

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

} // namespace asl

