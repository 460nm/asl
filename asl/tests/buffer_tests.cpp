#include "asl/buffer.hpp"

#include "asl/testing/testing.hpp"

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
};
static_assert(asl::allocator<CounterAllocator>);

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

// NOLINTBEGIN(*-pointer-arithmetic)
ASL_TEST(push)
{
    asl::buffer<int32_t> b;
    ASL_TEST_EXPECT(b.size() == 0);

    b.push(1);
    ASL_TEST_EXPECT(b.size() == 1);
    ASL_TEST_EXPECT(b.data()[0] == 1);

    b.push(2);
    b.push(3);
    ASL_TEST_EXPECT(b.size() == 3);
    ASL_TEST_EXPECT(b.data()[0] == 1);
    ASL_TEST_EXPECT(b.data()[1] == 2);
    ASL_TEST_EXPECT(b.data()[2] == 3);

    b.push(4);
    b.push(5);
    b.push(6);
    b.push(7);
    ASL_TEST_EXPECT(b.size() == 7);
    ASL_TEST_EXPECT(b.data()[0] == 1);
    ASL_TEST_EXPECT(b.data()[1] == 2);
    ASL_TEST_EXPECT(b.data()[2] == 3);
    ASL_TEST_EXPECT(b.data()[3] == 4);
    ASL_TEST_EXPECT(b.data()[4] == 5);
    ASL_TEST_EXPECT(b.data()[5] == 6);
    ASL_TEST_EXPECT(b.data()[6] == 7);
}
// NOLINTEND(*-pointer-arithmetic)

// @Todo Test push with non trivial move (non copy) types
