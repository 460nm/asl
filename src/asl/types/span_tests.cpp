// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/types/span.hpp"
#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"

static_assert(asl::trivially_destructible<asl::span<int>>);
static_assert(asl::trivially_destructible<asl::span<WithDestructor>>);

static_assert(asl::trivially_copy_constructible<asl::span<int>>);
static_assert(asl::trivially_copy_constructible<asl::span<Pinned>>);

static_assert(asl::size_of<asl::span<int>> == asl::size_of<void*> * 2);
static_assert(asl::size_of<asl::span<int, 2>> == asl::size_of<void*>);

ASL_TEST(empty_dynamic)
{
    const asl::span<int> s;
    ASL_TEST_EXPECT(s.size() == 0);
    ASL_TEST_EXPECT(s.size_bytes() == 0);
    ASL_TEST_EXPECT(s.is_empty());
}


ASL_TEST(empty_static)
{
    const asl::span<int, 0> s;
    ASL_TEST_EXPECT(s.size() == 0);
    ASL_TEST_EXPECT(s.size_bytes() == 0);
    ASL_TEST_EXPECT(s.is_empty());
}

ASL_TEST(from_array_dynamic)
{
    int array[] = {1, 2, 3};
    const asl::span<int> span = array;
    ASL_TEST_ASSERT(span.size() == 3);
    ASL_TEST_EXPECT(span[0] == 1);
    ASL_TEST_EXPECT(span[1] == 2);
    ASL_TEST_EXPECT(span[2] == 3);
}

static_assert(asl::default_constructible<asl::span<int>>);
static_assert(asl::default_constructible<asl::span<int, 0>>);
static_assert(!asl::default_constructible<asl::span<int, 8>>);

static_assert(asl::constructible_from<asl::span<int32_t>, int32_t(&)[8]>);
static_assert(!asl::constructible_from<asl::span<int32_t>, const int32_t(&)[8]>);
static_assert(asl::constructible_from<asl::span<const int32_t>, int32_t(&)[8]>);
static_assert(asl::constructible_from<asl::span<const int32_t>, const int32_t(&)[8]>);
static_assert(asl::constructible_from<asl::span<int32_t, 8>, int32_t(&)[8]>);
static_assert(!asl::constructible_from<asl::span<int32_t, 8>, int32_t(&)[10]>);

ASL_TEST(from_array_static)
{
    int array[] = {1, 2, 3};
    const asl::span<int, 3> span = array;
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

    const asl::span<int> span1 = array;

    const asl::span<int, 3> span2{span1};
    ASL_TEST_ASSERT(span2.size() == 3);
    ASL_TEST_EXPECT(span2[0] == 1);
    ASL_TEST_EXPECT(span2[1] == 2);
    ASL_TEST_EXPECT(span2[2] == 3);

    const asl::span<int> span3 = span2;
    ASL_TEST_ASSERT(span3.size() == 3);
    ASL_TEST_EXPECT(span3[0] == 1);
    ASL_TEST_EXPECT(span3[1] == 2);
    ASL_TEST_EXPECT(span3[2] == 3);

    const asl::span<const int, 3> span4{span2};
    ASL_TEST_ASSERT(span4.size() == 3);
    ASL_TEST_EXPECT(span4[0] == 1);
    ASL_TEST_EXPECT(span4[1] == 2);
    ASL_TEST_EXPECT(span4[2] == 3);
}

template<typename Span, isize_t kOffset, isize_t kSize = asl::dynamic_size>
concept IsValidSubspan = requires (Span s) { s.template subspan<kOffset, kSize>(); };

static_assert(asl::same_as<asl::span<int, 4>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<0>())>);

static_assert(asl::same_as<asl::span<int, 3>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<1>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<2>())>);

static_assert(asl::same_as<asl::span<int, 1>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<3>())>);

static_assert(asl::same_as<asl::span<int, 0>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<4>())>);

static_assert(!IsValidSubspan<asl::span<int, 4>, 5>);

static_assert(asl::same_as<asl::span<int>,
     decltype(asl::declval<asl::span<int>>().subspan<0>())>);

static_assert(asl::same_as<asl::span<int, 4>,
     decltype(asl::declval<asl::span<int>>().subspan<0, 4>())>);

static_assert(asl::same_as<asl::span<int, 4>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<0, 4>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<1, 2>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int, 4>>().subspan<2, 2>())>);

static_assert(!IsValidSubspan<asl::span<int, 4>, 2, 3>);

ASL_TEST(subspan_static_from_static)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int, 4> span{array};

    auto s1 = span.subspan<0>();
    ASL_TEST_ASSERT(s1.size() == 4);
    ASL_TEST_EXPECT(s1[0] == 1);
    ASL_TEST_EXPECT(s1[1] == 2);
    ASL_TEST_EXPECT(s1[2] == 3);
    ASL_TEST_EXPECT(s1[3] == 4);

    auto s2 = span.subspan<2>();
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 3);
    ASL_TEST_EXPECT(s2[1] == 4);

    auto s3 = span.subspan<4>();
    ASL_TEST_ASSERT(s3.size() == 0);

    auto s4 = span.subspan<1, 2>();
    ASL_TEST_ASSERT(s4.size() == 2);
    ASL_TEST_EXPECT(s4[0] == 2);
    ASL_TEST_EXPECT(s4[1] == 3);
}

ASL_TEST(subspan_static_from_dynamic)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int> span{array};

    auto s1 = span.subspan<0>();
    ASL_TEST_ASSERT(s1.size() == 4);
    ASL_TEST_EXPECT(s1[0] == 1);
    ASL_TEST_EXPECT(s1[1] == 2);
    ASL_TEST_EXPECT(s1[2] == 3);
    ASL_TEST_EXPECT(s1[3] == 4);

    auto s2 = span.subspan<2>();
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 3);
    ASL_TEST_EXPECT(s2[1] == 4);

    auto s3 = span.subspan<4>();
    ASL_TEST_ASSERT(s3.size() == 0);

    auto s4 = span.subspan<1, 2>();
    ASL_TEST_ASSERT(s4.size() == 2);
    ASL_TEST_EXPECT(s4[0] == 2);
    ASL_TEST_EXPECT(s4[1] == 3);
}

ASL_TEST(subspan_dynamic)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int> span{array};

    auto s1 = span.subspan(0);
    ASL_TEST_ASSERT(s1.size() == 4);
    ASL_TEST_EXPECT(s1[0] == 1);
    ASL_TEST_EXPECT(s1[1] == 2);
    ASL_TEST_EXPECT(s1[2] == 3);
    ASL_TEST_EXPECT(s1[3] == 4);

    auto s2 = span.subspan(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 3);
    ASL_TEST_EXPECT(s2[1] == 4);

    auto s3 = span.subspan(4);
    ASL_TEST_ASSERT(s3.size() == 0);

    auto s4 = span.subspan(1, 2);
    ASL_TEST_ASSERT(s4.size() == 2);
    ASL_TEST_EXPECT(s4[0] == 2);
    ASL_TEST_EXPECT(s4[1] == 3);
}

template<typename Span, isize_t kSize>
concept IsValidFirst = requires (Span s) { s.template first<kSize>(); };

static_assert(asl::same_as<asl::span<int, 0>,
     decltype(asl::declval<asl::span<int>>().first<0>())>);

static_assert(asl::same_as<asl::span<int, 1>,
     decltype(asl::declval<asl::span<int>>().first<1>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int>>().first<2>())>);

static_assert(asl::same_as<asl::span<int, 3>,
     decltype(asl::declval<asl::span<int>>().first<3>())>);

static_assert(asl::same_as<asl::span<int, 0>,
     decltype(asl::declval<asl::span<int, 4>>().first<0>())>);

static_assert(asl::same_as<asl::span<int, 1>,
     decltype(asl::declval<asl::span<int, 4>>().first<1>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int, 4>>().first<2>())>);

static_assert(asl::same_as<asl::span<int, 3>,
     decltype(asl::declval<asl::span<int, 4>>().first<3>())>);

static_assert(asl::same_as<asl::span<int, 4>,
     decltype(asl::declval<asl::span<int, 4>>().first<4>())>);

static_assert(!IsValidFirst<asl::span<int, 4>, 5>);
static_assert(!IsValidFirst<asl::span<int, 4>, asl::dynamic_size>);

ASL_TEST(first_static_from_static)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int, 4> span{array};

    auto s1 = span.first<0>();
    ASL_TEST_ASSERT(s1.size() == 0);

    auto s2 = span.first<2>();
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 1);
    ASL_TEST_EXPECT(s2[1] == 2);

    auto s3 = span.first<4>();
    ASL_TEST_ASSERT(s3.size() == 4);
    ASL_TEST_EXPECT(s3[0] == 1);
    ASL_TEST_EXPECT(s3[1] == 2);
    ASL_TEST_EXPECT(s3[2] == 3);
    ASL_TEST_EXPECT(s3[3] == 4);
}

ASL_TEST(first_static_from_dynamic)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int> span{array};

    auto s1 = span.first<0>();
    ASL_TEST_ASSERT(s1.size() == 0);

    auto s2 = span.first<2>();
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 1);
    ASL_TEST_EXPECT(s2[1] == 2);

    auto s3 = span.first<4>();
    ASL_TEST_ASSERT(s3.size() == 4);
    ASL_TEST_EXPECT(s3[0] == 1);
    ASL_TEST_EXPECT(s3[1] == 2);
    ASL_TEST_EXPECT(s3[2] == 3);
    ASL_TEST_EXPECT(s3[3] == 4);
}

ASL_TEST(first_dynamic)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int> span{array};

    auto s1 = span.first(0);
    ASL_TEST_ASSERT(s1.size() == 0);

    auto s2 = span.first(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 1);
    ASL_TEST_EXPECT(s2[1] == 2);

    auto s3 = span.first(4);
    ASL_TEST_ASSERT(s3.size() == 4);
    ASL_TEST_EXPECT(s3[0] == 1);
    ASL_TEST_EXPECT(s3[1] == 2);
    ASL_TEST_EXPECT(s3[2] == 3);
    ASL_TEST_EXPECT(s3[3] == 4);
}

template<typename Span, isize_t kSize>
concept IsValidLast = requires (Span s) { s.template last<kSize>(); };

static_assert(asl::same_as<asl::span<int, 0>,
     decltype(asl::declval<asl::span<int>>().last<0>())>);

static_assert(asl::same_as<asl::span<int, 1>,
     decltype(asl::declval<asl::span<int>>().last<1>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int>>().last<2>())>);

static_assert(asl::same_as<asl::span<int, 3>,
     decltype(asl::declval<asl::span<int>>().last<3>())>);

static_assert(asl::same_as<asl::span<int, 0>,
     decltype(asl::declval<asl::span<int, 4>>().last<0>())>);

static_assert(asl::same_as<asl::span<int, 1>,
     decltype(asl::declval<asl::span<int, 4>>().last<1>())>);

static_assert(asl::same_as<asl::span<int, 2>,
     decltype(asl::declval<asl::span<int, 4>>().last<2>())>);

static_assert(asl::same_as<asl::span<int, 3>,
     decltype(asl::declval<asl::span<int, 4>>().last<3>())>);

static_assert(asl::same_as<asl::span<int, 4>,
     decltype(asl::declval<asl::span<int, 4>>().last<4>())>);

static_assert(!IsValidLast<asl::span<int, 4>, 5>);
static_assert(!IsValidLast<asl::span<int, 4>, asl::dynamic_size>);

ASL_TEST(last_static_from_static)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int, 4> span{array};

    auto s1 = span.last<0>();
    ASL_TEST_ASSERT(s1.size() == 0);

    auto s2 = span.last<2>();
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 3);
    ASL_TEST_EXPECT(s2[1] == 4);

    auto s3 = span.last<4>();
    ASL_TEST_ASSERT(s3.size() == 4);
    ASL_TEST_EXPECT(s3[0] == 1);
    ASL_TEST_EXPECT(s3[1] == 2);
    ASL_TEST_EXPECT(s3[2] == 3);
    ASL_TEST_EXPECT(s3[3] == 4);
}

ASL_TEST(last_static_from_dynamic)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int> span{array};

    auto s1 = span.last<0>();
    ASL_TEST_ASSERT(s1.size() == 0);

    auto s2 = span.last<2>();
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 3);
    ASL_TEST_EXPECT(s2[1] == 4);

    auto s3 = span.last<4>();
    ASL_TEST_ASSERT(s3.size() == 4);
    ASL_TEST_EXPECT(s3[0] == 1);
    ASL_TEST_EXPECT(s3[1] == 2);
    ASL_TEST_EXPECT(s3[2] == 3);
    ASL_TEST_EXPECT(s3[3] == 4);
}

ASL_TEST(last_dynamic)
{
    int array[] = {1, 2, 3, 4};
    const asl::span<int> span{array};

    auto s1 = span.last(0);
    ASL_TEST_ASSERT(s1.size() == 0);

    auto s2 = span.last(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(s2[0] == 3);
    ASL_TEST_EXPECT(s2[1] == 4);

    auto s3 = span.last(4);
    ASL_TEST_ASSERT(s3.size() == 4);
    ASL_TEST_EXPECT(s3[0] == 1);
    ASL_TEST_EXPECT(s3[1] == 2);
    ASL_TEST_EXPECT(s3[2] == 3);
    ASL_TEST_EXPECT(s3[3] == 4);
}

template<typename T>
concept HasAsMutableBytes = requires(asl::span<T> s) { asl::as_mutable_bytes(s); };

static_assert(HasAsMutableBytes<int>);
static_assert(!HasAsMutableBytes<const int>);
static_assert(!HasAsMutableBytes<int* const>);
static_assert(HasAsMutableBytes<const int*>);

ASL_TEST(as_bytes)
{
     uint32_t data[] = {0x01020304, 0x05060708};
     const asl::span s1(data);
     const asl::span s2 = asl::as_bytes(s1);

     ASL_TEST_ASSERT(s2.size() == 8);
     ASL_TEST_ASSERT(static_cast<int>(s2[0]) == 0x04);
     ASL_TEST_ASSERT(static_cast<int>(s2[1]) == 0x03);
     ASL_TEST_ASSERT(static_cast<int>(s2[2]) == 0x02);
     ASL_TEST_ASSERT(static_cast<int>(s2[3]) == 0x01);
     ASL_TEST_ASSERT(static_cast<int>(s2[4]) == 0x08);
     ASL_TEST_ASSERT(static_cast<int>(s2[5]) == 0x07);
     ASL_TEST_ASSERT(static_cast<int>(s2[6]) == 0x06);
     ASL_TEST_ASSERT(static_cast<int>(s2[7]) == 0x05);
}

ASL_TEST(as_mutable_bytes)
{
     uint32_t data[] = {0x01020304, 0x05060708};
     const asl::span s1(data);
     const asl::span s2 = asl::as_mutable_bytes(s1);

     ASL_TEST_ASSERT(s2.size() == 8);
     ASL_TEST_ASSERT(static_cast<int>(s2[0]) == 0x04);
     ASL_TEST_ASSERT(static_cast<int>(s2[1]) == 0x03);
     ASL_TEST_ASSERT(static_cast<int>(s2[2]) == 0x02);
     ASL_TEST_ASSERT(static_cast<int>(s2[3]) == 0x01);
     ASL_TEST_ASSERT(static_cast<int>(s2[4]) == 0x08);
     ASL_TEST_ASSERT(static_cast<int>(s2[5]) == 0x07);
     ASL_TEST_ASSERT(static_cast<int>(s2[6]) == 0x06);
     ASL_TEST_ASSERT(static_cast<int>(s2[7]) == 0x05);
}
