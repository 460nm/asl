#include "asl/format.hpp"

#include <cstdio>

// @Todo Use something like ryu or dragonbox

void asl::AslFormat(Formatter& f, float value)
{
    static constexpr isize_t kBufferLength = 64;
    char buffer[kBufferLength];
    int output_length = snprintf(buffer, kBufferLength, "%f", (double)value);
    f.write(string_view(buffer, output_length));
}

void asl::AslFormat(Formatter& f, double value)
{
    static constexpr isize_t kBufferLength = 64;
    char buffer[kBufferLength];
    int output_length = snprintf(buffer, kBufferLength, "%f", value);
    f.write(string_view(buffer, output_length));
}
