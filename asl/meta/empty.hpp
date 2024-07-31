#pragma once

#include "asl/meta/types.hpp"
#include "asl/meta/quals.hpp"

namespace asl {

template<typename T> concept is_void = same<un_qual_t<T>, void>;

} // namespace asl

