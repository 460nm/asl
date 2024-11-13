#include "asl/string_view.hpp"
#include "asl/testing/testing.hpp"

// @Todo Don't use stdlib, remake memcmp
#include <string.h>

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
    ASL_TEST_EXPECT(memcmp(s1.data(), "Hello", 5) == 0);

    asl::string_view s2 = ""_sv;
    ASL_TEST_EXPECT(s2.is_empty());
}

ASL_TEST(substr1)
{
    asl::string_view s1 = "abcd";
    
    asl::string_view s2 = s1.substr(0);
    ASL_TEST_ASSERT(s2.size() == 4);
    ASL_TEST_EXPECT(memcmp(s2.data(), "abcd", 4) == 0);

    s2 = s1.substr(2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(memcmp(s2.data(), "cd", 2) == 0);

    s2 = s1.substr(4);
    ASL_TEST_ASSERT(s2.size() == 0);
}

ASL_TEST(substr2)
{
    asl::string_view s1 = "abcd";
    
    asl::string_view s2 = s1.substr(0, 4);
    ASL_TEST_ASSERT(s2.size() == 4);
    ASL_TEST_EXPECT(memcmp(s2.data(), "abcd", 4) == 0);

    s2 = s1.substr(1, 2);
    ASL_TEST_ASSERT(s2.size() == 2);
    ASL_TEST_EXPECT(memcmp(s2.data(), "bc", 2) == 0);

    s2 = s1.substr(4, 0);
    ASL_TEST_ASSERT(s2.size() == 0);

    s2 = s1.substr(1, 0);
    ASL_TEST_ASSERT(s2.size() == 0);
}
