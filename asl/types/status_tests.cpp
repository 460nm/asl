// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/types/status.hpp"
#include "asl/testing/testing.hpp"
#include "asl/formatting/format.hpp"
#include "asl/strings/string_builder.hpp"

ASL_TEST(simple_ok)
{
    const asl::status s = asl::ok();
    ASL_TEST_ASSERT(s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::ok);
}

ASL_TEST(simple_code)
{
    const asl::status s = asl::runtime_error();
    ASL_TEST_ASSERT(!s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::runtime);
    ASL_TEST_ASSERT(s.message() == ""_sv);
}

ASL_TEST(with_message)
{
    const asl::status s = asl::internal_error("We done goofed");
    ASL_TEST_ASSERT(!s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::internal);
    ASL_TEST_ASSERT(s.message() == "We done goofed"_sv);
}

ASL_TEST(copy_inline)
{
    const asl::status s = asl::ok();
    const asl::status s2 = asl::internal_error();

    asl::status s3 = s;
    ASL_TEST_ASSERT(s3.code() == asl::status_code::ok);

    s3 = s2;
    ASL_TEST_ASSERT(s3.code() == asl::status_code::internal);
}

ASL_TEST(copy_message)
{
    asl::status s2 = asl::ok();

    {
        const asl::status s = asl::internal_error("Oh no!");
        ASL_TEST_ASSERT(!s.ok());
        ASL_TEST_ASSERT(s.code() == asl::status_code::internal);
        ASL_TEST_ASSERT(s.message() == "Oh no!"_sv);

        const asl::status s3{s}; // NOLINT
        ASL_TEST_ASSERT(!s3.ok());
        ASL_TEST_ASSERT(s3.code() == asl::status_code::internal);
        ASL_TEST_ASSERT(s3.message() == "Oh no!"_sv);

        s2 = s;
        ASL_TEST_ASSERT(!s2.ok());
        ASL_TEST_ASSERT(s2.code() == asl::status_code::internal);
        ASL_TEST_ASSERT(s2.message() == "Oh no!"_sv);
    }

    ASL_TEST_ASSERT(!s2.ok());
    ASL_TEST_ASSERT(s2.code() == asl::status_code::internal);
    ASL_TEST_ASSERT(s2.message() == "Oh no!"_sv);
}

static_assert(asl::formattable<asl::status>);

ASL_TEST(format)
{
    auto s = asl::format_to_string("-{}-", asl::ok());
    ASL_TEST_EXPECT(s == "-[ok]-"_sv);

    s = asl::format_to_string("-{}-", asl::internal_error("hello"));
    ASL_TEST_EXPECT(s == "-[internal: hello]-"_sv);
}

ASL_TEST(make_with_format)
{
    auto s = asl::format_to_string("-{}-", asl::internal_error("hello, {}, {}", 45, "world"));
    ASL_TEST_EXPECT(s == "-[internal: hello, 45, world]-"_sv);
}
