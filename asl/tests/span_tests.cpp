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

ASL_TEST(from_array_dynamic)
{
    int array[] = {1, 2, 3};
    asl::span<int> span = array;
    ASL_TEST_ASSERT(span.size() == 3);
    ASL_TEST_EXPECT(span[0] == 1);
    ASL_TEST_EXPECT(span[1] == 2);
    ASL_TEST_EXPECT(span[2] == 3);
}

static_assert(asl::constructible_from<asl::span<int32_t>, int32_t(&)[8]>);
static_assert(!asl::constructible_from<asl::span<int32_t>, const int32_t(&)[8]>);
static_assert(asl::constructible_from<asl::span<const int32_t>, int32_t(&)[8]>);
static_assert(asl::constructible_from<asl::span<const int32_t>, const int32_t(&)[8]>);
static_assert(asl::constructible_from<asl::span<int32_t, 8>, int32_t(&)[8]>);
static_assert(!asl::constructible_from<asl::span<int32_t, 8>, int32_t(&)[10]>);

ASL_TEST(from_array_static)
{
    int array[] = {1, 2, 3};
    asl::span<int, 3> span = array;
    ASL_TEST_ASSERT(span.size() == 3);
    ASL_TEST_EXPECT(span[0] == 1);
    ASL_TEST_EXPECT(span[1] == 2);
    ASL_TEST_EXPECT(span[2] == 3);
}

static_assert(asl::constructible_from<asl::span<const int32_t>, asl::span<int32_t>>);
static_assert(!asl::constructible_from<asl::span<int32_t>, asl::span<const int32_t>>);
static_assert(!asl::constructible_from<asl::span<int32_t>, asl::span<int16_t>>);
static_assert(!asl::constructible_from<asl::span<int32_t>, asl::span<uint32_t>>);
static_assert(asl::constructible_from<asl::span<int32_t>, asl::span<int32_t, 6>>);
static_assert(asl::constructible_from<asl::span<int32_t, 6>, asl::span<int32_t>>);
static_assert(asl::constructible_from<asl::span<int32_t>, asl::span<int32_t, 6>>);
static_assert(asl::constructible_from<asl::span<int32_t, 6>, asl::span<int32_t, 6>>);
static_assert(!asl::constructible_from<asl::span<int32_t, 6>, asl::span<int32_t, 7>>);

ASL_TEST(conversion)
{
    int array[] = {1, 2, 3};

    asl::span<int> span1 = array;

    asl::span<int, 3> span2{span1};
    ASL_TEST_ASSERT(span2.size() == 3);
    ASL_TEST_EXPECT(span2[0] == 1);
    ASL_TEST_EXPECT(span2[1] == 2);
    ASL_TEST_EXPECT(span2[2] == 3);

    asl::span<int> span3 = span2;
    ASL_TEST_ASSERT(span3.size() == 3);
    ASL_TEST_EXPECT(span3[0] == 1);
    ASL_TEST_EXPECT(span3[1] == 2);
    ASL_TEST_EXPECT(span3[2] == 3);

    asl::span<const int, 3> span4{span2};
    ASL_TEST_ASSERT(span4.size() == 3);
    ASL_TEST_EXPECT(span4[0] == 1);
    ASL_TEST_EXPECT(span4[1] == 2);
    ASL_TEST_EXPECT(span4[2] == 3);
}
