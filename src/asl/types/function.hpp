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

struct Storage
{
    alignas(alignof(void*)) std::byte raw[kStorageSize];

    [[nodiscard]]
    void* get_ptr() const
    {
        // NOLINTNEXTLINE(*-const-cast)
        return const_cast<void*>(static_cast<const void*>(raw));
    }
};

template<typename T>
concept can_be_stored_inline =
    sizeof(T) <= sizeof(Storage) &&
    alignof(Storage) % alignof(T) == 0;

enum class FunctionOp : uint8_t
{
    kDestroyThis,
    kCopyFromOtherToThisUninit,
    kMoveFromOtherToThisUninit,
};

template<typename Functor, bool kStoreInline = can_be_stored_inline<Functor>>
struct FunctionImplBase
{
    using Allocator = DefaultAllocator;

    template<typename T>
    static void create(Storage* storage, T&& t)
    {
        Allocator allocator{};
        auto* ptr = alloc_new<Functor>(allocator, std::forward<T>(t));
        asl::memcpy(storage->get_ptr(), static_cast<void*>(&ptr), sizeof(void*));
    }

    static Functor** get_functor_ptr(const Storage* storage)
    {
        // NOLINTNEXTLINE(*-reinterpret-cast)
        return std::launder(reinterpret_cast<Functor**>(storage->get_ptr()));
    }

    static Functor* get_functor(const Storage* storage)
    {
        return *get_functor_ptr(storage);
    }

    static void op(Storage* this_storage, Storage* other_storage, FunctionOp op)
    {
        switch (op)
        {
            using enum FunctionOp;
            case kDestroyThis:
            {
                Allocator allocator{};
                alloc_delete(allocator, get_functor(this_storage));
                break;
            }
            case kCopyFromOtherToThisUninit:
            {
                create(this_storage, *static_cast<const Functor*>(get_functor(other_storage)));
                break;
            }
            case kMoveFromOtherToThisUninit:
            {
                auto* ptr = std::exchange(*get_functor_ptr(other_storage), nullptr);
                asl::memcpy(this_storage->get_ptr(), static_cast<void*>(&ptr), sizeof(void*));
                break;
            }
            default: break;
        }
    }
};

template<typename Functor>
struct FunctionImplBase<Functor, true>
{
    template<typename T>
    static void create(Storage* storage, T&& t)
    {
        new (storage->get_ptr()) Functor(std::forward<T>(t));
    }

    static Functor* get_functor(const Storage* storage)
    {
        // NOLINTNEXTLINE(*-reinterpret-cast)
        return std::launder(reinterpret_cast<Functor*>(storage->get_ptr()));
    }

    static void op(Storage* this_storage, Storage* other_storage, FunctionOp op)
    {
        switch (op)
        {
            using enum FunctionOp;
            case kDestroyThis:
            {
                destroy(get_functor(this_storage));
                break;
            }
            case kCopyFromOtherToThisUninit:
            {
                create(this_storage, *static_cast<const Functor*>(get_functor(other_storage)));
                break;
            }
            case kMoveFromOtherToThisUninit:
            {
                auto* other_functor = get_functor(other_storage);
                create(this_storage, std::move(*static_cast<const Functor*>(other_functor)));
                destroy(other_functor);
                break;
            }
            default: break;
        }
    }
};

template<typename Functor, typename R, typename... Args>
struct FunctionImpl : FunctionImplBase<Functor>
{
    static R invoke(Args... args, const Storage& storage)
    {
        auto* functor = FunctionImplBase<Functor>::get_functor(&storage);
        return asl::invoke(*functor, std::forward<Args>(args)...);
    }
};


template<typename T, typename R, typename... Args>
concept valid_functor =
    copy_constructible<T>
    && move_constructible<T>
    && invocable<T, Args...>
    && same_as<R, invoke_result_t<T, Args...>>;

}  // namespace function_detail

template<typename T>
class function;

template<typename R, typename... Args>
class function<R(Args...)> // NOLINT(*-member-init)
{
    using InvokeFn = R (*)(Args..., const function_detail::Storage&);
    using OpFn = void (*)(function_detail::Storage*, function_detail::Storage*, function_detail::FunctionOp);

    function_detail::Storage m_storage;
    InvokeFn                 m_invoke{};
    OpFn                     m_op{};

    void destroy()
    {
        if (m_op != nullptr)
        {
            (*m_op)(&m_storage, nullptr, function_detail::FunctionOp::kDestroyThis);
        }
    }

public:
    function() = default;

    template<typename T>
    function(T&& func) // NOLINT(*explicit*,*-member-init)
        requires (
            !same_as<function, remove_cvref_t<T>>
            && function_detail::valid_functor<T, R, Args...>
        )
    {
        using Functor = decay_t<T>;
        using Impl = function_detail::FunctionImpl<Functor, R, Args...>;

        Impl::create(&m_storage, std::forward<T>(func));
        m_invoke = &Impl::invoke; // NOLINT(*-member-initializer)
        m_op = &Impl::op; // NOLINT(*-member-initializer)
    }

    function(const function& other) // NOLINT(*-member-init)
        : m_invoke{other.m_invoke}
        , m_op{other.m_op}
    {
        if (m_op != nullptr)
        {
            m_op(
                &m_storage,
                const_cast<function_detail::Storage*>(&other.m_storage), // NOLINT(*-const-cast)
                function_detail::FunctionOp::kCopyFromOtherToThisUninit);
        }
    }

    function(function&& other) // NOLINT(*-member-init)
        : m_invoke{std::exchange(other.m_invoke, nullptr)}
        , m_op{std::exchange(other.m_op, nullptr)}
    {
        if (m_op != nullptr)
        {
            m_op(
                &m_storage,
                &other.m_storage,
                function_detail::FunctionOp::kMoveFromOtherToThisUninit);
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

            m_invoke = other.m_invoke;
            m_op = other.m_op;

            m_op(
                &m_storage,
                const_cast<function_detail::Storage*>(&other.m_storage), // NOLINT(*-const-cast)
                function_detail::FunctionOp::kCopyFromOtherToThisUninit);
        }
        return *this;
    }

    function& operator=(function&& other)
    {
        if (this != &other)
        {
            destroy();

            m_invoke = std::exchange(other.m_invoke, nullptr);
            m_op = std::exchange(other.m_op, nullptr);

            m_op(
                &m_storage,
                &other.m_storage,
                function_detail::FunctionOp::kMoveFromOtherToThisUninit);
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

        using Functor = decay_t<T>;
        using Impl = function_detail::FunctionImpl<Functor, R, Args...>;

        Impl::create(&m_storage, std::forward<T>(func));
        m_invoke = &Impl::invoke;
        m_op = &Impl::op;

        return *this;
    }

    constexpr R operator()(Args... args) const
    {
        ASL_ASSERT(m_invoke);
        return m_invoke(args..., m_storage);
    }
};

} // namespace asl
