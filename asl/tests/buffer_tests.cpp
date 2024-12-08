#include <asl/buffer.hpp>

#include <asl/testing/testing.hpp>

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
