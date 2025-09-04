// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/integers.hpp"

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int64_t) == 8);

static_assert(__is_integral(int8_t) && __is_signed(int8_t));
static_assert(__is_integral(int16_t) && __is_signed(int16_t));
static_assert(__is_integral(int32_t) && __is_signed(int32_t));
static_assert(__is_integral(int64_t) && __is_signed(int64_t));

static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

static_assert(__is_integral(uint8_t) && __is_unsigned(uint8_t));
static_assert(__is_integral(uint16_t) && __is_unsigned(uint16_t));
static_assert(__is_integral(uint32_t) && __is_unsigned(uint32_t));
static_assert(__is_integral(uint64_t) && __is_unsigned(uint64_t));

static_assert(sizeof(uintptr_t) == sizeof(void*));
static_assert(__is_integral(uintptr_t) && __is_unsigned(uintptr_t));

int main()
{
    return 0;
}
