#include "asl/testing/testing.hpp"

#include "asl/io/print.hpp"

static asl::testing::Test* g_head = nullptr;
static asl::testing::Test* g_tail = nullptr;

void asl::testing::register_test(Test* test)
{
    if (g_head == nullptr && g_tail == nullptr)
    {
        g_head = test;
        g_tail = test;
    }
    else
    {
        g_tail->m_next = test;
        test->m_prev = asl::exchange(g_tail, test);
    }
}

static bool g_current_test_fail = false;

void asl::testing::report_failure(const char* msg, const char* file, int line)
{
    asl::eprint("--------------------------------------------------------------\n");
    asl::eprint("Test assertion failed at {}, line {}:\n", file, line);
    asl::eprint("    {}\n", msg);
    asl::eprint("--------------------------------------------------------------\n");
    g_current_test_fail = true;
}

#define RESET "\x1b[0m"
#define RED(S) "\x1b[0;31m" S RESET
#define GREEN(S) "\x1b[0;32m" S RESET

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    int fail = 0;
    int pass = 0;

    asl::testing::Test* failed_head = nullptr;

    for (auto* it = g_head; it != nullptr; it = it->m_next)
    {
        asl::eprint(GREEN("[ RUN      ]") " {}\n", it->m_case_name);

        g_current_test_fail = false;
        it->m_fn();

        if (!g_current_test_fail)
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
