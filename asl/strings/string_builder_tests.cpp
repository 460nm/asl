// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/strings/string_builder.hpp"
#include "asl/testing/testing.hpp"

ASL_TEST(string_builder)
{
    asl::StringBuilder b;
    b.push('a');
    b.push("bcdef");
    b.push('g');

    ASL_TEST_EXPECT(b.as_string_view() == "abcdefg");

    const asl::string s = b.as_string();
    ASL_TEST_EXPECT(s == "abcdefg");
}

ASL_TEST(string_builder_rvalue)
{
    const asl::string s = asl::StringBuilder{}.push('a').push("bcdef").push('g').finish();

    ASL_TEST_EXPECT(s == "abcdefg");
}

