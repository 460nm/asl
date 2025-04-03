// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/numeric.hpp"

#include "asl/testing/testing.hpp"

ASL_TEST(is_pow2)
{
    ASL_TEST_EXPECT(asl::is_pow2(4));
    ASL_TEST_EXPECT(asl::is_pow2(65536));
    ASL_TEST_EXPECT(!asl::is_pow2(6));
    ASL_TEST_EXPECT(!asl::is_pow2(1978));
    ASL_TEST_EXPECT(!asl::is_pow2(0));
}
