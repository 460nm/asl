// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"

#include "asl/base/assert.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/utility.hpp"
#include "asl/io/print.hpp"

namespace
{

struct TestingState
{
    asl::testing::Test* head = nullptr;
    asl::testing::Test* tail = nullptr;

    bool current_test_fail = false;
};

}  // namespace

// NOLINTNEXTLINE(*-avoid-non-const-global-variables)
static TestingState g_state{};

void asl::testing::register_test(Test* test)
{
    if (g_state.head == nullptr && g_state.tail == nullptr)
    {
        g_state.head = test;
        g_state.tail = test;
    }
    else
    {
        g_state.tail->m_next = test;
        test->m_prev = asl::exchange(g_state.tail, test);
    }
}

void asl::testing::report_failure(const char* msg, const asl::source_location& sl)
{
    asl::eprint("--------------------------------------------------------------\n");
    asl::eprint("Test assertion failed at {}, line {}:\n", sl.file, sl.line);
    asl::eprint("    {}\n", msg);
    asl::eprint("--------------------------------------------------------------\n");
    g_state.current_test_fail = true;
}

static void report_assert_failure(const char* msg, const asl::source_location& sl, void* /* userdata */)
{
    asl::eprint("------------------------------------------------------------\n");
    asl::eprint("Assertion failure at {}, line {}:\n", sl.file, sl.line);
    asl::eprint("{}\n", msg);
    asl::eprint("------------------------------------------------------------\n");
}

#define RESET "\x1b[0m"
#define RED(S) "\x1b[0;31m" S RESET
#define GREEN(S) "\x1b[0;32m" S RESET

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    asl::set_assert_failure_handler(report_assert_failure, nullptr);

    int fail = 0;
    int pass = 0;

    asl::testing::Test* failed_head = nullptr;

    for (auto* it = g_state.head; it != nullptr; it = it->m_next)
    {
        asl::eprint(GREEN("[ RUN      ]") " {}\n", it->m_case_name);

        g_state.current_test_fail = false;
        it->m_fn();

        if (!g_state.current_test_fail)
        {
            asl::eprint(GREEN("[       OK ]") " {}\n", it->m_case_name);
            pass += 1;
        }
        else
        {
            asl::eprint(RED("[  FAILED  ]") " {}\n", it->m_case_name);
            fail += 1;

            it->m_next = asl::exchange(failed_head, it);
        }
    }

    asl::eprint(GREEN("[----------]") " {} test(s) run\n", fail + pass);

    if (fail == 0)
    {
        asl::eprint(GREEN("[  PASSED  ]") " Good job!\n");
    }
    else
    {
        asl::eprint(RED("[  FAILED  ]") " {} test(s) failed\n", fail);
        for (auto* it = failed_head; it != nullptr; it = it->m_next)
        {
            asl::eprint(RED("[  FAILED  ]") " {}\n", it->m_case_name);
        }
    }

    return fail;
}
