#pragma once

#include "asl/base/integers.hpp"
#include "asl/base/utility.hpp"
#include "asl/types/span.hpp"

namespace asl
{

class Writer
{
public:
    Writer() = default;
    ASL_DELETE_COPY_MOVE(Writer);
    virtual ~Writer() = default;

    virtual void write(span<const byte>) = 0;
};

} // namespace asl
