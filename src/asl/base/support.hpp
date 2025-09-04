// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/config.hpp"

// [support.types.nullptr]

using nullptr_t = decltype(nullptr);

// [support.types.layout]

#if defined(ASL_OS_WINDOWS)
    using size_t = unsigned long long;
    using isize_t = signed long long;
#elif defined(ASL_OS_LINUX)
    using size_t = unsigned long;
    using isize_t = signed long;
#endif

using ptrdiff_t = isize_t;

#define offsetof(Type, Member) (__builtin_offsetof(Type, Member))

#if defined(ASL_COMPILER_CLANG_CL)
    #define ASL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif defined(ASL_COMPILER_CLANG)
    #define ASL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace asl
{

struct alignas(void*) max_align_t {};

struct source_location
{
    const char* file;
    int         line;

    explicit source_location(
        const char* file_ = __builtin_FILE(),
        int line_ = __builtin_LINE())
        : file{file_}
        , line{line_}
    {}
};

} // namespace asl

// NOLINTBEGIN(*-macro-parentheses)
#define ASL_DELETE_COPY(T)                         \
    T(const T&) = delete;                          \
    T& operator=(const T&) = delete

#define ASL_DELETE_MOVE(T)                         \
    T(T&&) = delete;                               \
    T& operator=(T&&) = delete

#define ASL_DELETE_COPY_MOVE(T)                    \
    ASL_DELETE_COPY(T);                            \
    ASL_DELETE_MOVE(T)

#define ASL_DEFAULT_COPY(T)                         \
    T(const T&) = default;                          \
    T& operator=(const T&) = default

#define ASL_DEFAULT_MOVE(T)                         \
    T(T&&) = default;                               \
    T& operator=(T&&) = default

#define ASL_DEFAULT_COPY_MOVE(T)                    \
    ASL_DEFAULT_COPY(T);                            \
    ASL_DEFAULT_MOVE(T)
// NOLINTEND(*-macro-parentheses)

#define ASL_CONCAT2(A, B) A##B
#define ASL_CONCAT(A, B) ASL_CONCAT2(A, B)

