#include "asl/base/utility.hpp"
#include "asl/testing/testing.hpp"

template<typename T> static constexpr int identify(const T&)  { return 1; }
template<typename T> static constexpr int identify(const T&&) { return 2; }
template<typename T> static constexpr int identify(T&)        { return 3; }
template<typename T> static constexpr int identify(T&&)       { return 4; }

struct IdentifySelf
{
    constexpr int get(this auto&& self) { return identify(ASL_FWD(self)); }
};

static int get_const_lref(const IdentifySelf& i)  { return ASL_FWD(i).get(); }
static int get_const_rref(const IdentifySelf&& i) { return ASL_FWD(i).get(); }
static int get_lref(IdentifySelf& i)              { return ASL_FWD(i).get(); }
static int get_rref(IdentifySelf&& i)             { return ASL_FWD(i).get(); }

ASL_TEST(forward)
{
    IdentifySelf id;
    ASL_TEST_EXPECT(get_const_lref(id) == 1);
    ASL_TEST_EXPECT(get_lref(id) == 3);
    ASL_TEST_EXPECT(get_const_rref(ASL_MOVE(id)) == 2);
    ASL_TEST_EXPECT(get_rref(ASL_MOVE(id)) == 4);
}

ASL_TEST(move)
{
    IdentifySelf id;
    ASL_TEST_EXPECT(id.get() == 3);
    ASL_TEST_EXPECT(ASL_MOVE(id).get() == 4);
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

static int get_const_lref(const Level3& i)  { return ASL_FWD(i).deeper.deeper.id.get(); }
static int get_const_rref(const Level3&& i) { return ASL_FWD(i).deeper.deeper.id.get(); }
static int get_lref(Level3& i)              { return ASL_FWD(i).deeper.deeper.id.get(); }
static int get_rref(Level3&& i)             { return ASL_FWD(i).deeper.deeper.id.get(); }

ASL_TEST(forward2)
{
    Level3 id{};
    ASL_TEST_EXPECT(get_const_lref(id) == 1);
    ASL_TEST_EXPECT(get_lref(id) == 3);
    ASL_TEST_EXPECT(get_const_rref(ASL_MOVE(id)) == 2);
    ASL_TEST_EXPECT(get_rref(ASL_MOVE(id)) == 4);
}

template<typename T>
static int test_fwd_like(T&& t)
{
    IdentifySelf id;
    return ASL_FWD_LIKE(decltype(t), id).get();
}

ASL_TEST(forward_like)
{
    int x{};
    ASL_TEST_EXPECT(test_fwd_like<const int&>(x) == 1);
    ASL_TEST_EXPECT(test_fwd_like<int&>(x) == 3);
    ASL_TEST_EXPECT(test_fwd_like<const int&&>(ASL_MOVE(x)) == 2);
    ASL_TEST_EXPECT(test_fwd_like<int&&>(ASL_MOVE(x)) == 4);
}

