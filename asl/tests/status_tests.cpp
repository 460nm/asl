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
    ASL_TEST_ASSERT(s.message() == ""_sv);
}

ASL_TEST(with_message)
{
    asl::status s{asl::status_code::internal, "We done goofed"};
    ASL_TEST_ASSERT(!s);
    ASL_TEST_ASSERT(!s.ok());
    ASL_TEST_ASSERT(s.code() == asl::status_code::internal);
    ASL_TEST_ASSERT(s.message() == "We done goofed"_sv);
}

ASL_TEST(copy_inline)
{
    asl::status s{asl::status_code::ok};
    asl::status s2{asl::status_code::internal};
    
    asl::status s3 = s;
    ASL_TEST_ASSERT(s3.code() == asl::status_code::ok);
    
    s3 = s2;
    ASL_TEST_ASSERT(s3.code() == asl::status_code::internal);
}

ASL_TEST(copy_message)
{
    asl::status s2;

    {
        asl::status s{asl::status_code::internal, "Oh no!"};
        ASL_TEST_ASSERT(!s);
        ASL_TEST_ASSERT(!s.ok());
        ASL_TEST_ASSERT(s.code() == asl::status_code::internal);
        ASL_TEST_ASSERT(s.message() == "Oh no!"_sv);

        const asl::status s3{s}; // NOLINT
        ASL_TEST_ASSERT(!s3);
        ASL_TEST_ASSERT(!s3.ok());
        ASL_TEST_ASSERT(s3.code() == asl::status_code::internal);
        ASL_TEST_ASSERT(s3.message() == "Oh no!"_sv);

        s2 = s;
        ASL_TEST_ASSERT(!s2);
        ASL_TEST_ASSERT(!s2.ok());
        ASL_TEST_ASSERT(s2.code() == asl::status_code::internal);
        ASL_TEST_ASSERT(s2.message() == "Oh no!"_sv);
    }

    ASL_TEST_ASSERT(!s2);
    ASL_TEST_ASSERT(!s2.ok());
    ASL_TEST_ASSERT(s2.code() == asl::status_code::internal);
    ASL_TEST_ASSERT(s2.message() == "Oh no!"_sv);
}
