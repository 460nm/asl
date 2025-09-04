// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/types/status_or.hpp"
#include "asl/strings/string_view.hpp"
#include "asl/base/floats.hpp"

namespace asl
{

template<typename T>
struct parse_number_result
{
    T value;
    string_view remaining;
};

status_or<parse_number_result<float32_t>> parse_float32(string_view);
status_or<parse_number_result<float64_t>> parse_float64(string_view);

status_or<parse_number_result<uint8_t>>  parse_uint8(string_view, int base = 10);
status_or<parse_number_result<uint16_t>> parse_uint16(string_view, int base = 10);
status_or<parse_number_result<uint32_t>> parse_uint32(string_view, int base = 10);
status_or<parse_number_result<uint64_t>> parse_uint64(string_view, int base = 10);

status_or<parse_number_result<int8_t>>  parse_int8(string_view, int base = 10);
status_or<parse_number_result<int16_t>> parse_int16(string_view, int base = 10);
status_or<parse_number_result<int32_t>> parse_int32(string_view, int base = 10);
status_or<parse_number_result<int64_t>> parse_int64(string_view, int base = 10);

} // namespace asl

