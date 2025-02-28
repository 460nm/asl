// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/formatting/format.hpp"
#include "asl/testing/testing.hpp"
#include "asl/base/float.hpp"
#include "asl/strings/string_builder.hpp"

static_assert(asl::formattable<decltype("Hello")>);

ASL_TEST(format_args)
{
    // @Todo Introduce ASL_TEST_EXPECT_EQ, or ASL_TEST_EXPECT_STREQ

    auto s = asl::format_to_string("Hello, world!");
    ASL_TEST_EXPECT(s == "Hello, world!"_sv);

    s = asl::format_to_string("");
    ASL_TEST_EXPECT(s == ""_sv);

    s = asl::format_to_string("Hello, {}!", "world");
    ASL_TEST_EXPECT(s == "Hello, world!"_sv);

    s = asl::format_to_string("Hello, {}! {}", "world");
    ASL_TEST_EXPECT(s == "Hello, world! <ERROR>"_sv);

    s = asl::format_to_string("Hello, pup!", "world");
    ASL_TEST_EXPECT(s == "Hello, pup!"_sv);

    s = asl::format_to_string("{}", "CHEESE");
    ASL_TEST_EXPECT(s == "CHEESE"_sv);

    s = asl::format_to_string("{   ", "CHEESE");
    ASL_TEST_EXPECT(s == "<ERROR>   "_sv);

    s = asl::format_to_string("{", "CHEESE");
    ASL_TEST_EXPECT(s == "<ERROR>"_sv);

    s = asl::format_to_string("a{{b");
    ASL_TEST_EXPECT(s == "a{b"_sv);

    s = asl::format_to_string("{{{}}} }", "CHEESE");
    ASL_TEST_EXPECT(s == "{CHEESE} }"_sv);
}

ASL_TEST(format_integers)
{
    auto s = asl::format_to_string("{} {} {}", 0, 1, 2);
    ASL_TEST_EXPECT(s == "0 1 2"_sv);

    s = asl::format_to_string("{} {} {}", 10, 11, 12);
    ASL_TEST_EXPECT(s == "10 11 12"_sv);

    s = asl::format_to_string("{} {} {}", 100, 101, 102);
    ASL_TEST_EXPECT(s == "100 101 102"_sv);

    s = asl::format_to_string("{} {} {}", 1000, 1001, 1002);
    ASL_TEST_EXPECT(s == "1000 1001 1002"_sv);

    s = asl::format_to_string("{} {} {} {}", -1, -23, -456, -7890);
    ASL_TEST_EXPECT(s == "-1 -23 -456 -7890"_sv);
}

ASL_TEST(format_floats)
{
    auto s = asl::format_to_string("{} {} {}", 0.0F, 1.0, 2.0F);
    ASL_TEST_EXPECT(s == "0 1 2"_sv);

    s = asl::format_to_string("{} {} {}", 0.1F, 0.001F, 0.123F);
    ASL_TEST_EXPECT(s == "0.1 0.001 0.123"_sv);

    s = asl::format_to_string("{} {}", 1.25F, -22.3);
    ASL_TEST_EXPECT(s == "1.25 -22.3"_sv);

    s = asl::format_to_string("{}", 1e32);
    ASL_TEST_EXPECT(s == "100000000000000000000000000000000"_sv);

    s = asl::format_to_string("{}", 123e-8);
    ASL_TEST_EXPECT(s == "0.00000123"_sv);

    s = asl::format_to_string("{} {}", asl::infinity<float>(), -asl::infinity<double>());
    ASL_TEST_EXPECT(s == "Infinity -Infinity"_sv);

    s = asl::format_to_string("{}", asl::nan<float>());
    ASL_TEST_EXPECT(s == "NaN"_sv);
}

ASL_TEST(format_boolean)
{
    auto s = asl::format_to_string("{} {}", true, false);
    ASL_TEST_EXPECT(s == "true false"_sv);
}

struct CustomFormat
{
    int x;
    friend void AslFormat(asl::Formatter&, const CustomFormat&);
};

void AslFormat(asl::Formatter& f, const CustomFormat& c)
{
    f.write("("_sv);
    AslFormat(f, c.x);
    f.write(")"_sv);
}

static_assert(asl::formattable<CustomFormat>);

ASL_TEST(format_custom)
{
    auto s = asl::format_to_string("{}", CustomFormat{37});
    ASL_TEST_EXPECT(s == "(37)"_sv);
}
