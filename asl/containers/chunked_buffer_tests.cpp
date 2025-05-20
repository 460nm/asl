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

ASL_TEST(reserve)
{
    asl::chunked_buffer<int, 16> b;
    ASL_TEST_EXPECT(b.capacity() == 0);
    ASL_TEST_EXPECT(b.size() == 0);

    b.reserve_capacity(1);
    ASL_TEST_EXPECT(b.capacity() == 16);
    ASL_TEST_EXPECT(b.size() == 0);

    b.reserve_capacity(5);
    ASL_TEST_EXPECT(b.capacity() == 16);
    ASL_TEST_EXPECT(b.size() == 0);

    b.reserve_capacity(16);
    ASL_TEST_EXPECT(b.capacity() == 16);
    ASL_TEST_EXPECT(b.size() == 0);

    b.reserve_capacity(35);
    ASL_TEST_EXPECT(b.capacity() == 48);
    ASL_TEST_EXPECT(b.size() == 0);

    b.reserve_capacity(12);
    ASL_TEST_EXPECT(b.capacity() == 48);
    ASL_TEST_EXPECT(b.size() == 0);
}

ASL_TEST(resize_uninit)
{
    asl::chunked_buffer<int, 16> b;
    ASL_TEST_EXPECT(b.capacity() == 0);
    ASL_TEST_EXPECT(b.size() == 0);

    b.resize_uninit(1);
    ASL_TEST_EXPECT(b.capacity() == 16);
    ASL_TEST_EXPECT(b.size() == 1);

    b.resize_uninit(5);
    ASL_TEST_EXPECT(b.capacity() == 16);
    ASL_TEST_EXPECT(b.size() == 5);

    b.resize_uninit(16);
    ASL_TEST_EXPECT(b.capacity() == 16);
    ASL_TEST_EXPECT(b.size() == 16);

    b.resize_uninit(35);
    ASL_TEST_EXPECT(b.capacity() == 48);
    ASL_TEST_EXPECT(b.size() == 35);

    b.resize_uninit(12);
    ASL_TEST_EXPECT(b.capacity() == 48);
    ASL_TEST_EXPECT(b.size() == 12);

    // @Todo Test that resizing down destroys stuff
}

// @Todo test clear actually destroys
// @Todo test destroy with alloc counter
