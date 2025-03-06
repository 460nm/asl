// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/utility.hpp"

namespace asl {

template<typename... Args, typename C>
constexpr auto invoke(is_func auto C::* f, auto&& self, Args&&... args)
    -> decltype((self.*f)(std::forward<Args>(args)...))
    requires requires {
        (self.*f)(std::forward<Args>(args)...);
    }
{
    return (std::forward<decltype(self)>(self).*f)(std::forward<Args>(args)...);
}

template<typename... Args, typename C>
constexpr auto invoke(is_func auto C::* f, auto* self, Args&&... args)
    -> decltype((self->*f)(std::forward<Args>(args)...))
    requires requires {
        (self->*f)(std::forward<Args>(args)...);
    }
{
    return (self->*f)(std::forward<Args>(args)...);
}

template<typename... Args, typename C>
constexpr auto invoke(is_object auto C::* m, auto&& self, Args&&...)
    -> decltype(self.*m)
    requires (
        sizeof...(Args) == 0 &&
        requires { self.*m; }
    )
{
    return std::forward<decltype(self)>(self).*m;
}

template<typename... Args, typename C>
constexpr auto invoke(is_object auto C::* m, auto* self, Args&&...)
    -> decltype(self->*m)
    requires (
        sizeof...(Args) == 0 &&
        requires { self->*m; }
    )
{
    return self->*m;
}

template<typename... Args>
constexpr auto invoke(auto&& f, Args&&... args)
    -> decltype(f(std::forward<Args>(args)...))
    requires requires {
        f(std::forward<Args>(args)...);
    }
{
    return std::forward<decltype(f)>(f)(std::forward<Args>(args)...);
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
