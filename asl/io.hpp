#pragma once

#include "asl/integers.hpp"
#include "asl/utility.hpp"

namespace asl
{

class writer
{
public:
    writer() = default;
    ASL_DELETE_COPY_MOVE(writer);
    virtual ~writer() = default;
    
    // @Todo Use string view, or span of bytes?
    virtual void write(const char* str, isize_t len) = 0;
};

} // namespace asl
