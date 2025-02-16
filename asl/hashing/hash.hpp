#pragma once

#include "asl/base/integers.hpp"
#include "asl/base/meta.hpp"
#include "asl/types/span.hpp"
#include "asl/base/utility.hpp"

namespace asl::city_hash
{

// Hash function for a byte array.
uint64_t CityHash64(const char *s, size_t len);

// Hash function for a byte array.  For convenience, a 64-bit seed is also
// hashed into the result.
uint64_t CityHash64WithSeed(const char *s, size_t len, uint64_t seed);

// Hash function for a byte array.  For convenience, two seeds are also
// hashed into the result.
uint64_t CityHash64WithSeeds(const char *s, size_t len,
                           uint64_t seed0, uint64_t seed1);

// Hash function for a byte array.
uint128_t CityHash128(const char *s, size_t len);

// Hash function for a byte array.  For convenience, a 128-bit seed is also
// hashed into the result.
uint128_t CityHash128WithSeed(const char *s, size_t len, uint128_t seed);

// Hash function for a byte array.  Most useful in 32-bit binaries.
uint32_t CityHash32(const char *s, size_t len);

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
constexpr uint64_t Hash128to64(uint64_t high, uint64_t low)
{
  // Murmur-inspired hashing.
  const uint64_t kMul = 0x9ddfea08eb382d69ULL;
  uint64_t a = (low ^ high) * kMul;
  a ^= (a >> 47);
  uint64_t b = (high ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
constexpr uint64_t Hash128to64(const uint128_t& x)
{
  return Hash128to64(x.high, x.low);
}

} // namespace asl::city_hash

namespace asl
{

template<typename T, typename H>
concept hashable_generic = requires(const T& value, H h)
{
    { AslHashValue(h, value) } -> same_as<H>;
};

struct HashState
{
    uint128_t state{};

    constexpr HashState() = default;
    explicit constexpr HashState(uint128_t s) : state{s} {}

    template<typename T>
    static HashState combine_contiguous(HashState h, span<const T> s)
    {
        if constexpr (uniquely_represented<T>)
        {
            auto bytes = as_bytes(s);
            auto hashed = city_hash::CityHash128WithSeed(
                reinterpret_cast<const char*>(bytes.data()),
                static_cast<size_t>(bytes.size()),
                h.state);
            return HashState{hashed};
        }
        else
        {
            for (const auto& value: s)
            {
                h = AslHashValue(ASL_MOVE(h), value);
            }
            return h;
        }
    }

    static constexpr HashState combine(HashState h)
    {
        return h;
    }

    template<hashable_generic<HashState> Arg, hashable_generic<HashState>... Remaining>
    static constexpr HashState combine(HashState h, const Arg& arg, const Remaining&... remaining)
    {
        return combine(AslHashValue(ASL_MOVE(h), arg), remaining...);
    }
};

template<typename T>
concept hashable = hashable_generic<T, HashState>;

template<typename H, uniquely_represented T>
constexpr H AslHashValue(H h, const T& value)
{
    return H::combine_contiguous(ASL_MOVE(h), span<const T>{&value, 1});
}

template<typename H>
constexpr H AslHashValue(H h, bool value)
{
    return AslHashValue(ASL_MOVE(h), value ? 1 : 0);
}

template<typename H, typename T>
constexpr void AslHashValue(H h, T*); // Don't hash pointers

template<typename H, hashable T>
constexpr H AslHashValue(H h, const span<T>& s)
{
    return H::combine_contiguous(ASL_MOVE(h), span<const T>{s.data(), s.size()});
}

template<hashable T>
constexpr uint64_t hash_value(const T& value)
{
    auto result = AslHashValue(HashState{}, value).state;
    return city_hash::Hash128to64(result);
}

} // namespace asl

