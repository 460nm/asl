#include "asl/containers/intrusive_list.hpp"
#include "asl/testing/testing.hpp"

struct IntNode : public asl::intrusive_list_node<IntNode>
{
    int value;

    explicit IntNode(int v)
        : asl::intrusive_list_node<IntNode>{}
        , value{v}
    {}
};

ASL_TEST(empty_list)
{
    const asl::IntrusiveList<IntNode> list;
    ASL_TEST_EXPECT(list.is_empty());
    ASL_TEST_EXPECT(list.head() == nullptr);
    ASL_TEST_EXPECT(list.tail() == nullptr);
}

ASL_TEST(push_front)
{
    IntNode one{1};
    IntNode two{2};
    IntNode three{3};
    asl::IntrusiveList<IntNode> list;

    list.push_front(&one);
    ASL_TEST_EXPECT(!list.is_empty());
    ASL_TEST_EXPECT(list.head() == &one);
    ASL_TEST_EXPECT(list.tail() == &one);

    auto it = list.begin();
    auto end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it == end);

    list.push_front(&two);
    ASL_TEST_EXPECT(list.head() == &two);
    ASL_TEST_EXPECT(list.tail() == &one);
    it = list.begin();
    end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 2);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it == end);

    list.push_front(&three);
    ASL_TEST_EXPECT(list.head() == &three);
    ASL_TEST_EXPECT(list.tail() == &one);
    it = list.begin();
    end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 3);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 2);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it == end);
}

ASL_TEST(push_back)
{
    IntNode one{1};
    IntNode two{2};
    IntNode three{3};
    asl::IntrusiveList<IntNode> list;

    list.push_back(&one);
    ASL_TEST_EXPECT(!list.is_empty());
    ASL_TEST_EXPECT(list.head() == &one);
    ASL_TEST_EXPECT(list.tail() == &one);

    auto it = list.begin();
    auto end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it == end);

    list.push_back(&two);
    ASL_TEST_EXPECT(list.head() == &one);
    ASL_TEST_EXPECT(list.tail() == &two);
    it = list.begin();
    end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 2);
    it++;
    ASL_TEST_ASSERT(it == end);

    list.push_back(&three);
    ASL_TEST_EXPECT(list.head() == &one);
    ASL_TEST_EXPECT(list.tail() == &three);
    it = list.begin();
    end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 2);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 3);
    it++;
    ASL_TEST_ASSERT(it == end);
}

ASL_TEST(detach)
{
    IntNode one{1};
    IntNode two{2};
    IntNode three{3};
    asl::IntrusiveList<IntNode> list;

    list.push_back(&one);
    list.push_back(&two);
    list.push_back(&three);

    list.detach(&two);
    auto it = list.begin();
    auto end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 3);
    it++;
    ASL_TEST_ASSERT(it == end);

    list.detach(&three);
    it = list.begin();
    end = list.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(it->value == 1);
    it++;
    ASL_TEST_ASSERT(it == end);

    list.detach(&one);
    it = list.begin();
    end = list.end();

    ASL_TEST_ASSERT(list.is_empty());
    ASL_TEST_ASSERT(it == end);
}

ASL_TEST(pop_front)
{
    IntNode one{1};
    IntNode two{2};
    IntNode three{3};
    asl::IntrusiveList<IntNode> list;

    list.push_back(&one);
    list.push_back(&two);
    list.push_back(&three);

    IntNode* n = list.pop_front();
    ASL_TEST_ASSERT(n != nullptr);
    ASL_TEST_ASSERT(!list.is_empty());
    ASL_TEST_EXPECT(n->value == 1);

    n = list.pop_front();
    ASL_TEST_ASSERT(n != nullptr);
    ASL_TEST_ASSERT(!list.is_empty());
    ASL_TEST_EXPECT(n->value == 2);

    n = list.pop_front();
    ASL_TEST_ASSERT(n != nullptr);
    ASL_TEST_ASSERT(list.is_empty());
    ASL_TEST_EXPECT(n->value == 3);

    n = list.pop_front();
    ASL_TEST_ASSERT(n == nullptr);
    ASL_TEST_ASSERT(list.is_empty());
}

ASL_TEST(pop_back)
{
    IntNode one{1};
    IntNode two{2};
    IntNode three{3};
    asl::IntrusiveList<IntNode> list;

    list.push_back(&one);
    list.push_back(&two);
    list.push_back(&three);

    IntNode* n = list.pop_back();
    ASL_TEST_ASSERT(n != nullptr);
    ASL_TEST_ASSERT(!list.is_empty());
    ASL_TEST_EXPECT(n->value == 3);

    n = list.pop_back();
    ASL_TEST_ASSERT(n != nullptr);
    ASL_TEST_ASSERT(!list.is_empty());
    ASL_TEST_EXPECT(n->value == 2);

    n = list.pop_back();
    ASL_TEST_ASSERT(n != nullptr);
    ASL_TEST_ASSERT(list.is_empty());
    ASL_TEST_EXPECT(n->value == 1);

    n = list.pop_back();
    ASL_TEST_ASSERT(n == nullptr);
    ASL_TEST_ASSERT(list.is_empty());
}
