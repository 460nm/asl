#pragma once

#define ASL_ASSERT(...)                                                 \
    if (__VA_ARGS__) {}                                                 \
    else { __debugbreak(); }
