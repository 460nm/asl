#include "asl/base/float.hpp"

#include "asl/testing/testing.hpp"

ASL_TEST(is_infinity)
{
    ASL_TEST_EXPECT(!asl::is_infinity(0.0F));
    ASL_TEST_EXPECT(!asl::is_infinity(-25.0F));
    ASL_TEST_EXPECT(asl::is_infinity(45.0F / 0.0F));
    ASL_TEST_EXPECT(asl::is_infinity(-45.0F / 0.0F));
    ASL_TEST_EXPECT(asl::is_infinity(asl::infinity<float>()));
    ASL_TEST_EXPECT(asl::is_infinity(-asl::infinity<double>()));
}

ASL_TEST(is_nan)
{
    ASL_TEST_EXPECT(!asl::is_nan(0.0F));
    ASL_TEST_EXPECT(!asl::is_nan(-25.0F));
    ASL_TEST_EXPECT(!asl::is_nan(45.0F / 0.0F));
    ASL_TEST_EXPECT(asl::is_nan(asl::nan<float>()));
    ASL_TEST_EXPECT(asl::is_nan(asl::nan<double>()));
}

