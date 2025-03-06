// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/containers/buffer.hpp"

#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"

struct Big
{
    uint64_t data[8];
};

static_assert(asl::buffer<int32_t>::kInlineCapacity == 5);
static_assert(asl::buffer<int64_t>::kInlineCapacity == 2);
static_assert(asl::buffer<char>::kInlineCapacity == 23);
static_assert(asl::buffer<Big>::kInlineCapacity == 0);

ASL_TEST(default_size)
{
    asl::buffer<int32_t>  b1;
    ASL_TEST_EXPECT(b1.size() == 0);
    ASL_TEST_EXPECT(b1.capacity() == 5);
    ASL_TEST_EXPECT(static_cast<const void*>(b1.data()) == &b1);

    asl::buffer<Big> b2;
    ASL_TEST_EXPECT(b2.size() == 0);
    ASL_TEST_EXPECT(b2.capacity() == 0);
    ASL_TEST_EXPECT(b2.data() == nullptr);
}

struct CounterAllocator
{
    isize_t* count;

    void* alloc(const asl::layout& layout) const
    {
        *count += 1;
        return asl::GlobalHeap::alloc(layout);
    }

    void* realloc(void* ptr, const asl::layout& old, const asl::layout& new_layout) const
    {
        *count += 1;
        return asl::GlobalHeap::realloc(ptr, old, new_layout);
    }

    static void dealloc(void* ptr, const asl::layout& layout)
    {
        asl::GlobalHeap::dealloc(ptr, layout);
    }

    constexpr bool operator==(const CounterAllocator&) const { return true; }
};
static_assert(asl::allocator<CounterAllocator>);

struct IncompatibleAllocator
{
    static void* alloc(const asl::layout& layout)
    {
        return asl::GlobalHeap::alloc(layout);
    }

    static void* realloc(void* ptr, const asl::layout& old, const asl::layout& new_layout)
    {
        return asl::GlobalHeap::realloc(ptr, old, new_layout);
    }

    static void dealloc(void* ptr, const asl::layout& layout)
    {
        asl::GlobalHeap::dealloc(ptr, layout);
    }

    constexpr bool operator==(const IncompatibleAllocator&) const { return false; }
};
static_assert(asl::allocator<IncompatibleAllocator>);

ASL_TEST(reserve_capacity)
{
    isize_t count = 0;
    asl::buffer<int32_t, CounterAllocator> b(CounterAllocator{&count});
    ASL_TEST_EXPECT(b.size() == 0);
    ASL_TEST_EXPECT(b.capacity() == 5);
    ASL_TEST_EXPECT(count == 0);

    b.reserve_capacity(4);
    ASL_TEST_EXPECT(b.size() == 0);
    ASL_TEST_EXPECT(b.capacity() == 5);
    ASL_TEST_EXPECT(count == 0);

    b.reserve_capacity(12);
    ASL_TEST_EXPECT(b.size() == 0);
    ASL_TEST_EXPECT(b.capacity() >= 12);
    ASL_TEST_EXPECT(count == 1);

    b.reserve_capacity(13);
    ASL_TEST_EXPECT(b.size() == 0);
    ASL_TEST_EXPECT(b.capacity() >= 13);
    ASL_TEST_EXPECT(count == 1);

    b.reserve_capacity(130);
    ASL_TEST_EXPECT(b.size() == 0);
    ASL_TEST_EXPECT(b.capacity() >= 130);
    ASL_TEST_EXPECT(count == 2);
}

ASL_TEST(push)
{
    asl::buffer<int32_t> b;
    ASL_TEST_EXPECT(b.size() == 0);

    b.push(1);
    ASL_TEST_EXPECT(b.size() == 1);
    ASL_TEST_EXPECT(b[0] == 1);

    b.push(2);
    b.push(3);
    ASL_TEST_EXPECT(b.size() == 3);
    ASL_TEST_EXPECT(b[0] == 1);
    ASL_TEST_EXPECT(b[1] == 2);
    ASL_TEST_EXPECT(b[2] == 3);

    b.push(4);
    b.push(5);
    b.push(6);
    b.push(7);
    ASL_TEST_EXPECT(b.size() == 7);
    ASL_TEST_EXPECT(b[0] == 1);
    ASL_TEST_EXPECT(b[1] == 2);
    ASL_TEST_EXPECT(b[2] == 3);
    ASL_TEST_EXPECT(b[3] == 4);
    ASL_TEST_EXPECT(b[4] == 5);
    ASL_TEST_EXPECT(b[5] == 6);
    ASL_TEST_EXPECT(b[6] == 7);
}

ASL_TEST(from_span)
{
    int data[] = {1, 2, 4, 8};
    asl::buffer<int> b{data};

    ASL_TEST_EXPECT(b.size() == 4);
    ASL_TEST_EXPECT(b[0] == 1);
    ASL_TEST_EXPECT(b[1] == 2);
    ASL_TEST_EXPECT(b[2] == 4);
    ASL_TEST_EXPECT(b[3] == 8);
}

struct MoveableType
{
    int moved{};
    int value;

    explicit MoveableType(int x) : value{x} {}
    MoveableType(const MoveableType&) = delete;
    MoveableType(MoveableType&& other)  : moved{other.moved + 1}, value{other.value} {}
    MoveableType& operator=(const MoveableType&) = delete;
    MoveableType& operator=(MoveableType&&) = delete;
};
static_assert(!asl::trivially_copy_constructible<MoveableType>);
static_assert(!asl::trivially_move_constructible<MoveableType>);
static_assert(!asl::copyable<MoveableType>);
static_assert(asl::move_constructible<MoveableType>);

ASL_TEST(push_move)
{
    asl::buffer<MoveableType> b;

    static_assert(asl::buffer<MoveableType>::kInlineCapacity > 0);

    b.push(0);
    ASL_TEST_EXPECT(b[0].value == 0);
    ASL_TEST_EXPECT(b[0].moved == 0);

    b.push(1);
    ASL_TEST_EXPECT(b[0].value == 0);
    ASL_TEST_EXPECT(b[0].moved == 0);
    ASL_TEST_EXPECT(b[1].value == 1);
    ASL_TEST_EXPECT(b[1].moved == 0);

    b.push(2);
    ASL_TEST_EXPECT(b[0].value == 0);
    ASL_TEST_EXPECT(b[0].moved == 1);
    ASL_TEST_EXPECT(b[1].value == 1);
    ASL_TEST_EXPECT(b[1].moved == 1);
    ASL_TEST_EXPECT(b[2].value == 2);
    ASL_TEST_EXPECT(b[2].moved == 0);

    b.push(3);
    ASL_TEST_EXPECT(b[0].value == 0);
    ASL_TEST_EXPECT(b[0].moved == 1);
    ASL_TEST_EXPECT(b[1].value == 1);
    ASL_TEST_EXPECT(b[1].moved == 1);
    ASL_TEST_EXPECT(b[2].value == 2);
    ASL_TEST_EXPECT(b[2].moved == 0);
    ASL_TEST_EXPECT(b[3].value == 3);
    ASL_TEST_EXPECT(b[3].moved == 0);

    b.push(4);
    ASL_TEST_EXPECT(b[0].value == 0);
    ASL_TEST_EXPECT(b[0].moved == 2);
    ASL_TEST_EXPECT(b[1].value == 1);
    ASL_TEST_EXPECT(b[1].moved == 2);
    ASL_TEST_EXPECT(b[2].value == 2);
    ASL_TEST_EXPECT(b[2].moved == 1);
    ASL_TEST_EXPECT(b[3].value == 3);
    ASL_TEST_EXPECT(b[3].moved == 1);
    ASL_TEST_EXPECT(b[4].value == 4);
    ASL_TEST_EXPECT(b[4].moved == 0);
}

ASL_TEST(clear)
{
    asl::buffer<int32_t> b;
    ASL_TEST_EXPECT(b.size() == 0);

    b.push(1);
    b.push(2);
    b.push(3);
    b.push(4);
    b.push(5);
    b.push(6);
    b.push(7);
    ASL_TEST_EXPECT(b.size() == 7);

    b.clear();
    ASL_TEST_EXPECT(b.size() == 0);
}

static_assert(asl::buffer<DestructorObserver>::kInlineCapacity == 2);

ASL_TEST(clear_destructor_small)
{
    bool d0 = false;
    bool d1 = false;

    asl::buffer<DestructorObserver> buf;

    buf.push(&d0);
    buf.push(&d1);

    buf.clear();
    ASL_TEST_EXPECT(d0 == true);
    ASL_TEST_EXPECT(d1 == true);
}

ASL_TEST(clear_destructor_heap)
{
    bool d0 = false;
    bool d1 = false;
    bool d2 = false;

    asl::buffer<DestructorObserver> buf;

    buf.push(&d0);
    buf.push(&d1);
    buf.push(&d2);
    ASL_TEST_EXPECT(d0 == false);
    ASL_TEST_EXPECT(d1 == false);
    ASL_TEST_EXPECT(d2 == false);

    buf.clear();
    ASL_TEST_EXPECT(d0 == true);
    ASL_TEST_EXPECT(d1 == true);
    ASL_TEST_EXPECT(d2 == true);
}

ASL_TEST(move_construct_from_heap)
{
    bool d[3]{};
    asl::buffer<DestructorObserver> buf;
    buf.push(&d[0]);
    buf.push(&d[1]);
    buf.push(&d[2]);

    {
        asl::buffer<DestructorObserver> buf2(std::move(buf));
        ASL_TEST_EXPECT(buf2.size() == 3);
        ASL_TEST_EXPECT(d[0] == false);
        ASL_TEST_EXPECT(d[1] == false);
        ASL_TEST_EXPECT(d[2] == false);
    }

    ASL_TEST_EXPECT(buf.size() == 0);
    ASL_TEST_EXPECT(d[0] == true);
    ASL_TEST_EXPECT(d[1] == true);
    ASL_TEST_EXPECT(d[2] == true);
}

ASL_TEST(move_construct_inline_trivial)
{
    asl::buffer<uint64_t> buf;
    buf.push(1U);
    buf.push(2U);

    asl::buffer<uint64_t> buf2(std::move(buf));
    ASL_TEST_EXPECT(buf2[0] == 1U);
    ASL_TEST_EXPECT(buf2[1] == 2U);

    ASL_TEST_EXPECT(buf2.size() == 2);
    ASL_TEST_EXPECT(buf.size() == 0);
}

ASL_TEST(move_construct_from_inline_non_trivial)
{
    bool d[2]{};
    asl::buffer<DestructorObserver> buf;
    buf.push(&d[0]);
    buf.push(&d[1]);

    {
        asl::buffer<DestructorObserver> buf2(std::move(buf));
        ASL_TEST_EXPECT(buf2.size() == 2);
        ASL_TEST_EXPECT(d[0] == false);
        ASL_TEST_EXPECT(d[1] == false);
    }

    ASL_TEST_EXPECT(buf.size() == 0);
    ASL_TEST_EXPECT(d[0] == true);
    ASL_TEST_EXPECT(d[1] == true);
}

ASL_TEST(move_assign_from_heap)
{
    bool d[6]{};

    {
        asl::buffer<DestructorObserver> buf;
        asl::buffer<DestructorObserver> buf2;

        buf.push(&d[0]);
        buf.push(&d[1]);
        buf.push(&d[2]);

        buf2.push(&d[3]);
        buf2.push(&d[4]);
        buf2.push(&d[5]);

        ASL_TEST_EXPECT(d[0] == false);
        ASL_TEST_EXPECT(d[1] == false);
        ASL_TEST_EXPECT(d[2] == false);
        ASL_TEST_EXPECT(d[3] == false);
        ASL_TEST_EXPECT(d[4] == false);
        ASL_TEST_EXPECT(d[5] == false);

        buf2 = std::move(buf);

        ASL_TEST_EXPECT(buf.size() == 0);
        ASL_TEST_EXPECT(buf2.size() == 3);

        ASL_TEST_EXPECT(d[0] == false);
        ASL_TEST_EXPECT(d[1] == false);
        ASL_TEST_EXPECT(d[2] == false);
        ASL_TEST_EXPECT(d[3] == true);
        ASL_TEST_EXPECT(d[4] == true);
        ASL_TEST_EXPECT(d[5] == true);
    }

    ASL_TEST_EXPECT(d[0] == true);
    ASL_TEST_EXPECT(d[1] == true);
    ASL_TEST_EXPECT(d[2] == true);
    ASL_TEST_EXPECT(d[3] == true);
    ASL_TEST_EXPECT(d[4] == true);
    ASL_TEST_EXPECT(d[5] == true);
}

ASL_TEST(move_assign_trivial_heap_to_inline)
{
    isize_t alloc_count = 0;
    asl::buffer<int64_t, CounterAllocator> buf{CounterAllocator{&alloc_count}};
    asl::buffer<int64_t, CounterAllocator> buf2{CounterAllocator{&alloc_count}};

    buf.push(1);
    buf.push(2);
    ASL_TEST_EXPECT(alloc_count == 0);

    buf2.push(3);
    buf2.push(4);
    buf2.push(5);
    ASL_TEST_EXPECT(alloc_count == 1);

    buf = std::move(buf2);
    ASL_TEST_EXPECT(alloc_count == 1);

    ASL_TEST_EXPECT(buf.size() == 3);
    ASL_TEST_EXPECT(buf2.size() == 0);
    ASL_TEST_EXPECT(buf[0] == 3);
    ASL_TEST_EXPECT(buf[1] == 4);
    ASL_TEST_EXPECT(buf[2] == 5);
}

ASL_TEST(move_assign_trivial_inline_to_heap)
{
    isize_t alloc_count = 0;
    asl::buffer<int64_t, CounterAllocator> buf{CounterAllocator{&alloc_count}};
    asl::buffer<int64_t, CounterAllocator> buf2{CounterAllocator{&alloc_count}};

    buf.push(1);
    buf.push(2);
    ASL_TEST_EXPECT(alloc_count == 0);

    buf2.push(3);
    buf2.push(4);
    buf2.push(5);
    ASL_TEST_EXPECT(alloc_count == 1);

    buf2 = std::move(buf);
    ASL_TEST_EXPECT(alloc_count == 1);

    ASL_TEST_EXPECT(buf.size() == 0);
    ASL_TEST_EXPECT(buf2.size() == 2);
    ASL_TEST_EXPECT(buf2[0] == 1);
    ASL_TEST_EXPECT(buf2[1] == 2);
}

ASL_TEST(move_assign_inline_to_heap)
{
    bool d[6]{};

    {
        asl::buffer<DestructorObserver> buf;
        asl::buffer<DestructorObserver> buf2;

        buf.push(&d[0]);
        buf.push(&d[1]);

        buf2.push(&d[2]);
        buf2.push(&d[3]);
        buf2.push(&d[4]);
        buf2.push(&d[5]);

        buf2 = std::move(buf);

        ASL_TEST_EXPECT(buf.size() == 0);
        ASL_TEST_EXPECT(buf2.size() == 2);
        ASL_TEST_EXPECT(d[0] == false);
        ASL_TEST_EXPECT(d[1] == false);
        ASL_TEST_EXPECT(d[2] == false); // moved but not destroyed
        ASL_TEST_EXPECT(d[3] == false); // moved but not destroyed
        ASL_TEST_EXPECT(d[4] == true);
        ASL_TEST_EXPECT(d[5] == true);
    }

    ASL_TEST_EXPECT(d[0] == true);
    ASL_TEST_EXPECT(d[1] == true);
    ASL_TEST_EXPECT(d[2] == false); // moved but not destroyed
    ASL_TEST_EXPECT(d[3] == false); // moved but not destroyed
    ASL_TEST_EXPECT(d[4] == true);
    ASL_TEST_EXPECT(d[5] == true);
}

ASL_TEST(move_assign_from_inline_incompatible_allocator)
{
    bool d[6]{};

    {
        asl::buffer<DestructorObserver, IncompatibleAllocator> buf;
        asl::buffer<DestructorObserver, IncompatibleAllocator> buf2;

        buf.push(&d[0]);
        buf.push(&d[1]);

        buf2.push(&d[2]);
        buf2.push(&d[3]);
        buf2.push(&d[4]);
        buf2.push(&d[5]);

        buf2 = std::move(buf);

        ASL_TEST_EXPECT(buf.size() == 0);
        ASL_TEST_EXPECT(buf2.size() == 2);
        ASL_TEST_EXPECT(d[0] == false);
        ASL_TEST_EXPECT(d[1] == false);
        ASL_TEST_EXPECT(d[2] == true);
        ASL_TEST_EXPECT(d[3] == true);
        ASL_TEST_EXPECT(d[4] == true);
        ASL_TEST_EXPECT(d[5] == true);
    }

    ASL_TEST_EXPECT(d[0] == true);
    ASL_TEST_EXPECT(d[1] == true);
    ASL_TEST_EXPECT(d[2] == true);
    ASL_TEST_EXPECT(d[3] == true);
    ASL_TEST_EXPECT(d[4] == true);
    ASL_TEST_EXPECT(d[5] == true);
}

ASL_TEST(copy_construct_inline)
{
    asl::buffer<int> buf;
    buf.push(0);
    buf.push(1);
    buf.push(2);

    asl::buffer<int> buf2{buf};

    ASL_TEST_EXPECT(buf.size() == buf2.size());
    ASL_TEST_EXPECT(buf.size() == 3);
    ASL_TEST_EXPECT(buf[0] == 0);
    ASL_TEST_EXPECT(buf[1] == 1);
    ASL_TEST_EXPECT(buf[2] == 2);
    ASL_TEST_EXPECT(buf2[0] == 0);
    ASL_TEST_EXPECT(buf2[1] == 1);
    ASL_TEST_EXPECT(buf2[2] == 2);
}

ASL_TEST(copy_assign_into_smaller)
{
    asl::buffer<int> buf;
    buf.push(0);
    buf.push(1);
    buf.push(2);

    asl::buffer<int> buf2;
    buf2.push(4);

    buf2 = buf;

    ASL_TEST_EXPECT(buf.size() == 3);
    ASL_TEST_EXPECT(buf2.size() == 3);

    ASL_TEST_EXPECT(buf[0] == 0);
    ASL_TEST_EXPECT(buf[1] == 1);
    ASL_TEST_EXPECT(buf[2] == 2);
    ASL_TEST_EXPECT(buf2[0] == 0);
    ASL_TEST_EXPECT(buf2[1] == 1);
    ASL_TEST_EXPECT(buf2[2] == 2);
}

ASL_TEST(copy_assign_into_larger)
{
    asl::buffer<int> buf;
    buf.push(0);
    buf.push(1);
    buf.push(2);

    asl::buffer<int> buf2;
    buf2.push(4);

    buf = buf2;

    ASL_TEST_EXPECT(buf.size() == 1);
    ASL_TEST_EXPECT(buf2.size() == 1);

    ASL_TEST_EXPECT(buf[0] == 4);
    ASL_TEST_EXPECT(buf2[0] == 4);
}

ASL_TEST(resize_default)
{
    asl::buffer<int> buf;

    buf.push(5);
    buf.resize(4);

    ASL_TEST_ASSERT(buf.size() == 4);
    ASL_TEST_EXPECT(buf[0] == 5);
    ASL_TEST_EXPECT(buf[1] == 0);
    ASL_TEST_EXPECT(buf[2] == 0);
    ASL_TEST_EXPECT(buf[3] == 0);

    buf.resize(2);

    ASL_TEST_ASSERT(buf.size() == 2);
    ASL_TEST_EXPECT(buf[0] == 5);
    ASL_TEST_EXPECT(buf[1] == 0);
}

ASL_TEST(resize)
{
    asl::buffer<int> buf;

    buf.push(5);
    buf.resize(4, 6);

    ASL_TEST_ASSERT(buf.size() == 4);
    ASL_TEST_EXPECT(buf[0] == 5);
    ASL_TEST_EXPECT(buf[1] == 6);
    ASL_TEST_EXPECT(buf[2] == 6);
    ASL_TEST_EXPECT(buf[3] == 6);

    buf.resize(2, 7);

    ASL_TEST_ASSERT(buf.size() == 2);
    ASL_TEST_EXPECT(buf[0] == 5);
    ASL_TEST_EXPECT(buf[1] == 6);
}

ASL_TEST(resize_zero)
{
    asl::buffer<int> buf;
    for (int i = 0; i < 100; ++i)
    {
        buf.push(i);
    }

    buf.resize_zero(200);
    ASL_TEST_ASSERT(buf.size() == 200);

    for (int i = 0; i < 100; ++i)
    {
        ASL_TEST_EXPECT(buf[i] == i);
        ASL_TEST_EXPECT(buf[100 + i] == 0);
    }
}

