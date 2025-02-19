#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/assert.hpp"

namespace asl
{

template<typename T>
struct intrusive_list_node
{
    T* prev{};
    T* next{};
};

template<typename T>
concept is_intrusive_list_node = convertible_from<intrusive_list_node<T>*, T*>;


template<is_intrusive_list_node T>
class IntrusiveList
{
    struct sentinel: public intrusive_list_node<T> {};

    sentinel m_sentinel{};

    T* sentinel() { return reinterpret_cast<T*>(&m_sentinel); }
    const T* sentinel() const { return reinterpret_cast<const T*>(&m_sentinel); }

    constexpr T* head_inner() const { return m_sentinel.next; }

    constexpr T* tail_inner() const { return m_sentinel.prev; }

    void insert_after(T* before, T* after)
    {
        after->prev = before;
        after->next = before->next;

        before->next = after;
        after->next->prev = after;
    }

public:
    constexpr IntrusiveList() : m_sentinel{ sentinel(), sentinel() } {}

    constexpr bool is_empty() const { return head_inner() == sentinel(); }

    void push_front(T* node)
    {
        ASL_ASSERT(node->next == nullptr && node->prev == nullptr);
        insert_after(head_inner()->prev, node);
    }

    void push_back(T* node)
    {
        ASL_ASSERT(node->next == nullptr && node->prev == nullptr);
        insert_after(tail_inner(), node);
    }

    T* head() const
    {
        T* h = head_inner();
        return (h == sentinel()) ? nullptr : h;
    }

    T* tail() const
    {
        T* t = tail_inner();
        return (t == sentinel()) ? nullptr : t;
    }

    void detach(T* node)
    {
        ASL_ASSERT(node->next != nullptr && node->prev != nullptr);

        node->prev->next = node->next;
        node->next->prev = node->prev;

        node->next = nullptr;
        node->prev = nullptr;
    }

    T* pop_front()
    {
        if (T* h = head_inner(); h != sentinel())
        {
            detach(h);
            return h;
        }
        return nullptr;
    }

    T* pop_back()
    {
        if (T* t = tail_inner(); t != sentinel())
        {
            detach(t);
            return t;
        }
        return nullptr;
    }

    template<typename TT>
    struct generic_iterator
    {
        TT* m_node;

    public:
        constexpr explicit generic_iterator(TT* node) : m_node{node} {}

        constexpr bool operator==(const generic_iterator& other) const = default;

        constexpr generic_iterator& operator++()
        {
            m_node = m_node->next;
            return *this;
        }

        constexpr generic_iterator operator++(int)
        {
            return iterator{ exchange(m_node, m_node->next) };
        }

        constexpr TT& operator*() const { return *m_node; }

        constexpr TT* operator->() const { return m_node; }
    };

    using iterator = generic_iterator<T>;
    using const_iterator = generic_iterator<const T>;

    // @Todo(C++23) Deduplicate with deducing-this maybe
    const_iterator begin() const { return const_iterator{ head_inner() }; }
    const_iterator end() const { return const_iterator{ sentinel() }; }

    iterator begin() { return iterator{ head_inner() }; }
    iterator end() { return iterator{ sentinel() }; }
};

}

