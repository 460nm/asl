#include "asl/hash_set.hpp"
#include "asl/testing/testing.hpp"
#include "asl/tests/test_types.hpp"
#include "asl/string.hpp"
#include "asl/string_view.hpp"

ASL_TEST(empty)
{
    asl::hash_set<int> set;
    
    ASL_TEST_EXPECT(set.size() == 0);
    
    for (int i = 0; i < 100; ++i)
    {
        ASL_TEST_EXPECT(!set.contains(i));
    }
}

ASL_TEST(a_bunch_of_strings)
{
    asl::hash_set<asl::string<>> set;

    set.insert("Hello, world!");
    ASL_TEST_EXPECT(set.size() == 1);
    
    set.insert("Hello, puppy!");
    ASL_TEST_EXPECT(set.size() == 2);
    
    set.insert("Hello, puppy!");
    ASL_TEST_EXPECT(set.size() == 2);

    ASL_TEST_EXPECT(set.contains("Hello, world!"_sv));
    ASL_TEST_EXPECT(set.contains("Hello, puppy!"_sv));
    ASL_TEST_EXPECT(!set.contains("Hello, Steven!"_sv));
}

ASL_TEST(a_bunch_of_ints)
{
    asl::hash_set<int> set;

    int count = 3000;

    for (int i = 0; i < count; ++i)
    {
        set.insert(i);
    }

    ASL_TEST_EXPECT(set.size() == count);

    for (int i = 0; i < count * 2; ++i)
    {
        ASL_TEST_EXPECT(set.contains(i) == (i < count));
    }
}

struct HashWithDestructor: public DestructorObserver
{
    int x;

    HashWithDestructor(int x_, bool* ptr)
        : DestructorObserver{ptr}
        , x{x_}
    {}

    constexpr bool operator==(const HashWithDestructor& other) const
    {
        return x == other.x;
    }
};

struct CustomComparator
{
    static bool eq(const HashWithDestructor& a, const HashWithDestructor& b)
    {
        return a.x == b.x;
    }
    
    static bool eq(const HashWithDestructor& a, int b)
    {
        return a.x == b;
    }
};

struct CustomHasher
{
    static uint64_t hash(const HashWithDestructor& b)
    {
        return asl::hash_value(b.x);
    }
    
    static uint64_t hash(int x)
    {
        return asl::hash_value(x);
    }
};

ASL_TEST(destructor_and_remove)
{
    static constexpr int kCount = 200;
    bool destroyed[kCount]{};

    {
        asl::hash_set<HashWithDestructor, asl::DefaultAllocator, CustomHasher, CustomComparator> set;

        for (int i = 0; i < kCount; ++i)
        {
            set.insert(i, &destroyed[i]); // NOLINT
        }

        ASL_TEST_EXPECT(set.size() == kCount);

        for (int i = 0; i < kCount; ++i)
        {
            ASL_TEST_EXPECT(!destroyed[i]); // NOLINT
        }

        for (int i = 0; i < kCount; i += 2)
        {
            ASL_TEST_EXPECT(set.remove(i));
        }

        for (int i = 0; i < kCount; i += 2)
        {
            ASL_TEST_EXPECT(!set.contains(i));
            ASL_TEST_EXPECT(set.contains(i+1));
            ASL_TEST_EXPECT(destroyed[i]); // NOLINT
            ASL_TEST_EXPECT(!destroyed[i + 1]); // NOLINT
        }
    }

    for (int i = 0; i < kCount; ++i)
    {
        ASL_TEST_EXPECT(destroyed[i]); // NOLINT
    }
}

