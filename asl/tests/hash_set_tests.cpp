#include "asl/hash_set.hpp"
#include "asl/testing/testing.hpp"
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

// @Todo Remove elements

// @Todo Test destructors

