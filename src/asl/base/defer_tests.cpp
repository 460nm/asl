// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/defer.hpp"
#include "asl/testing/testing.hpp"

ASL_TEST(defer)
{
    uint32_t a = 0;

    {
        ASL_DEFER [&a]() { a |= 1U; };
        ASL_TEST_EXPECT(a == 0);

        {
            ASL_DEFER [&a]() { a |= 2U; };
            ASL_DEFER [&a]() { a |= 4U; };
            ASL_TEST_EXPECT(a == 0);
        }

        ASL_TEST_EXPECT(a == 6);

        {
            ASL_DEFER [&a]() { a |= 8U; };
            ASL_TEST_EXPECT(a == 6);
        }

        ASL_TEST_EXPECT(a == 14);

        ASL_DEFER [&a]() { a |= 16U; };
        ASL_TEST_EXPECT(a == 14);
    }

    ASL_TEST_EXPECT(a == 31);
}
