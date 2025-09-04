// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/assert.hpp"

#include "asl/base/meta.hpp"

// NOLINTNEXTLINE(*-non-const-global-variables)
static asl::AssertFailureHandler* s_handler = nullptr;

// NOLINTNEXTLINE(*-non-const-global-variables)
static void* s_user = nullptr;

void asl::set_assert_failure_handler(AssertFailureHandler handler, void* user)
{
    s_handler = handler;
    s_user = user;
}

void asl::report_assert_failure(const char* msg, const source_location& sl)
{
    if (s_handler != nullptr)
    {
        s_handler(msg, sl, s_user);
    }
}
