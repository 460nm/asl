// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/meta.hpp"
#include "asl/base/byte.hpp"

static_assert(sizeof(std::byte) == 1);
static_assert(alignof(std::byte) == 1);

static_assert(to_integer<int>(std::byte{0}) == 0);
static_assert(to_integer<int>(std::byte{45}) == 45);
static_assert(to_integer<int>(std::byte{255}) == 255);

static_assert(to_integer<int>(std::byte{1} << 3) == 8);
static_assert(to_integer<int>(std::byte{255} << 2) == 252);

static_assert(to_integer<int>(std::byte{4} >> 16) == 0);
static_assert(to_integer<int>(std::byte{255} >> 2) == 63);

static_assert(to_integer<int>(std::byte{1} | std::byte{5}) == 5);
static_assert(to_integer<int>(std::byte{1} & std::byte{5}) == 1);
static_assert(to_integer<int>(std::byte{1} ^ std::byte{5}) == 4);
static_assert(to_integer<int>(~std::byte{3}) == 252);

static_assert(asl::has_unique_object_representations_v<std::byte>);

int main()
{
    return 0;
}
