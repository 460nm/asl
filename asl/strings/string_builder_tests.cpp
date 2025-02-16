#include "asl/strings/string_builder.hpp"
#include "asl/testing/testing.hpp"

ASL_TEST(string_builder)
{
    asl::StringBuilder b;
    b.push('a');
    b.push("bcdef");
    b.push('g');

    ASL_TEST_EXPECT(b.as_string_view() == "abcdefg");

    asl::string s = b.as_string();
    ASL_TEST_EXPECT(s == "abcdefg");
}

ASL_TEST(string_builder_rvalue)
{
    asl::string s = asl::StringBuilder{}.push('a').push("bcdef").push('g').finish();

    ASL_TEST_EXPECT(s == "abcdefg");
}

