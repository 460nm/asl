#include "asl/testing/testing.hpp"

#include <asl/print.hpp>

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
    asl::eprint("    {}:\n", msg);
    asl::eprint("--------------------------------------------------------------\n");
    g_current_test_fail = true;
}

#define RESET "\x1b[0m"
#define RED "\x1b[0;31m"
#define GREEN "\x1b[0;32m"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    int fail = 0;
    int pass = 0;
    
    for (auto* it = g_head; it != nullptr; it = it->m_next)
    {
        asl::eprint(GREEN "[ RUN      ]" RESET " {}\n", it->m_case_name);
        
        g_current_test_fail = false;
        it->m_fn();
        
        if (!g_current_test_fail)
        {
            asl::eprint(GREEN "[       OK ]" RESET " {}\n", it->m_case_name);
            pass += 1;
        }
        else
        {
            asl::eprint(RED "[  FAILED  ]" RESET " {}\n", it->m_case_name);
            fail += 1;
        }
    }
    
    asl::eprint(GREEN "[----------]" RESET " {} test(s) run\n", fail + pass);
    
    if (fail == 0)
    {
        asl::eprint(GREEN "[  PASSED  ]" RESET " Good job!\n");
    }
    else
    {
        asl::eprint(RED "[  FAILED  ]" RESET " {} test(s) failed\n", fail);
    }
    
    return 0;
}
