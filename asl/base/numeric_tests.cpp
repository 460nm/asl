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
    ASL_TEST_EXPECT(asl::is_pow2(4U));
    ASL_TEST_EXPECT(asl::is_pow2(uint64_t{65536}));
}

ASL_TEST(round_down_pow2) // NOLINT
{
    ASL_TEST_EXPECT(asl::round_down_pow2(0, 1) == 0);
    ASL_TEST_EXPECT(asl::round_down_pow2(1, 1) == 1);
    ASL_TEST_EXPECT(asl::round_down_pow2(2, 1) == 2);
    ASL_TEST_EXPECT(asl::round_down_pow2(3, 1) == 3);
    ASL_TEST_EXPECT(asl::round_down_pow2(-1, 1) == -1);
    ASL_TEST_EXPECT(asl::round_down_pow2(-2, 1) == -2);
    ASL_TEST_EXPECT(asl::round_down_pow2(-3, 1) == -3);

    ASL_TEST_EXPECT(asl::round_down_pow2(0U, 1U) == 0U);
    ASL_TEST_EXPECT(asl::round_down_pow2(1U, 1U) == 1U);
    ASL_TEST_EXPECT(asl::round_down_pow2(2U, 1U) == 2U);
    ASL_TEST_EXPECT(asl::round_down_pow2(3U, 1U) == 3U);

    ASL_TEST_EXPECT(asl::round_down_pow2(0, 16) == 0);
    ASL_TEST_EXPECT(asl::round_down_pow2(1, 16) == 0);
    ASL_TEST_EXPECT(asl::round_down_pow2(8, 16) == 0);
    ASL_TEST_EXPECT(asl::round_down_pow2(15, 16) == 0);
    ASL_TEST_EXPECT(asl::round_down_pow2(16, 16) == 16);
    ASL_TEST_EXPECT(asl::round_down_pow2(17, 16) == 16);
    ASL_TEST_EXPECT(asl::round_down_pow2(255, 16) == 240);
    ASL_TEST_EXPECT(asl::round_down_pow2(-255, 16) == -256);
}

ASL_TEST(round_up_pow2) // NOLINT
{
    ASL_TEST_EXPECT(asl::round_up_pow2(0, 1) == 0);
    ASL_TEST_EXPECT(asl::round_up_pow2(1, 1) == 1);
    ASL_TEST_EXPECT(asl::round_up_pow2(2, 1) == 2);
    ASL_TEST_EXPECT(asl::round_up_pow2(3, 1) == 3);
    ASL_TEST_EXPECT(asl::round_up_pow2(-1, 1) == -1);
    ASL_TEST_EXPECT(asl::round_up_pow2(-2, 1) == -2);
    ASL_TEST_EXPECT(asl::round_up_pow2(-3, 1) == -3);

    ASL_TEST_EXPECT(asl::round_up_pow2(0U, 1U) == 0U);
    ASL_TEST_EXPECT(asl::round_up_pow2(1U, 1U) == 1U);
    ASL_TEST_EXPECT(asl::round_up_pow2(2U, 1U) == 2U);
    ASL_TEST_EXPECT(asl::round_up_pow2(3U, 1U) == 3U);

    ASL_TEST_EXPECT(asl::round_up_pow2(0, 16) == 0);
    ASL_TEST_EXPECT(asl::round_up_pow2(1, 16) == 16);
    ASL_TEST_EXPECT(asl::round_up_pow2(8, 16) == 16);
    ASL_TEST_EXPECT(asl::round_up_pow2(15, 16) == 16);
    ASL_TEST_EXPECT(asl::round_up_pow2(16, 16) == 16);
    ASL_TEST_EXPECT(asl::round_up_pow2(17, 16) == 32);
    ASL_TEST_EXPECT(asl::round_up_pow2(255, 16) == 256);
    ASL_TEST_EXPECT(asl::round_up_pow2(-255, 16) == -240);
}
