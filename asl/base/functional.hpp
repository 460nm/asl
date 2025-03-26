// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/utility.hpp"

namespace asl {

template<typename F, typename... Args>
constexpr auto invoke(F&& f, Args&&... args)
    -> decltype(std::forward<F>(f)(std::forward<Args>(args)...))
    requires (!is_member_ptr<un_cvref_t<F>>) && requires {
        f(std::forward<Args>(args)...);
    }
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template<typename C>
constexpr auto&& invoke(auto C::* f, same_or_derived_from<C> auto&& arg)
{
    return std::forward<decltype(arg)>(arg).*f;
}

template<typename C>
constexpr auto&& invoke(auto C::* f, auto&& arg)
    requires (
        !same_or_derived_from<decltype(arg), C>
        && requires { (*arg).*f; }
    )
{
    return (*std::forward<decltype(arg)>(arg)).*f;
}

template<typename C, typename... Args>
constexpr auto invoke(is_func auto C::* f, same_or_derived_from<C> auto&& self, Args&&... args)
    -> decltype((std::forward<decltype(self)>(self).*f)(std::forward<Args>(args)...))
    requires requires { (self.*f)(std::forward<Args>(args)...); }
{
    return (std::forward<decltype(self)>(self).*f)(std::forward<Args>(args)...);
}

template<typename C, typename... Args>
constexpr auto invoke(is_func auto C::* f, auto&& self, Args&&... args)
    -> decltype(((*std::forward<decltype(self)>(self)).*f)(std::forward<Args>(args)...))
    requires (
        !same_or_derived_from<decltype(self), C>
        && requires { ((*self).*f)(std::forward<Args>(args)...); }
    )
{
    return ((*std::forward<decltype(self)>(self)).*f)(std::forward<Args>(args)...);
}

template<typename Void, typename F, typename... Args>
struct _invoke_result_helper;

template<typename R, typename... Args>
struct _invoke_result_helper<void, R, Args...>
{
    using type = decltype(invoke(declval<R>(), declval<Args>()...));
};

template<typename F, typename... Args>
using invoke_result_t = _invoke_result_helper<void, F, Args...>::type;

template<typename F, typename... Args>
concept invocable = requires (F&& f, Args&&... args)
{
    invoke(std::forward<F>(f), std::forward<Args>(args)...);
};

} // namespace asl
