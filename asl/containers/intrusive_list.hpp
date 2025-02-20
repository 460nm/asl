#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/utility.hpp"

namespace asl
{

template<typename T>
struct intrusive_list_node
{
    T* m_prev{};
    T* m_next{};
};

template<typename T>
concept is_intrusive_list_node = convertible_from<intrusive_list_node<T>*, T*>;

template<is_intrusive_list_node T>
class IntrusiveList
{
    T* m_head{};

    static void insert_after(T* before, T* after)
    {
        after->m_prev = before;
        after->m_next = before->m_next;

        before->m_next = after;
        after->m_next->m_prev = after;
    }

public:
    constexpr IntrusiveList() = default;

    explicit IntrusiveList(T* head)
    {
        push_front(head);
    }

    ASL_DELETE_COPY(IntrusiveList)
    ASL_DEFAULT_MOVE(IntrusiveList)
    ~IntrusiveList() = default;

    constexpr bool is_empty() const { return m_head == nullptr; }

    void push_front(T* node)
    {
        ASL_ASSERT(node->m_next == nullptr && node->m_prev == nullptr);
        if (is_empty())
        {
            m_head = node;
            node->m_prev = node;
            node->m_next = node;
        }
        else
        {
            insert_after(m_head->m_prev, node);
            m_head = node;
        }
    }

    void push_back(T* node)
    {
        ASL_ASSERT(node->m_next == nullptr && node->m_prev == nullptr);
        if (is_empty())
        {
            m_head = node;
            node->m_prev = node;
            node->m_next = node;
        }
        else
        {
            insert_after(m_head->m_prev, node);
        }
    }

    constexpr T* head() const
    {
        return m_head;
    }

    constexpr T* tail() const
    {
        return m_head != nullptr ? m_head->m_prev : nullptr;
    }

    void detach(T* node)
    {
        ASL_ASSERT(node->m_next != nullptr && node->m_prev != nullptr);

        if (m_head->m_next == m_head)
        {
            ASL_ASSERT(m_head->m_prev == m_head);
            ASL_ASSERT(m_head == node);
            m_head = nullptr;
        }
        else
        {
            if (m_head == node)
            {
                m_head = node->m_next;
            }

            node->m_prev->m_next = node->m_next;
            node->m_next->m_prev = node->m_prev;
        }

        node->m_next = nullptr;
        node->m_prev = nullptr;
    }

    T* pop_front()
    {
        if (!is_empty())
        {
            T* node = m_head;
            detach(node);
            return node;
        }
        return nullptr;
    }

    T* pop_back()
    {
        if (!is_empty())
        {
            T* node = m_head->m_prev;
            detach(node);
            return node;
        }
        return nullptr;
    }

    template<typename TT>
    struct generic_iterator
    {
        TT*  m_node;
        bool m_advanced = false;

    public:
        constexpr explicit generic_iterator(TT* node, bool end = false)
            : m_node{node}
            , m_advanced{end}
        {}

        constexpr bool operator==(const generic_iterator& other) const = default;

        constexpr generic_iterator& operator++()
        {
            m_node = m_node->m_next;
            m_advanced = true;
            return *this;
        }

        constexpr generic_iterator operator++(int)
        {
            return iterator{
                exchange(m_node, m_node->m_next), exchange(m_advanced, true)
            };
        }

        constexpr TT& operator*() const { return *m_node; }

        constexpr TT* operator->() const { return m_node; }
    };

    using iterator = generic_iterator<T>;
    using const_iterator = generic_iterator<const T>;

    // @Todo(C++23) Deduplicate with deducing-this maybe
    const_iterator begin() const
    {
        return const_iterator{ head(), is_empty() };
    }
    
    const_iterator end() const
    {
        return const_iterator{ head(), true };
    }

    iterator begin()
    {
        return iterator{ head(), is_empty() };
    }
    
    iterator end()
    {
        return iterator{ head(), true };
    }
};

}

