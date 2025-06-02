// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/handle_pool/index_pool.hpp"

enum TestHandleType : uint8_t
{
    kType1,
    kType2,
};

using TestHandle = asl::index_pool_handle<10, 14, TestHandleType>;
using Pool = asl::IndexPool<TestHandle>;

ASL_TEST(test)
{
}

