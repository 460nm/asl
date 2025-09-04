// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/byte.hpp"
#include "asl/types/span.hpp"

namespace asl
{

class Writer
{
public:
    Writer() = default;
    ASL_DELETE_COPY_MOVE(Writer);
    virtual ~Writer() = default;

    virtual void write(span<const std::byte>) = 0;
};

} // namespace asl
