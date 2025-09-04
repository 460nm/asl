// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"
#include "asl/tests/counting_allocator.hpp"
#include "asl/containers/chunked_buffer.hpp"

static_assert(asl::movable<asl::chunked_buffer<int, 8>>);
static_assert(asl::movable<asl::chunked_buffer<Copyable, 8>>);
static_assert(asl::movable<asl::chunked_buffer<MoveableOnly, 8>>);
static_assert(asl::movable<asl::chunked_buffer<Pinned, 8>>);

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

ASL_TEST(pop)
{
    asl::chunked_buffer<int, 2> b;

    for (int i = 0; i < 8; ++i)
    {
        b.push(i);
    }
    ASL_TEST_EXPECT(b.size() == 8);

    b.pop();
    ASL_TEST_EXPECT(b.size() == 7);
    for (int i = 0; i < 7; ++i)
    {
        ASL_TEST_EXPECT(b[i] == i);
    }
}

ASL_TEST(pop_destruct)
{
    bool d[3]{};
    asl::chunked_buffer<DestructorObserver, 16> b;

    b.push(&d[0]);
    b.push(&d[1]);
    b.push(&d[2]);

    ASL_TEST_EXPECT(!d[0]);
    ASL_TEST_EXPECT(!d[1]);
    ASL_TEST_EXPECT(!d[2]);

    b.pop();
    ASL_TEST_EXPECT(!d[0]);
    ASL_TEST_EXPECT(!d[1]);
    ASL_TEST_EXPECT(d[2]);

    b.pop();
    ASL_TEST_EXPECT(!d[0]);
    ASL_TEST_EXPECT(d[1]);
    ASL_TEST_EXPECT(d[2]);
}

ASL_TEST(clear_destroy)
{
    bool destroyed[5]{};
    asl::chunked_buffer<DestructorObserver, 2> buf;

    for (bool& d: destroyed)
    {
        buf.push(&d); // NOLINT
    }

    for (const bool d: destroyed)
    {
        ASL_TEST_EXPECT(!d);
    }

    buf.clear();

    for (const bool d: destroyed)
    {
        ASL_TEST_EXPECT(d);
    }
}

ASL_TEST(alloc_count) // NOLINT
{
    CountingAllocator::Stats stats;
    asl::chunked_buffer<int, 4, CountingAllocator> buf{CountingAllocator{&stats}};

    ASL_TEST_EXPECT(stats.alive_bytes == 0);
    ASL_TEST_EXPECT(stats.alloc_count == 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 0);

    buf.push(1);
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 1);

    buf.push(2);
    buf.push(3);
    buf.push(4);
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 1);

    buf.push(5);
    buf.push(6);
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 2);

    buf.resize(8, 8);
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 2);

    buf.resize(32, 8);
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 9);

    buf.resize(16, 0);
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 9);

    buf.clear();
    ASL_TEST_EXPECT(stats.alive_bytes > 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 0);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 9);

    buf.destroy();
    ASL_TEST_EXPECT(stats.alive_bytes == 0);
    ASL_TEST_EXPECT(stats.dealloc_count == 9);
    ASL_TEST_EXPECT(stats.any_alloc_count() == 9);
}

ASL_TEST(move)
{
    bool destroyed[5]{};

    {
        asl::chunked_buffer<DestructorObserver, 2> buf;

        for (bool& d: destroyed)
        {
            buf.push(&d); // NOLINT
        }

        for (const bool d: destroyed)
        {
            ASL_TEST_EXPECT(!d);
        }

        asl::chunked_buffer<DestructorObserver, 2> buf2 = std::move(buf);

        for (const bool d: destroyed)
        {
            ASL_TEST_EXPECT(!d);
        }

        buf = std::move(buf2);
        buf2.destroy();

        for (const bool d: destroyed)
        {
            ASL_TEST_EXPECT(!d);
        }
    }

    for (const bool d: destroyed)
    {
        ASL_TEST_EXPECT(d);
    }
}

ASL_TEST(copy) // NOLINT
{
    asl::chunked_buffer<int, 4> buf;
    for (int i = 0; i < 10; ++i) { buf.push(i); }

    asl::chunked_buffer<int, 4> buf2 = buf;

    ASL_TEST_EXPECT(buf.size() == 10);
    ASL_TEST_EXPECT(buf2.size() == 10);
    for (int i = 0; i < 10; ++i)
    {
        ASL_TEST_EXPECT(buf[i] == i);
        ASL_TEST_EXPECT(buf2[i] == i);
    }

    buf2.resize(5);
    buf = buf2;

    ASL_TEST_EXPECT(buf.size() == 5);
    ASL_TEST_EXPECT(buf2.size() == 5);
    for (int i = 0; i < 5; ++i)
    {
        ASL_TEST_EXPECT(buf[i] == i);
        ASL_TEST_EXPECT(buf2[i] == i);
    }

    buf.clear();
    buf.resize(80, 12);
    buf2 = buf;
    ASL_TEST_EXPECT(buf.size() == 80);
    ASL_TEST_EXPECT(buf2.size() == 80);
    for (int i = 0; i < 80; ++i)
    {
        ASL_TEST_EXPECT(buf[i] == 12);
        ASL_TEST_EXPECT(buf2[i] == 12);
    }
}

ASL_TEST(iterator)
{
    asl::chunked_buffer<int, 4> buf;
    for (int i = 0; i < 30; ++i) { buf.push(100 + i); }

    auto it = buf.begin();
    auto end = buf.end();
    for (int i = 0; i < 30; ++i)
    {
        ASL_TEST_ASSERT(it != end);
        ASL_TEST_EXPECT(*it == 100 + i);
        it++;
    }
    ASL_TEST_EXPECT(it == end);

    static_assert(asl::same_as<decltype(*it), int&>);

    asl::chunked_buffer<int, 8> buf2;
    ASL_TEST_EXPECT(buf2.begin() == buf2.end());
}

ASL_TEST(const_iterator)
{
    asl::chunked_buffer<int, 4> buf_value;
    for (int i = 0; i < 30; ++i) { buf_value.push(100 + i); }

    const auto& buf = buf_value;

    auto it = buf.begin();
    auto end = buf.end();
    for (int i = 0; i < 30; ++i)
    {
        ASL_TEST_ASSERT(it != end);
        ASL_TEST_EXPECT(*it == 100 + i);
        ++it;
    }
    ASL_TEST_EXPECT(it == end);

    static_assert(asl::same_as<decltype(*it), const int&>);

    asl::chunked_buffer<int, 8> buf2;
    ASL_TEST_EXPECT(buf2.begin() == buf2.end());
}

