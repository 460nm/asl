#pragma once

#include "asl/integers.hpp"
#include "asl/utility.hpp"
#include "asl/span.hpp"

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
