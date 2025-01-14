#pragma once

#include "asl/annotations.hpp"
#include "asl/meta.hpp"
#include "asl/utility.hpp"
#include "asl/maybe_uninit.hpp"
#include "asl/hash.hpp"
#include "asl/allocator.hpp"
#include "asl/memory.hpp"

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
requires move_constructible<T> && move_assignable<T>
class hash_set
{
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

    constexpr isize_t max_size() const
    {
        // Max load factor is 75%
        return (m_capacity >> 1) + (m_capacity >> 2);
    }

    static void insert_inner(
        T&& value,
        uint8_t* tags,
        maybe_uninit<T>* values,
        isize_t capacity,
        isize_t* size)
    {
        ASL_ASSERT(*size < capacity);
        ASL_ASSERT(is_pow2(capacity));

        const isize_t capacity_mask = capacity - 1;
        const uint64_t hash = KeyHasher::hash(value);
        const uint8_t tag = static_cast<uint8_t>(hash & kHashMask) | kHasValue;
        const auto starting_index = static_cast<isize_t>(hash >> 7) & capacity_mask;

        isize_t first_available_index = -1;
        isize_t already_present_index = -1;
        
        // NOLINTBEGIN(*-pointer-arithmetic)

        for (
            isize_t i = starting_index;
            i != starting_index || first_available_index < 0;
            i = (i + 1) & capacity_mask)
        {
            uint8_t t = tags[i];
            
            if ((t & kHasValue) == 0 && first_available_index < 0)
            {
                first_available_index = i;
            }

            if (t == tag && KeyComparator::eq(values[i].as_init_unsafe(), value))
            {
                ASL_ASSERT(already_present_index < 0);
                already_present_index = i;
                if (first_available_index < 0)
                {
                    first_available_index = i;
                }
                break;
            }

            if (t == kEmpty) { break; }
        }

        ASL_ASSERT(first_available_index >= 0 || already_present_index >= 0);

        if (already_present_index == first_available_index)
        {
            ASL_ASSERT((tags[already_present_index] & kHasValue) != 0);
            values[already_present_index].assign_unsafe(ASL_MOVE(value));
        }
        else
        {
            ASL_ASSERT((tags[first_available_index] & kHasValue) == 0);
            if (already_present_index >= 0)
            {
                ASL_ASSERT((tags[already_present_index] & kHasValue) != 0);
                values[already_present_index].destroy_unsafe();
                tags[already_present_index] = kTombstone;
            }
            else
            {
                *size += 1;
            }

            values[first_available_index].construct_unsafe(ASL_MOVE(value));
            tags[first_available_index] = tag;
        }
        
        // NOLINTEND(*-pointer-arithmetic)
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    isize_t find_slot(const U& value) const
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

    void grow_and_rehash()
    {
        isize_t new_capacity = max(kMinCapacity, m_capacity * 2);
        
        auto* new_tags = reinterpret_cast<uint8_t*>(m_allocator.alloc(layout::array<uint8_t>(new_capacity)));
        auto* new_values = reinterpret_cast<maybe_uninit<T>*>(m_allocator.alloc(layout::array<maybe_uninit<T>>(new_capacity)));
        asl::memzero(new_tags, new_capacity);

        isize_t new_size = 0;

        // NOLINTBEGIN(*-pointer-arithmetic)
        for (isize_t i = 0; i < m_capacity; ++i)
        {
            if ((m_tags[i] & kHasValue) == 0) { continue; }

            insert_inner(ASL_MOVE(m_values[i].as_init_unsafe()), new_tags, new_values, new_capacity, &new_size);

            // Destroy now so that destroy() has less things to do
            m_values[i].destroy_unsafe();
            m_tags[i] = kTombstone;
        }
        // NOLINTEND(*-pointer-arithmetic)

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

public:
    constexpr hash_set() requires default_constructible<Allocator>
        : m_allocator{}
    {}

    explicit constexpr hash_set(Allocator allocator)
        : m_allocator{ASL_MOVE(allocator)}
    {}

    // @Todo Copy, move

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

    constexpr isize_t size() const { return m_size; }

    template<typename... Args>
    void insert(Args&&... args)
        requires constructible_from<T, Args&&...>
    {
        if (m_size >= max_size())
        {
            grow_and_rehash();
        }
        ASL_ASSERT(m_size < max_size());
        insert_inner(ASL_MOVE(T{ASL_FWD(args)...}), m_tags, m_values, m_capacity, &m_size);
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    bool contains(const U& value) const
    {
        return find_slot(value) >= 0;
    }

    template<typename U>
    requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, T, U>
    bool remove(const U& value)
    {
        isize_t slot = find_slot(value);
        if (slot < 0) { return false; }

        m_values[slot].destroy_unsafe(); // NOLINT(*-pointer-arithmetic)
        m_tags[slot] = kTombstone; // NOLINT(*-pointer-arithmetic)
        m_size -= 1;

        return true;
    }
};

} // namespace asl

