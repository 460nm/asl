// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/utility.hpp"
#include "asl/testing/testing.hpp"

template<typename T> static constexpr int identify(const T&)  { return 1; }
template<typename T> static constexpr int identify(const T&&) { return 2; }
template<typename T> static constexpr int identify(T&)        { return 3; }
template<typename T> static constexpr int identify(T&&)       { return 4; }

struct IdentifySelf
{
    constexpr int get(this auto&& self) { return identify(std::forward<decltype(self)>(self)); }
};

static int get_const_lref(const IdentifySelf& i)  { return i.get(); }
static int get_const_rref(const IdentifySelf&& i) { return std::move(i).get(); } // NOLINT
static int get_lref(IdentifySelf& i)              { return i.get(); }
static int get_rref(IdentifySelf&& i)             { return std::move(i).get(); } // NOLINT

ASL_TEST(forward)
{
    IdentifySelf id{};
    ASL_TEST_EXPECT(get_const_lref(IdentifySelf{}) == 1);
    ASL_TEST_EXPECT(get_lref(id) == 3);
    ASL_TEST_EXPECT(get_const_rref(IdentifySelf{}) == 2);
    ASL_TEST_EXPECT(get_rref(IdentifySelf{}) == 4);
}

ASL_TEST(move)
{
    IdentifySelf id;
    ASL_TEST_EXPECT(id.get() == 3);
    ASL_TEST_EXPECT(IdentifySelf{}.get() == 4);
}

struct Level1
{
    IdentifySelf id;
};

struct Level2
{
    Level1 deeper;
};

struct Level3
{
    Level2 deeper;
};

static int get_const_lref(const Level3& i)  { return i.deeper.deeper.id.get(); }
static int get_const_rref(const Level3&& i) { return std::move(i).deeper.deeper.id.get(); }
static int get_lref(Level3& i)              { return i.deeper.deeper.id.get(); }
static int get_rref(Level3&& i)             { return std::move(i).deeper.deeper.id.get(); }

ASL_TEST(forward2)
{
    Level3 id{};
    ASL_TEST_EXPECT(get_const_lref(id) == 1);
    ASL_TEST_EXPECT(get_lref(id) == 3);
    ASL_TEST_EXPECT(get_const_rref(Level3{}) == 2);
    ASL_TEST_EXPECT(get_rref(Level3{}) == 4);
}

template<typename T>
static int test_fwd_like(T) // NOLINT
{
    const IdentifySelf id;
    return std::forward_like<T>(id).get();
}

ASL_TEST(forward_like)
{
    int x{};
    ASL_TEST_EXPECT(test_fwd_like<const int&>(7) == 1);
    ASL_TEST_EXPECT(test_fwd_like<int&>(x) == 3);
    ASL_TEST_EXPECT(test_fwd_like<const int&&>(8) == 2);
    ASL_TEST_EXPECT(test_fwd_like<int&&>(9) == 4);
}

enum class Enum : int // NOLINT
{
    kOne = 1,
    kTwo = 2,
};

enum Enum2 : int // NOLINT
{
    kOne = 1,
    kTwo = 2,
};

static_assert(asl::to_underlying(Enum::kOne) == 1);
static_assert(asl::to_underlying(Enum::kTwo) == 2);
static_assert(asl::to_underlying(kOne) == 1);
static_assert(asl::to_underlying(kTwo) == 2);

