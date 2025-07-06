// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <fast_float.h>
#pragma clang diagnostic pop

// We need to isolate fast_float.h completely from asl
// because it conflicts with our redefinitions of things
// from the STL. In this case it's operator new, but there
// might be other conflicts.

#pragma clang diagnostic ignored "-Wmissing-prototypes"

namespace asl
{

extern bool parse_float_impl(const char** begin, const char* end, float* value)
{
    auto res = fast_float::from_chars(*begin, end, *value);
    *begin = res.ptr;
    return res.ec == std::errc{};
}

extern bool parse_double_impl(const char** begin, const char* end, double* value)
{
    auto res = fast_float::from_chars(*begin, end, *value);
    *begin = res.ptr;
    return res.ec == std::errc{};
}

} // namespace asl

