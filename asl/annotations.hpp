#pragma once

#ifndef __clang__
    #error Not a valid environment
#endif

#ifdef _MSC_VER
    #define ASL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
    #define ASL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
