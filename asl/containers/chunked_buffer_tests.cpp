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
}

ASL_TEST(resize_zero)
{
    asl::chunked_buffer<int, 4> b;
    ASL_TEST_EXPECT(b.capacity() == 0);
    ASL_TEST_EXPECT(b.size() == 0);

    b.resize_zero(2);
    for (isize_t i = 0; i < 2; ++i)
    {
        ASL_TEST_EXPECT(b[i] == 0);
    }

    b.resize_zero(18);
    for (isize_t i = 0; i < 18; ++i)
    {
        ASL_TEST_EXPECT(b[i] == 0);
    }
}

ASL_TEST(resize)
{
    asl::chunked_buffer<int, 4> b;
    ASL_TEST_EXPECT(b.capacity() == 0);
    ASL_TEST_EXPECT(b.size() == 0);

    b.resize(10);
    for (isize_t i = 0; i < 10; ++i)
    {
        ASL_TEST_EXPECT(b[i] == 0);
    }

    b.resize(20, 8);
    for (isize_t i = 0; i < 10; ++i)
    {
        ASL_TEST_EXPECT(b[i] == 0);
    }

    for (isize_t i = 10; i < 20; ++i)
    {
        ASL_TEST_EXPECT(b[i] == 8);
    }
}

ASL_TEST(push)
{
    asl::chunked_buffer<int, 4> b;

    for (int i = 0; i < 100; ++i)
    {
        b.push(i);
    }

    for (int i = 0; i < 100; ++i)
    {
        ASL_TEST_EXPECT(b[i] == i);
    }

    b.resize(1000);
    for (int i = 0; i < 100; ++i)
    {
        ASL_TEST_EXPECT(b[i] == i);
    }
}

// ASL_TEST(resize_destroy)
// {
//     bool destroyed[5];
//     asl::chunked_buffer<DestructorObserver, 2> buf;

//     for (int i = 0; i < 5; ++i)
//     {
//         buf.push(&destroyed[i]);
//     }
   
//     // @Todo Test that resizing does destroys stuff
// }

// @Todo test clear actually destroys
// @Todo test destroy with alloc counter
