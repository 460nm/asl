// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"

namespace asl
{

using AssertFailureHandler = void (const char* msg, const source_location&, void* user);

void set_assert_failure_handler(AssertFailureHandler handler, void* user);

void report_assert_failure(const char* msg, const source_location& sl = source_location{});

} // namespace asl

#define ASL_DEBUG_BREAK() __builtin_debugtrap()

#if !ASL_OPTIMIZED
    #define ASL_ASSERT(...)                                                 \
        if (__VA_ARGS__) {}                                                 \
        else                                                                \
        {                                                                   \
            ::asl::report_assert_failure(#__VA_ARGS__);                     \
            ASL_DEBUG_BREAK();                                              \
        }
#else
    #define ASL_ASSERT(...)
#endif

#define ASL_ASSERT_RELEASE(...)                                         \
    if (__VA_ARGS__) {}                                                 \
    else                                                                \
    {                                                                   \
        ::asl::report_assert_failure(#__VA_ARGS__);                     \
        ASL_DEBUG_BREAK();                                              \
    }
