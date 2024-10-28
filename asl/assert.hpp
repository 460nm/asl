#pragma once

// @Todo Make this portable-ish
#define ASL_ASSERT(...)                                                 \
    if (__VA_ARGS__) {}                                                 \
    else { __builtin_debugtrap(); }
