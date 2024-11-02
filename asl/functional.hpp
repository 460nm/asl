#pragma once

#include "asl/meta.hpp"
#include "asl/utility.hpp"

namespace asl {

template<typename... Args, typename C>
constexpr auto invoke(is_func auto C::* f, auto&& self, Args&&... args)
    requires requires {
        (self.*f)(ASL_FWD(args)...);
    }
{
    return (ASL_FWD(self).*f)(ASL_FWD(args)...);
}

template<typename... Args, typename C>
constexpr auto invoke(is_func auto C::* f, auto* self, Args&&... args)
    requires requires {
        (self->*f)(ASL_FWD(args)...);
    }
{
    return (self->*f)(ASL_FWD(args)...);
}

template<typename... Args, typename C>
constexpr auto invoke(is_object auto C::* m, auto&& self, Args&&...)
    requires (
        sizeof...(Args) == 0 &&
        requires { self.*m; }
    )
{
    return ASL_FWD(self).*m;
}

template<typename... Args, typename C>
constexpr auto invoke(is_object auto C::* m, auto* self, Args&&...)
    requires (
        sizeof...(Args) == 0 &&
        requires { self->*m; }
    )
{
    return self->*m;
}

template<typename... Args>
constexpr auto invoke(auto&& f, Args&&... args)
    requires requires {
        f(ASL_FWD(args)...);
    }
{
    return ASL_FWD(f)(ASL_FWD(args)...);
}

template<typename F> struct _result_of_helper;

template<typename R, typename... Args>
struct _result_of_helper<R(Args...)>
{
    using type = decltype(invoke(declval<R>(), declval<Args>()...));
};

template<typename F> using result_of_t = _result_of_helper<F>::type;

} // namespace asl
