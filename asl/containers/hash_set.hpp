// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/annotations.hpp"
#include "asl/base/utility.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/bit.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/memory/memory.hpp"
#include "asl/types/maybe_uninit.hpp"
#include "asl/hashing/hash.hpp"

namespace asl
{

template<typename H, typename T>
concept key_hasher = requires (const T& value)
{
    { H::hash(value) } -> same_as<uint64_t>;
};

template<hashable T>
struct default_key_hasher
{
    constexpr static uint64_t hash(const T& value)
    {
        return hash_value(value);
    }
};

template<typename C, typename U, typename V = U>
concept key_comparator = requires(const U& a, const V& b)
{
    { C::eq(a, b) } -> same_as<bool>;
};

template<equality_comparable T>
struct default_key_comparator
{
    constexpr static bool eq(const T& a, const T& b)
    {
        return a == b;
    }
};

template<
    is_object T,
    allocator Allocator = DefaultAllocator,
    key_hasher<T> KeyHasher = default_key_hasher<T>,
    key_comparator<T> KeyComparator = default_key_comparator<T>
>
requires moveable<T>
class hash_set
{
protected:
    static constexpr uint8_t kHasValue  = 0x80;
    static constexpr uint8_t kHashMask  = 0x7f;
    static constexpr uint8_t kEmpty     = 0x00;
    static constexpr uint8_t kTombstone = 0x01;

    static constexpr isize_t kMinCapacity = 8;

    // Important so we can memzero the tags
    static_assert(kEmpty == 0);

    uint8_t*         m_tags{};
    maybe_uninit<T>* m_values{};
    isize_t          m_capacity{};
    isize_t          m_size{};

    ASL_NO_UNIQUE_ADDRESS Allocator m_allocator;

    [[nodiscard]] constexpr isize_t max_size() const
    {
        // Max load factor is 75%
        return (m_capacity >> 1) + (m_capacity >> 2); // NOLINT(*-signed-bitwise)
    }

    static isize_t size_to_capacity(isize_t size)
    {
        ASL_ASSERT(size > 0);
        return max<isize_t>(
            kMinCapacity,
            static_cast<isize_t>(bit_ceil((static_cast<uint64_t>(size) * 4 + 2) / 3)));
    }

    static void insert_inner(
        T&& value,
        uint8_t* tags,
        maybe_uninit<T>* values,
        isize_t capacity,
        isize_t* size)
    {
        ASL_ASSERT(*size < capacity);

        const auto result = find_slot_insert(value, tags, values, capacity);

        // NOLINTBEGIN(*-pointer-arithmetic)

        ASL_ASSERT(result.first_available_index >= 0);

        if (result.already_present_index != result.first_available_index)
        {
            ASL_ASSERT((tags[result.first_available_index] & kHasValue) == 0);
            if (result.already_present_index >= 0)
            {
                ASL_ASSERT((tags[result.already_present_index] & kHasValue) != 0);
                values[result.already_present_index].destroy_unsafe();
                tags[result.already_present_index] = kTombstone;
            }
            else
            {
                *size += 1;
            }

            values[result.first_available_index].construct_unsafe(std::move(value));
            tags[result.first_available_index] = result.tag;
        }

        // NOLINTEND(*-pointer-arithmetic)
    }

    void grow_and_rehash()
    {
        grow_and_rehash(max(kMinCapacity, m_capacity * 2));
    }

    void grow_and_rehash(isize_t new_capacity)
    {
        ASL_ASSERT(new_capacity >= kMinCapacity && is_pow2(new_capacity) && new_capacity > m_capacity);

        auto* new_tags = static_cast<uint8_t*>(m_allocator.alloc(layout::array<uint8_t>(new_capacity)));
        auto* new_values = static_cast<maybe_uninit<T>*>(m_allocator.alloc(layout::array<maybe_uninit<T>>(new_capacity)));
        asl::memzero(new_tags, new_capacity);

        isize_t new_size = 0;

        if (m_size > 0)
        {
            // NOLINTBEGIN(*-pointer-arithmetic)
            for (isize_t i = 0; i < m_capacity; ++i)
            {
                if ((m_tags[i] & kHasValue) == 0) { continue; }

                insert_inner(std::move(m_values[i].as_init_unsafe()), new_tags, new_values, new_capacity, &new_size);

                // Destroy now so that destroy() has less things to do
                m_values[i].destroy_unsafe();
                m_tags[i] = kTombstone;
            }
            // NOLINTEND(*-pointer-arithmetic)
        }

        ASL_ASSERT(new_size == m_size);

        m_size = 0;
        destroy();

        m_tags = new_tags;
        m_values = new_values;
        m_capacity = new_capacity;
        m_size = new_size;
    }

    void clear_values()
    {
        if constexpr (!trivially_destructible<T>)
        {
            if (m_size > 0)
            {
                for (isize_t i = 0; i < m_capacity; ++i)
                {
                    if ((m_tags[i] & kHasValue) != 0) // NOLINT(*-pointer-arithmetic)
                    {
                        m_values[i].destroy_unsafe(); // NOLINT(*-pointer-arithmetic)
                    }
                }
            }
        }
    }

    void copy_from(const hash_set& other)
    {
        if (other.size() > 0)
        {
            isize_t min_capacity = size_to_capacity(other.size());
            if (m_capacity < min_capacity)
            {
                grow_and_rehash(min_capacity);
            }
            ASL_ASSERT(m_capacity >= min_capacity);

            for (isize_t i = 0; i < other.m_capacity; ++i)
            {
                if ((other.m_tags[i] & kHasValue) != 0) // NOLINT(*-pointer-arithmetic)
                {
                    insert(other.m_values[i].as_init_unsafe()); // NOLINT(*-pointer-arithmetic)
                }
            }
        }
    }

    struct FindSlotResult
    {
        uint8_t tag{};
        isize_t first_available_index = -1;
        isize_t already_present_index = -1;
    };

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    static FindSlotResult find_slot_insert(
        const U& value,
        const uint8_t* tags,
        const maybe_uninit<T>* values,
        isize_t capacity)
    {
        ASL_ASSERT(is_pow2(capacity));

        FindSlotResult result{};

        const isize_t capacity_mask = capacity - 1;
        const uint64_t hash = KeyHasher::hash(value);
        const auto starting_index = static_cast<isize_t>(hash >> 7) & capacity_mask;

        result.tag = static_cast<uint8_t>(hash & kHashMask) | kHasValue;

        // NOLINTBEGIN(*-pointer-arithmetic)
        for (
            isize_t i = starting_index;
            i != starting_index || result.first_available_index < 0;
            i = (i + 1) & capacity_mask)
        {
            uint8_t t = tags[i];

            if ((t & kHasValue) == 0 && result.first_available_index < 0)
            {
                result.first_available_index = i;
            }

            if (t == result.tag && KeyComparator::eq(values[i].as_init_unsafe(), value))
            {
                ASL_ASSERT(result.already_present_index < 0);
                result.already_present_index = i;
                if (result.first_available_index < 0)
                {
                    result.first_available_index = i;
                }
                break;
            }

            if (t == kEmpty) { break; }
        }

        // NOLINTEND(*-pointer-arithmetic)

        return result;
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    isize_t find_slot_lookup(const U& value) const
    {
        if (m_size <= 0) { return -1; };

        ASL_ASSERT(is_pow2(m_capacity));

        const isize_t capacity_mask = m_capacity - 1;
        const uint64_t hash = KeyHasher::hash(value);
        const uint8_t tag = static_cast<uint8_t>(hash & kHashMask) | kHasValue;
        const auto starting_index = static_cast<isize_t>(hash >> 7) & capacity_mask;

        // NOLINTBEGIN(*-pointer-arithmetic)

        isize_t i = starting_index;
        do
        {
            const uint8_t t = m_tags[i];

            if (t == tag && KeyComparator::eq(m_values[i].as_init_unsafe(), value)) { return i; }
            if (t == kEmpty) { break; }

            i = (i + 1) & capacity_mask;
        } while (i != starting_index);

        // NOLINTEND(*-pointer-arithmetic)

        return -1;
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    FindSlotResult find_slot_insert(const U& value)
    {
        return find_slot_insert(value, m_tags, m_values, m_capacity);
    }

    void maybe_grow_to_fit_one_more()
    {
        if (m_size >= max_size())
        {
            grow_and_rehash();
        }
    }

public:
    constexpr hash_set() requires default_constructible<Allocator>
        : m_allocator{}
    {}

    explicit constexpr hash_set(Allocator allocator)
        : m_allocator{std::move(allocator)}
    {}

    hash_set(const hash_set& other)
        requires copy_constructible<Allocator> && copyable<T>
        : hash_set{other.m_allocator}
    {
        copy_from(other);
    }

    hash_set& operator=(const hash_set& other)
        requires copyable<T>
    {
        if (&other != this)
        {
            clear();
            copy_from(other);
        }
        return *this;
    }

    hash_set(hash_set&& other)
        requires move_constructible<Allocator>
        : m_tags{exchange(other.m_tags, nullptr)}
        , m_values{exchange(other.m_values, nullptr)}
        , m_capacity{exchange(other.m_capacity, 0)}
        , m_size{exchange(other.m_size, 0)}
        , m_allocator{std::move(other.m_allocator)}
    {}

    hash_set& operator=(hash_set&& other)
    {
        if (&other != this)
        {
            destroy();
            m_tags = exchange(other.m_tags, nullptr);
            m_values = exchange(other.m_values, nullptr);
            m_capacity = exchange(other.m_capacity, 0);
            m_size = exchange(other.m_size, 0);
            m_allocator = std::move(other.m_allocator);
        }
        return *this;
    }

    ~hash_set()
    {
        destroy();
    }

    void destroy()
    {
        clear_values();
        m_size = 0;

        if (m_capacity > 0)
        {
            m_allocator.dealloc(m_tags, layout::array<uint8_t>(m_capacity));
            m_allocator.dealloc(m_values, layout::array<maybe_uninit<T>>(m_capacity));
            m_capacity = 0;
        }
    }

    void clear()
    {
        clear_values();
        m_size = 0;

        if (m_capacity > 0)
        {
            asl::memzero(m_tags, m_capacity);
        }
    }

    [[nodiscard]] constexpr isize_t size() const { return m_size; }

    [[nodiscard]] constexpr bool is_empty() const { return m_size == 0; }

    template<typename... Args>
    void insert(Args&&... args)
        requires constructible_from<T, Args&&...>
    {
        maybe_grow_to_fit_one_more();
        ASL_ASSERT(m_size < max_size());
        insert_inner(T{std::forward<Args>(args)...}, m_tags, m_values, m_capacity, &m_size);
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    bool contains(const U& value) const
    {
        return find_slot_lookup(value) >= 0;
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    bool remove(const U& value)
    {
        isize_t slot = find_slot_lookup(value);
        if (slot < 0) { return false; }

        m_values[slot].destroy_unsafe(); // NOLINT(*-pointer-arithmetic)
        m_tags[slot] = kTombstone; // NOLINT(*-pointer-arithmetic)
        m_size -= 1;

        return true;
    }
};

} // namespace asl

