#pragma once

#include "asl/ptr.hpp"

namespace asl {

template<typename T>
class object final
{
    using wrapped = devoid_t<un_qual_t<typename ptr<T>::pointee>>;

    ASL_NO_UNIQUE_ADDRESS wrapped m_value;
    
public:
};

} // namespace asl
