#include "asl/testing/testing.hpp"
#include "asl/hash.hpp"
#include "asl/string_view.hpp"
#include "asl/string.hpp"

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

// @Todo span, buffer (add combine_contiguous (optimize uniquely_represented))
// @Todo enum classes
// @Todo option (optimize uniquely_represented + has_niche)
// @Todo status, status_or
// @Todo box
