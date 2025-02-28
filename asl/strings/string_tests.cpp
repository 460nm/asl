// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/strings/string.hpp"
#include "asl/testing/testing.hpp"
#include "asl/formatting/format.hpp"

ASL_TEST(default)
{
    asl::string s;
    ASL_TEST_ASSERT(s.size() == 0);
    ASL_TEST_ASSERT(s.as_string_view().size() == 0);
    ASL_TEST_ASSERT(s == ""_sv);
    ASL_TEST_ASSERT(s == s);
}

ASL_TEST(from_string_view)
{
    asl::string s = "hello"_sv;
    ASL_TEST_ASSERT(s.size() == 5);
    ASL_TEST_ASSERT(s == "hello"_sv);
}

static_assert(asl::formattable<asl::string<>>);
