// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

// NOLINTBEGIN(*-macro-to-enum)

#pragma once

#if defined(_WIN32)
    #define ASL_OS_WINDOWS 1
#elif defined(__linux__)
    #define ASL_OS_LINUX 1
#else
    #error Unknown OS
#endif

#if defined(__clang__) && defined(_MSC_VER)
    #define ASL_COMPILER_CLANG_CL 1
#elif defined(__clang__)
    #define ASL_COMPILER_CLANG 1
#else
    #error Unknown compiler
#endif

// ASL_DEBUG=1 for slow builds, with extra validation logic and such.
#if !defined(ASL_DEBUG)
    #error ASL_DEBUG should be defined to 0 or 1
#endif

// ASL_OPTIMIZED=1 for fast builds, with minimal validation logic.
#if !defined(ASL_OPTIMIZED)
    #error ASL_OPTIMIZED should be defined to 0 or 1
#endif

// NOLINTEND(*-macro-to-enum)

