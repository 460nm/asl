#pragma once

#include "asl/format.hpp"

namespace asl
{

namespace print_internals
{

// @Todo Make print writers thread safe
writer* get_stdout_writer();
writer* get_stderr_writer();

} // namespace print_internals

// @Todo Use string_view
template<formattable... Args>
void print(const char* fmt, const Args&... args)
{
    format(print_internals::get_stdout_writer(), fmt, args...);
}

// @Todo Use string_view
template<formattable... Args>
void eprint(const char* fmt, const Args&... args)
{
    format(print_internals::get_stderr_writer(), fmt, args...);
}

} // namespace asl
