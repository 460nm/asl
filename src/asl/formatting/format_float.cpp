// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#define JKJ_STD_REPLACEMENT_NAMESPACE_DEFINED 0
#define JKJ_STATIC_DATA_SECTION_DEFINED 0
#include <dragonbox.h>
#pragma clang diagnostic pop

namespace asl
{

extern void jkj_dragonbox_to_decimal(
    float value,
    bool* is_negative,
    int* exponent,
    uint64_t* significand);

extern void jkj_dragonbox_to_decimal(
    double value,
    bool* is_negative,
    int* exponent,
    uint64_t* significand);

void jkj_dragonbox_to_decimal(
    float value,
    bool* is_negative,
    int* exponent,
    uint64_t* significand)
{
    auto decimal = jkj::dragonbox::to_decimal(value);
    *is_negative = decimal.is_negative;
    *exponent = decimal.exponent;
    *significand = decimal.significand;
}

void jkj_dragonbox_to_decimal(
    double value,
    bool* is_negative,
    int* exponent,
    uint64_t* significand)
{
    auto decimal = jkj::dragonbox::to_decimal(value);
    *is_negative = decimal.is_negative;
    *exponent = decimal.exponent;
    *significand = decimal.significand;
}

} // namespace asl

