// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/config.hpp"

#if ASL_COMPILER_CLANG_CL
    #define ASL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif ASL_COMPILER_CLANG
    #define ASL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
