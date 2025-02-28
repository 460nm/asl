// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/utility.hpp"
#include "asl/base/meta.hpp"
#include "asl/hashing/hash.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/containers/hash_set.hpp"

namespace asl
{

namespace hash_map_internal
{

template<typename K, typename V>
struct Slot
{
    K key;
    V value;
};

template<hashable K, typename V, key_hasher<K> KeyHasher>
struct SlotHasher : public KeyHasher
{
    using KeyHasher::hash;

    constexpr static uint64_t hash(const Slot<K, V>& slot)
    {
        return KeyHasher::hash(slot.key);
    }
};

template<equality_comparable K, typename V, key_comparator<K> KeyComparator>
struct SlotComparator : public KeyComparator
{
    using KeyComparator::eq;

    constexpr static bool eq(const Slot<K, V>& a, const Slot<K, V>& b)
    {
        return KeyComparator::eq(a.key, b.key);
    }

    constexpr static bool eq(const Slot<K, V>& a, const K& b)
    {
        return KeyComparator::eq(a.key, b);
    }
};

} // namespace hash_map_internal

template<
    is_object K,
    is_object V,
    allocator Allocator = DefaultAllocator,
    key_hasher<K> KeyHasher = default_key_hasher<K>,
    key_comparator<K> KeyComparator = default_key_comparator<K>
>
requires moveable<K> && moveable<V>
class hash_map : protected hash_set<
    hash_map_internal::Slot<K, V>,
    Allocator,
    hash_map_internal::SlotHasher<K, V, KeyHasher>,
    hash_map_internal::SlotComparator<K, V, KeyComparator>>
{
    using Base =
        hash_set<
            hash_map_internal::Slot<K, V>,
            Allocator,
            hash_map_internal::SlotHasher<K, V, KeyHasher>,
            hash_map_internal::SlotComparator<K, V, KeyComparator>>;

public:
    constexpr hash_map() requires default_constructible<Allocator> = default;

    explicit constexpr hash_map(Allocator allocator)
        : Base{ASL_MOVE(allocator)}
    {}

    hash_map(const hash_map&) requires copyable<K> && copyable<V> = default;

    hash_map& operator=(const hash_map&) requires copyable<K> && copyable<V> = default;

    hash_map(hash_map&&) = default;

    hash_map& operator=(hash_map&&) = default;

    ~hash_map() = default;

    using Base::destroy;

    using Base::clear;

    using Base::size;

    using Base::remove;

    using Base::contains;

    template<typename U, typename Arg0, typename... Args1>
    requires
        key_hasher<KeyHasher, U> &&
        key_comparator<KeyComparator, K, U> &&
        constructible_from<K, U&&> &&
        constructible_from<V, Arg0&&, Args1&&...>
    void insert(U&& key, Arg0&& arg0, Args1&&... args1)
    {
        Base::maybe_grow_to_fit_one_more();

        auto result = Base::find_slot_insert(key);

        // NOLINTBEGIN(*-pointer-arithmetic)

        ASL_ASSERT(result.first_available_index >= 0);

        if (result.already_present_index >= 0)
        {
            if (result.already_present_index != result.first_available_index)
            {
                ASL_ASSERT((Base::m_tags[result.first_available_index] & Base::kHasValue) == 0);

                Base::m_values[result.first_available_index].construct_unsafe(ASL_MOVE(Base::m_values[result.already_present_index].as_init_unsafe()));
                Base::m_values[result.already_present_index].destroy_unsafe();

                Base::m_tags[result.first_available_index] = result.tag;
                Base::m_tags[result.already_present_index] = Base::kTombstone;
            }

            ASL_ASSERT(Base::m_tags[result.first_available_index] == result.tag);

            if constexpr (sizeof...(Args1) == 0 && assignable_from<V&, Arg0&&>)
            {
                Base::m_values[result.first_available_index].as_init_unsafe().value = ASL_FWD(arg0);
            }
            else
            {
                Base::m_values[result.first_available_index].as_init_unsafe().value = ASL_MOVE(V{ASL_FWD(arg0), ASL_FWD(args1)...});
            }
        }
        else
        {
            ASL_ASSERT((Base::m_tags[result.first_available_index] & Base::kHasValue) == 0);
            Base::m_values[result.first_available_index].construct_unsafe(ASL_FWD(key), V{ASL_FWD(arg0), ASL_FWD(args1)...});
            Base::m_tags[result.first_available_index] = result.tag;
            Base::m_size += 1;
        }

        // NOLINTEND(*-pointer-arithmetic)
    }

    template<typename U>
    auto get(this auto&& self, const U& value)
        requires key_hasher<KeyHasher, U> && key_comparator<KeyComparator, K, U>
    {
        using return_type = un_ref_t<copy_cref_t<decltype(self), V>>*;
        isize_t index = self.find_slot_lookup(value);
        if (index >= 0)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            return return_type{ &self.m_values[index].as_init_unsafe().value };
        }
        return return_type{ nullptr };
    }
};

} // namespace asl
