#include "asl/span.hpp"
#include "asl/testing/testing.hpp"
#include "asl/tests/test_types.hpp"

static_assert(asl::trivially_destructible<asl::span<int>>);
static_assert(asl::trivially_destructible<asl::span<HasDestructor>>);

static_assert(asl::trivially_copyable<asl::span<int>>);
static_assert(asl::trivially_copyable<asl::span<NonCopyConstructible>>);

static_assert(asl::size_of<asl::span<int>> == asl::size_of<void*> * 2);
static_assert(asl::size_of<asl::span<int, 2>> == asl::size_of<void*>);

ASL_TEST(empty_dynamic)
{
    asl::span<int> s;
    ASL_TEST_EXPECT(s.size() == 0);
    ASL_TEST_EXPECT(s.size_bytes() == 0);
    ASL_TEST_EXPECT(s.is_empty());
}

ASL_TEST(empty_static)
{
    asl::span<int, 0> s;
    ASL_TEST_EXPECT(s.size() == 0);
    ASL_TEST_EXPECT(s.size_bytes() == 0);
    ASL_TEST_EXPECT(s.is_empty());
}
