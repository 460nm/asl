#include "asl/testing/testing.hpp"
#include "asl/hash.hpp"
#include "asl/string_view.hpp"
#include "asl/string.hpp"
#include "asl/buffer.hpp"

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
    uint64_t a = asl::hash_value<uint16_t>(45);
    uint64_t b = asl::hash_value<uint16_t>(45);
    uint64_t c = asl::hash_value<uint16_t>(46);
    uint64_t d = asl::hash_value<uint32_t>(45);

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

// @Todo option (optimize uniquely_represented + has_niche)
// @Todo status, status_or
// @Todo box
