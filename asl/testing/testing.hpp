#pragma once

namespace asl::testing
{

using TestFunction = void();
int register_test(const char* suite_name, const char* case_name, TestFunction* fn);

} // namespace asl::testing

#define ASL_TEST(SUITE, CASE)                                                      \
    static void asl_test_fn_##SUITE##_##CASE();                                    \
    static const int asl_test_##SUITE##_##CASE = ::asl::testing::register_test(    \
        #SUITE, #CASE, asl_test_fn_##SUITE##_##CASE);                              \
    void asl_test_fn_##SUITE##_##CASE()
