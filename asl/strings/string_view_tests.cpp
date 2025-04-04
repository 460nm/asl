// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/strings/string_view.hpp"
#include "asl/testing/testing.hpp"

static_assert(asl::trivially_destructible<asl::string_view>);
static_assert(asl::trivially_copy_constructible<asl::string_view>);

ASL_TEST(default)
{
    const asl::string_view s1;
    ASL_TEST_EXPECT(s1.is_empty());

    const asl::string_view s2 = nullptr;
    ASL_TEST_EXPECT(s2.is_empty());
}

ASL_TEST(from_literal)
{
    const asl::string_view s1 = "Hello"_sv;
    ASL_TEST_ASSERT(s1.size() == 5);
    ASL_TEST_EXPECT(asl::memcmp(s1.data(), "Hello", 5) == 0);

    const asl::string_view s2 = ""_sv;
    ASL_TEST_EXPECT(s2.is_empty());
}

ASL_TEST(from_zstr)
{
    const char* s1 = "";
    const char* s2 = "abc";
    const char* s3 = "abc\0def";

    auto sv1 = asl::string_view::from_zstr(s1);
    auto sv2 = asl::string_view::from_zstr(s2);
    auto sv3 = asl::string_view::from_zstr(s3);

    ASL_TEST_ASSERT(sv1.size() == 0);
    ASL_TEST_ASSERT(sv2.size() == 3);
    ASL_TEST_ASSERT(sv3.size() == 3);

    ASL_TEST_ASSERT(sv2 == "abc"_sv);
    ASL_TEST_ASSERT(sv3 == "abc"_sv);
}

ASL_TEST(substr1)
{
    const asl::string_view s1 = "abcd";

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
    const asl::string_view s1 = "abcd";

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
    const asl::string_view s1 = "abcd";

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
    const asl::string_view s1 = "abcd";

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
