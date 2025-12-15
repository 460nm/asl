// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/memory.hpp"
#include "asl/base/integers.hpp"
#include "asl/base/byte.hpp"
#include "asl/base/assert.hpp"
#include "asl/allocator/allocator.hpp"

namespace asl
{

namespace function_detail
{

static constexpr isize_t kStorageSize = sizeof(void*) * 2;

union Storage
{
    Storage() {} // NOLINT(*-member-init)

    alignas(void*) std::byte inline_raw[kStorageSize];
    void* functor;
};

template<typename T>
concept can_be_stored_inline =
    sizeof(T) <= sizeof(Storage) &&
    alignof(Storage) % alignof(T) == 0;

template<typename R, typename... Args>
struct FunctionInterface
{
    FunctionInterface() = default;
    ASL_DELETE_COPY_MOVE(FunctionInterface);
    virtual ~FunctionInterface() = default;

    virtual void copy_to_uninit(const Storage& from, Storage& to) const = 0;
    virtual void move_to_uninit(Storage& from, Storage& to) const = 0;
    virtual void destroy(Storage&) const = 0;
    virtual R invoke(Args... args, const Storage& storage) const = 0;
};

template<typename Functor, bool kStoreInline, typename R, typename... Args>
struct FunctionImpl final : public FunctionInterface<R, Args...>
{
    using Allocator = DefaultAllocator;

    template<typename T>
    static void create(Storage& storage, T&& t)
    {
        Allocator allocator{};
        storage.functor = alloc_new<Functor>(allocator, std::forward<T>(t));
    }

    void copy_to_uninit(const Storage& from, Storage& to) const final
    {
        create(to, *static_cast<const Functor*>(from.functor));
    }

    void move_to_uninit(Storage& from, Storage& to) const final
    {
        to.functor = std::exchange(from.functor, nullptr);
    }

    void destroy(Storage& storage) const final
    {
        Allocator allocator{};
        alloc_delete(allocator, static_cast<Functor*>(storage.functor));
    }

    R invoke(Args... args, const Storage& storage) const final
    {
        return asl::invoke(
            *static_cast<Functor*>(storage.functor),
            std::forward<Args>(args)...);
    }
};

template<typename Functor, typename R, typename... Args>
struct FunctionImpl<Functor, true, R, Args...> final : public FunctionInterface<R, Args...>
{
    template<typename T>
    static void create(Storage& storage, T&& t)
    {
        construct_at<Functor>(&storage.inline_raw, std::forward<T>(t));
    }

    static const Functor& get_functor(const Storage& storage)
    {
        // NOLINTNEXTLINE(*-reinterpret-cast)
        return *std::launder(reinterpret_cast<const Functor*>(&storage.inline_raw));
    }

    static Functor& get_functor(Storage& storage)
    {
        // NOLINTNEXTLINE(*-reinterpret-cast)
        return *std::launder(reinterpret_cast<Functor*>(&storage.inline_raw));
    }

    void copy_to_uninit(const Storage& from, Storage& to) const final
    {
        create(to, get_functor(from));
    }

    void move_to_uninit(Storage& from, Storage& to) const final
    {
        create(to, get_functor(from));
        destroy(from);
    }

    void destroy(Storage& storage) const final
    {
        destroy_at(&get_functor(storage));
    }

    R invoke(Args... args, const Storage& storage) const final
    {
        auto* functor = const_cast<Functor*>(&get_functor(storage));
        return asl::invoke(*functor, std::forward<Args>(args)...);
    }
};

template<typename T, typename R, typename... Args>
concept valid_functor =
    copy_constructible<T>
    && move_constructible<T>
    && invocable<T, Args...>
    && same_as<R, invoke_result_t<T, Args...>>;

template<typename Functor, typename R, typename... Args>
const FunctionInterface<R, Args...>* make_function_impl(valid_functor<R, Args...> auto&& f, Storage& storage)
{
    using Impl = FunctionImpl<Functor, can_be_stored_inline<Functor>, R, Args...>;
    static const Impl impl{};
    Impl::create(storage, std::forward<Functor>(f));
    return &impl;
}

}  // namespace function_detail

template<typename T>
class function;

template<typename R, typename... Args>
class function<R(Args...)>
{
    function_detail::Storage m_storage;
    const function_detail::FunctionInterface<R, Args...>* m_impl{};

    void destroy()
    {
        if (m_impl != nullptr)
        {
            m_impl->destroy(m_storage);
        }
    }

public:
    function() = default;

    template<typename T>
    function(T&& func) // NOLINT(*explicit*)
        requires (
            !same_as<function, remove_cvref_t<T>>
            && function_detail::valid_functor<T, R, Args...>
        )
        : m_impl{
            function_detail::make_function_impl<decay_t<T>, R, Args...>(
                std::forward<T>(func), m_storage)}
    {
    }

    function(const function& other)
        : m_impl{other.m_impl}
    {
        if (m_impl != nullptr)
        {
            m_impl->copy_to_uninit(other.m_storage, m_storage);
        }
    }

    function(function&& other)
        : m_impl{std::exchange(other.m_impl, nullptr)}
    {
        if (m_impl != nullptr)
        {
            m_impl->move_to_uninit(other.m_storage, m_storage);
        }
    }

    ~function()
    {
        destroy();
    }

    function& operator=(const function& other)
    {
        if (this != &other)
        {
            destroy();

            m_impl = other.m_impl;
            if (m_impl)
            {
                m_impl->copy_to_uninit(other.m_storage, m_storage);
            }
        }
        return *this;
    }

    function& operator=(function&& other)
    {
        if (this != &other)
        {
            destroy();

            m_impl = std::exchange(other.m_impl, nullptr);
            if (m_impl)
            {
                m_impl->move_to_uninit(other.m_storage, m_storage);
            }
        }
        return *this;
    }

    template<typename T>
    function& operator=(T&& func)
        requires (
            !same_as<function, remove_cvref_t<T>>
            && function_detail::valid_functor<T, R, Args...>
        )
    {
        destroy();

        m_impl = function_detail::make_function_impl<decay_t<T>, R, Args...>(
            std::forward<T>(func), m_storage);

        return *this;
    }

    constexpr R operator()(Args... args) const
    {
        ASL_ASSERT(m_impl);
        return m_impl->invoke(std::forward<Args>(args)..., m_storage);
    }
};

} // namespace asl
