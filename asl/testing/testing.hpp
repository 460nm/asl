// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"

namespace asl::testing
{

struct Test;

void register_test(Test*);

void report_failure(const char* msg, const asl::source_location& = asl::source_location{});

using TestFunction = void();

struct Test
{
    const char*   m_case_name;
    TestFunction* m_fn;
    Test*         m_next{};
    Test*         m_prev{};

    constexpr explicit Test(const char* case_name, TestFunction* fn)
        : m_case_name{case_name}
        , m_fn{fn}
    {
        register_test(this);
    }
};

} // namespace asl::testing

#define ASL_TEST(CASE)                              \
    static void asl_test_fn_##CASE(); /* NOLINT */  \
    /* NOLINTNEXTLINE */                            \
    static ::asl::testing::Test asl_test_##CASE(    \
        #CASE,                                      \
        asl_test_fn_##CASE);                        \
    void asl_test_fn_##CASE()

#define ASL_TEST_ASSERT(EXPR) \
    if (EXPR) {}              \
    else { ::asl::testing::report_failure(#EXPR); return; }

#define ASL_TEST_EXPECT(...) \
    if (__VA_ARGS__) {}      \
    else { ::asl::testing::report_failure(#__VA_ARGS__); }
