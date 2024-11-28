#include "asl/string_view.hpp"
#include "asl/testing/testing.hpp"

static_assert(asl::trivially_destructible<asl::string_view>);
static_assert(asl::trivially_copyable<asl::string_view>);

ASL_TEST(default)
{
    asl::string_view s1;
    ASL_TEST_EXPECT(s1.is_empty());

    asl::string_view s2 = nullptr;
    ASL_TEST_EXPECT(s2.is_empty());
}

ASL_TEST(from_literal)
{
    asl::string_view s1 = "Hello"_sv;
    ASL_TEST_ASSERT(s1.size() == 5);
    ASL_TEST_EXPECT(asl::memcmp(s1.data(), "Hello", 5) == 0);

    asl::string_view s2 = ""_sv;
    ASL_TEST_EXPECT(s2.is_empty());
}

ASL_TEST(substr1)
{
    asl::string_view s1 = "abcd";
    
    asl::string_view s2 = s1.substr(0);
    ASL_TEST_ASSERT(s2.size() == 4);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "abcd", 4) == 0);

    s2 = s1.substr(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "cd", 2) == 0);

    s2 = s1.substr(4);
    ASL_TEST_ASSERT(s2.size() == 0);
}

ASL_TEST(substr2)
{
    asl::string_view s1 = "abcd";
    
    asl::string_view s2 = s1.substr(0, 4);
    ASL_TEST_ASSERT(s2.size() == 4);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "abcd", 4) == 0);

    s2 = s1.substr(1, 2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "bc", 2) == 0);

    s2 = s1.substr(4, 0);
    ASL_TEST_ASSERT(s2.size() == 0);

    s2 = s1.substr(1, 0);
    ASL_TEST_ASSERT(s2.size() == 0);
}

ASL_TEST(first)
{
    asl::string_view s1 = "abcd";
    
    asl::string_view s2 = s1.first(0);
    ASL_TEST_ASSERT(s2.size() == 0);

    s2 = s1.first(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "ab", 2) == 0);

    s2 = s1.first(4);
    ASL_TEST_ASSERT(s2.size() == 4);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "abcd", 4) == 0);
}

ASL_TEST(last)
{
    asl::string_view s1 = "abcd";
    
    asl::string_view s2 = s1.last(0);
    ASL_TEST_ASSERT(s2.size() == 0);

    s2 = s1.last(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "cd", 2) == 0);

    s2 = s1.last(4);
    ASL_TEST_ASSERT(s2.size() == 4);
    ASL_TEST_EXPECT(asl::memcmp(s2.data(), "abcd", 4) == 0);
}

ASL_TEST(equal)
{
    ASL_TEST_EXPECT("abc"_sv == "abc"_sv);
    ASL_TEST_EXPECT(""_sv == ""_sv);
    ASL_TEST_EXPECT("abc"_sv != "ab"_sv);
    ASL_TEST_EXPECT("abc"_sv != "abd"_sv);
}
