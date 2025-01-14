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

    template<typename H>
    friend H AslHashValue(H h, const HashWithDestructor& value)
    {
        return H::combine(ASL_MOVE(h), value.x);
    }
};

ASL_TEST(destructor_and_remove)
{
    static constexpr int kCount = 200;
    bool destroyed[kCount]{};

    {
        asl::hash_set<HashWithDestructor> set;

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
            // @Todo Remove with something comparable
            ASL_TEST_EXPECT(set.remove(HashWithDestructor{i, nullptr}));
        }

        for (int i = 0; i < kCount; i += 2)
        {
            ASL_TEST_EXPECT(!set.contains(HashWithDestructor{i, nullptr}));
            ASL_TEST_EXPECT(set.contains(HashWithDestructor{i+1, nullptr}));
            ASL_TEST_EXPECT(destroyed[i]); // NOLINT
            ASL_TEST_EXPECT(!destroyed[i + 1]); // NOLINT
        }
    }

    for (int i = 0; i < kCount; ++i)
    {
        ASL_TEST_EXPECT(destroyed[i]); // NOLINT
    }
}

