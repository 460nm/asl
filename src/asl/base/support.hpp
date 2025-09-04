// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

// [support.types.nullptr]

using nullptr_t = decltype(nullptr);

// [support.types.layout]

using size_t = unsigned long long;
using isize_t = long long;
using ptrdiff_t = isize_t;

#define offsetof(Type, Member) (__builtin_offsetof(Type, Member))

namespace asl
{

struct alignas(void*) max_align_t {};

} // namespace asl
