// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/containers/hash_map.hpp"

// NOLINTNEXTLINE(*-complexity)
ASL_TEST(default)
{
    asl::hash_map<int, int> map;

    ASL_TEST_EXPECT(!map.contains(45));
    ASL_TEST_EXPECT(!map.contains(46));

    map.insert(45, 5);
    map.insert(46, 6);

    ASL_TEST_EXPECT(map.size() == 2);

    ASL_TEST_EXPECT(map.contains(45));
    ASL_TEST_EXPECT(map.contains(46));
    ASL_TEST_EXPECT(!map.contains(47));

    ASL_TEST_EXPECT(*map.get(45) == 5);
    ASL_TEST_EXPECT(*map.get(46) == 6);
    ASL_TEST_EXPECT(map.get(47) == nullptr);

    ASL_TEST_EXPECT(map.remove(45));
    ASL_TEST_EXPECT(!map.remove(45));

    ASL_TEST_EXPECT(map.size() == 1);

    ASL_TEST_EXPECT(!map.contains(45));
    ASL_TEST_EXPECT(map.contains(46));
    ASL_TEST_EXPECT(!map.contains(47));

    ASL_TEST_EXPECT(map.get(45) == nullptr);
    ASL_TEST_EXPECT(*map.get(46) == 6);
    ASL_TEST_EXPECT(map.get(47) == nullptr);

    map.insert(46, 460);

    ASL_TEST_EXPECT(map.size() == 1);

    ASL_TEST_EXPECT(!map.contains(45));
    ASL_TEST_EXPECT(map.contains(46));
    ASL_TEST_EXPECT(!map.contains(47));

    ASL_TEST_EXPECT(map.get(45) == nullptr);
    ASL_TEST_EXPECT(*map.get(46) == 460);
    ASL_TEST_EXPECT(map.get(47) == nullptr);
}
