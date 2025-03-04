// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/formatting/format.hpp"
#include "asl/io/writer.hpp"

namespace asl
{

namespace print_internals
{

// @Todo Make print writers thread safe
Writer* get_stdout_writer();
Writer* get_stderr_writer();

} // namespace print_internals

template<formattable... Args>
void print(string_view fmt, const Args&... args)
{
    format(print_internals::get_stdout_writer(), fmt, args...);
}

template<formattable... Args>
void eprint(string_view fmt, const Args&... args)
{
    format(print_internals::get_stderr_writer(), fmt, args...);
}

} // namespace asl
