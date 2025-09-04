// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/hashing/hash.hpp"
#include "asl/strings/string_view.hpp"
#include "asl/strings/string.hpp"
#include "asl/containers/buffer.hpp"
#include "asl/types/box.hpp"
#include "asl/types/option.hpp"
#include "asl/types/status.hpp"
#include "asl/types/status_or.hpp"

static_assert(!asl::hashable<int*>);
static_assert(!asl::hashable<int[]>);
static_assert(!asl::hashable<int[9]>);

static_assert(asl::hashable<uint8_t>);
static_assert(asl::hashable<uint16_t>);
static_assert(asl::hashable<uint32_t>);
static_assert(asl::hashable<uint64_t>);
static_assert(asl::hashable<uint128_t>);

static_assert(asl::hashable<int8_t>);
static_assert(asl::hashable<int16_t>);
static_assert(asl::hashable<int32_t>);
static_assert(asl::hashable<int64_t>);

ASL_TEST(integers)
{
    const uint64_t a = asl::hash_value<uint16_t>(45);
    const uint64_t b = asl::hash_value<uint16_t>(45);
    const uint64_t c = asl::hash_value<uint16_t>(46);
    const uint64_t d = asl::hash_value<uint32_t>(45);

    ASL_TEST_EXPECT(a == b);
    ASL_TEST_EXPECT(a != c);
    ASL_TEST_EXPECT(a != d);
}

static_assert(asl::hashable<bool>);

ASL_TEST(bool)
{
    ASL_TEST_EXPECT(asl::hash_value(true) == asl::hash_value(true));
    ASL_TEST_EXPECT(asl::hash_value(false) == asl::hash_value(false));
    ASL_TEST_EXPECT(asl::hash_value(true) != asl::hash_value(false));
}

static_assert(asl::hashable<asl::string_view>);
static_assert(asl::hashable<asl::string<>>);

ASL_TEST(strings)
{
    ASL_TEST_EXPECT(asl::hash_value("hello"_sv) == asl::hash_value("hello"_sv));
    ASL_TEST_EXPECT(asl::hash_value("hello"_sv) != asl::hash_value("hello "_sv));
    ASL_TEST_EXPECT(asl::hash_value("hello"_sv) != asl::hash_value("HELLO"_sv));

    ASL_TEST_EXPECT(asl::hash_value(asl::string("hello"_sv)) == asl::hash_value(asl::string("hello"_sv)));
    ASL_TEST_EXPECT(asl::hash_value(asl::string("hello"_sv)) != asl::hash_value(asl::string("hello "_sv)));
    ASL_TEST_EXPECT(asl::hash_value(asl::string("hello"_sv)) != asl::hash_value(asl::string("HELLO"_sv)));

    ASL_TEST_EXPECT(asl::hash_value("hello"_sv) == asl::hash_value(asl::string("hello"_sv)));
}

static_assert(asl::hashable<asl::span<const int>>);
static_assert(!asl::hashable<asl::span<const int*>>);
static_assert(asl::hashable<asl::span<asl::string_view>>);

ASL_TEST(span)
{
    int ints1[] = {1, 2, 3};
    int ints2[] = {1, 2, 3};
    int ints3[] = {1, 2};
    int ints4[] = {3, 2, 1};

    ASL_TEST_EXPECT(asl::hash_value(asl::span<int>(ints1)) == asl::hash_value(asl::span<int>(ints2)));
    ASL_TEST_EXPECT(asl::hash_value(asl::span<int>(ints1)) != asl::hash_value(asl::span<int>(ints3)));
    ASL_TEST_EXPECT(asl::hash_value(asl::span<int>(ints1)) != asl::hash_value(asl::span<int>(ints4)));

    asl::string_view strs1[] = {"a", "abc", "hello"};
    asl::string_view strs2[] = {"a", "abc", "hello"};
    asl::string_view strs3[] = {"a", "abc"};
    asl::string_view strs4[] = {"a", "abc", "hello", "what"};

    ASL_TEST_EXPECT(asl::hash_value(asl::span<asl::string_view>(strs1)) == asl::hash_value(asl::span<asl::string_view>(strs2)));
    ASL_TEST_EXPECT(asl::hash_value(asl::span<asl::string_view>(strs1)) != asl::hash_value(asl::span<asl::string_view>(strs3)));
    ASL_TEST_EXPECT(asl::hash_value(asl::span<asl::string_view>(strs1)) != asl::hash_value(asl::span<asl::string_view>(strs4)));
}

static_assert(asl::hashable<asl::buffer<int>>);
static_assert(!asl::hashable<asl::buffer<int*>>);

ASL_TEST(buffer)
{
    asl::buffer<int> ints1;
    ints1.push(1);
    ints1.push(2);
    ints1.push(3);

    asl::buffer<int> ints2;
    ints2.push(1);
    ints2.push(2);
    ints2.push(3);

    asl::buffer<int> ints3;
    ints3.push(1);
    ints3.push(2);

    asl::buffer<int> ints4;
    ints4.push(1);
    ints4.push(2);
    ints4.push(4);

    ASL_TEST_EXPECT(asl::hash_value(ints1) == asl::hash_value(ints2));
    ASL_TEST_EXPECT(asl::hash_value(ints1) != asl::hash_value(ints3));
    ASL_TEST_EXPECT(asl::hash_value(ints1) != asl::hash_value(ints4));
    ASL_TEST_EXPECT(asl::hash_value(ints1) == asl::hash_value(ints1.as_span()));

    asl::buffer<asl::string_view> strs1;
    strs1.push("Hello");
    strs1.push("World");

    asl::buffer<asl::string_view> strs2;
    strs2.push("Hello");
    strs2.push("World");

    asl::buffer<asl::string_view> strs3;
    strs3.push("Hello");
    strs3.push("world");

    asl::buffer<asl::string_view> strs4;
    strs4.push("Hello");
    strs4.push("World");
    strs4.push("World");

    ASL_TEST_EXPECT(asl::hash_value(strs1) == asl::hash_value(strs2));
    ASL_TEST_EXPECT(asl::hash_value(strs1) != asl::hash_value(strs3));
    ASL_TEST_EXPECT(asl::hash_value(strs1) != asl::hash_value(strs4));
    ASL_TEST_EXPECT(asl::hash_value(strs1) == asl::hash_value(strs1.as_span()));
}

enum Enum1 {};
enum class Enum2 {};

static_assert(asl::hashable<Enum1>);
static_assert(asl::hashable<Enum2>);

static_assert(!asl::hashable<asl::box<int*>>);
static_assert(asl::hashable<asl::box<asl::string_view>>);

ASL_TEST(box)
{
    auto b1 = asl::make_box<asl::string_view>("Hello, world!");
    auto b2 = asl::make_box<asl::string_view>("Hello, world!");
    auto b3 = asl::make_box<asl::string_view>("Hello, world! 2");

    ASL_TEST_EXPECT(asl::hash_value(b1) == asl::hash_value(b2));
    ASL_TEST_EXPECT(asl::hash_value(b1) != asl::hash_value(b3));
    ASL_TEST_EXPECT(asl::hash_value(b1) == asl::hash_value("Hello, world!"_sv));
}

struct NonZero
{
    int value;

    constexpr explicit NonZero(int x) : value(x)
    {
        ASL_ASSERT(x != 0);
    }

    constexpr explicit NonZero(asl::niche_t) : value(0) {}

    constexpr bool operator==(asl::niche_t) const { return value == 0; }
};

template<> struct asl::has_unique_object_representations<NonZero> : true_type {};
static_assert(asl::has_niche<NonZero>);
static_assert(asl::has_unique_object_representations_v<NonZero>);

static_assert(asl::hashable<asl::option<int>>);
static_assert(!asl::hashable<asl::option<int*>>);
static_assert(asl::hashable<asl::option<asl::string_view>>);
static_assert(asl::hashable<asl::option<NonZero>>);
static_assert(asl::has_unique_object_representations_v<asl::option<NonZero>>);

ASL_TEST(option)
{
    const asl::option<int> int1 = 0;
    const asl::option<int> int2 = 0;
    const asl::option<int> int3 = 1;
    const asl::option<int> int4 = asl::nullopt;

    ASL_TEST_EXPECT(asl::hash_value(int1) == asl::hash_value(int2));
    ASL_TEST_EXPECT(asl::hash_value(int1) != asl::hash_value(int3));
    ASL_TEST_EXPECT(asl::hash_value(int1) != asl::hash_value(int4));

    const asl::option<NonZero> noz1{8};
    const asl::option<NonZero> noz2{8};
    const asl::option<NonZero> noz3{9};
    const asl::option<NonZero> noz4 = asl::nullopt;

    ASL_TEST_EXPECT(asl::hash_value(noz1) == asl::hash_value(noz2));
    ASL_TEST_EXPECT(asl::hash_value(noz1) != asl::hash_value(noz3));
    ASL_TEST_EXPECT(asl::hash_value(noz1) != asl::hash_value(noz4));
}

static_assert(asl::hashable<asl::status>);

// NOLINTNEXTLINE(*-cognitive-complexity)
ASL_TEST(status)
{
    const asl::status s1 = asl::ok();
    const asl::status s2 = asl::ok();
    const asl::status s3 = asl::internal_error();
    const asl::status s4 = asl::internal_error();
    const asl::status s5 = asl::runtime_error();
    const asl::status s6 = asl::internal_error("Oh, no!");
    const asl::status s7 = asl::internal_error("Oh, no!");
    const asl::status s8 = asl::internal_error("Oh, no");
    const asl::status s9 = asl::runtime_error("Oh, no!");

    ASL_TEST_EXPECT(asl::hash_value(s1) == asl::hash_value(s2));
    ASL_TEST_EXPECT(asl::hash_value(s3) == asl::hash_value(s4));
    ASL_TEST_EXPECT(asl::hash_value(s6) == asl::hash_value(s7));

    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s3));
    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s5));
    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s6));
    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s9));

    ASL_TEST_EXPECT(asl::hash_value(s3) != asl::hash_value(s5));
    ASL_TEST_EXPECT(asl::hash_value(s3) != asl::hash_value(s6));
    ASL_TEST_EXPECT(asl::hash_value(s3) != asl::hash_value(s8));
    ASL_TEST_EXPECT(asl::hash_value(s3) != asl::hash_value(s9));

    ASL_TEST_EXPECT(asl::hash_value(s6) != asl::hash_value(s8));
    ASL_TEST_EXPECT(asl::hash_value(s6) != asl::hash_value(s9));
}

static_assert(asl::hashable<asl::status_or<int>>);
static_assert(asl::hashable<asl::status_or<asl::string_view>>);
static_assert(!asl::hashable<asl::status_or<int*>>);

ASL_TEST(status_or)
{
    const asl::status_or<int> s1 = 42;
    const asl::status_or<int> s2 = 42;
    const asl::status_or<int> s3 = 43;
    const asl::status_or<int> s4 = asl::runtime_error();
    const asl::status_or<int> s5 = asl::runtime_error();
    const asl::status_or<int> s6 = asl::runtime_error("Hello");
    const asl::status_or<int> s7 = asl::runtime_error("Hello");

    ASL_TEST_EXPECT(asl::hash_value(s1) == asl::hash_value(s2));
    ASL_TEST_EXPECT(asl::hash_value(s4) == asl::hash_value(s5));
    ASL_TEST_EXPECT(asl::hash_value(s6) == asl::hash_value(s7));

    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s3));
    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s4));
    ASL_TEST_EXPECT(asl::hash_value(s1) != asl::hash_value(s6));

    ASL_TEST_EXPECT(asl::hash_value(s4) != asl::hash_value(s6));
}
