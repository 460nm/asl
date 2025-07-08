// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/config.hpp"

#if defined(ASL_COMPILER_CLANG_CL)
    #define ASL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif defined(ASL_COMPILER_CLANG)
    #define ASL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
