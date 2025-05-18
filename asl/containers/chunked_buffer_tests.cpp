// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"
#include "asl/containers/chunked_buffer.hpp"

static_assert(asl::moveable<asl::chunked_buffer<int, 8>>);
static_assert(asl::moveable<asl::chunked_buffer<Copyable, 8>>);
static_assert(asl::moveable<asl::chunked_buffer<MoveableOnly, 8>>);
static_assert(asl::moveable<asl::chunked_buffer<Pinned, 8>>);

static_assert(asl::copyable<asl::chunked_buffer<int, 8>>);
static_assert(asl::copyable<asl::chunked_buffer<Copyable, 8>>);
static_assert(!asl::copyable<asl::chunked_buffer<MoveableOnly, 8>>);
static_assert(!asl::copyable<asl::chunked_buffer<Pinned, 8>>);

ASL_TEST(test)
{
    asl::chunked_buffer<int, 16> b;
    b.destroy();
}

// @Todo test clear actually destroys
// @Todo test destroy with alloc counter
