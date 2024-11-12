#pragma once

#include "asl/integers.hpp"
#include "asl/utility.hpp"
#include "asl/span.hpp"

namespace asl
{

class writer
{
public:
    writer() = default;
    ASL_DELETE_COPY_MOVE(writer);
    virtual ~writer() = default;
    
    virtual void write(span<const byte>) = 0;
};

} // namespace asl
