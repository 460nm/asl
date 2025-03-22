// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/utility.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/functional.hpp"

namespace asl
{

template<typename T>
class function_ref;

template<typename R, typename... Args>
class function_ref<R(Args...)>
{
    using InvokeFn = R (*)(Args..., void*);

    void*       m_obj;
    InvokeFn    m_invoke;

    template<typename T>
    static R invoke(Args... args, void* obj)
    {
        // NOLINTNEXTLINE(*-reinterpret-cast)
        return asl::invoke(*reinterpret_cast<T*>(obj), std::forward<Args>(args)...);
    }

public:
    function_ref() = delete;

    ASL_DEFAULT_COPY_MOVE(function_ref);
    ~function_ref() = default;

    template<typename T>
    function_ref(T&& t) // NOLINT(*-missing-std-forward, *explicit*)
        requires (
            !same_as<un_cvref_t<T>, function_ref>
            && invocable<T, Args...>
            && same_as<invoke_result_t<T, Args...>, R>
        )
        // NOLINTNEXTLINE(*cast*)
        : m_obj{const_cast<void*>(reinterpret_cast<const void*>(&t))}
        , m_invoke{invoke<un_ref_t<T>>}
    {}

    template<typename T>
    function_ref& operator=(T&& t) // NOLINT(*-missing-std-forward)
        requires (
            !same_as<un_cvref_t<T>, function_ref>
            && invocable<T, Args...>
            && same_as<invoke_result_t<T, Args...>, R>
        )
    {
        // NOLINTNEXTLINE(*cast*)
        m_obj = const_cast<void*>(reinterpret_cast<const void*>(&t));
        m_invoke = invoke<un_ref_t<T>>;

        return *this;
    }

    constexpr R operator()(this function_ref self, Args... args)
    {
        return self.m_invoke(std::forward<Args>(args)..., self.m_obj);
    }
};

} // namespace asl
