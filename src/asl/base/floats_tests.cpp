// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/floats.hpp"

static_assert(sizeof(float32_t) == 4);
static_assert(sizeof(float64_t) == 8);

static_assert(__is_floating_point(float32_t));
static_assert(__is_floating_point(float64_t));

int main()
{
    return 0;
}
