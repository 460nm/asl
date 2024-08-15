#pragma once

#include "asl/ptr.hpp"

namespace asl {

template<typename T>
class object final
{
    using wrapped = devoid_t<un_qual_t<typename ptr<T>::pointee>>;

    ASL_NO_UNIQUE_ADDRESS wrapped m_value;
    
public:
    object() requires default_constructible<wrapped> = default;

    object(const object&) = default;
    object(object&&) = default;

    object& operator=(const object&) = default;
    object& operator=(object&&) = default;

    ~object() = default;
};

} // namespace asl
