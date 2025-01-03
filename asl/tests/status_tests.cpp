#include "asl/status.hpp"
#include "asl/testing/testing.hpp"

ASL_TEST(simple_ok)
{
    asl::status s;
    ASL_TEST_ASSERT(s);
    ASL_TEST_ASSERT(s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::ok);
}

ASL_TEST(simple_code)
{
    asl::status s{asl::status_code::runtime};
    ASL_TEST_ASSERT(!s);
    ASL_TEST_ASSERT(!s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::runtime);
}

ASL_TEST(with_message)
{
    asl::status s{asl::status_code::internal, "We done goofed"};
    ASL_TEST_ASSERT(!s);
    ASL_TEST_ASSERT(!s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::internal);
    // @Todo test message
}
