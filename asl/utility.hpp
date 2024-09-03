#pragma once

#include "asl/meta.hpp"

#define ASL_MOVE(expr_) (static_cast<::asl::un_ref_t<decltype(expr_)>&&>(expr_))

#define ASL_FWD(expr_) (static_cast<decltype(expr_)&&>(expr_))

namespace asl
{

template<typename T, typename U>
T exchange(T& obj, U&& new_value)
{
    T old_value = ASL_MOVE(obj);
    obj = ASL_FORWARD(new_value);
    return old_value;
}

} // namespace asl
