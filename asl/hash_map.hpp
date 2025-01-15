#pragma once

#include "asl/annotations.hpp"
#include "asl/meta.hpp"
#include "asl/utility.hpp"
#include "asl/maybe_uninit.hpp"
#include "asl/hash.hpp"
#include "asl/allocator.hpp"
#include "asl/memory.hpp"
#include "asl/hash_set.hpp"

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
class hash_map : hash_set<
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

    // @Todo insert
    // @Todo contains
    // @Todo remove
    // @Todo get
    // @Todo tests
};

} // namespace asl
