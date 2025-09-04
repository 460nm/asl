// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/support.hpp"

struct MyStruct
{
    size_t a;
    size_t b;
};

static_assert(sizeof(size_t) == sizeof(void*));
static_assert(sizeof(isize_t) == sizeof(void*));
static_assert(sizeof(ptrdiff_t) == sizeof(void*));
static_assert(__is_integral(ptrdiff_t) && __is_signed(ptrdiff_t));

static_assert(offsetof(MyStruct, a) == 0);
static_assert(offsetof(MyStruct, b) == 8);
static_assert(noexcept(offsetof(MyStruct, a)));

static_assert(alignof(asl::max_align_t) == alignof(void*));

int main()
{
    return 0;
}
