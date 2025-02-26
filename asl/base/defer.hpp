#pragma once

#include "asl/base/utility.hpp"
#include "asl/base/functional.hpp"

namespace asl
{

template<invocable Callback>
class DeferCallback
{
    Callback m_callback;
    bool     m_moved = false;

public:
    template<typename T>
    explicit DeferCallback(T&& callback) : m_callback(ASL_FWD(callback))
    {
    }

    ASL_DELETE_COPY(DeferCallback);

    DeferCallback(DeferCallback&& other) :
        m_callback(ASL_MOVE(other.m_callback)), m_moved(exchange(other.m_moved, true))
    {
    }

    DeferCallback& operator=(DeferCallback&&) = delete;

    ~DeferCallback()
    {
        if (!m_moved) { invoke(m_callback); }
    }
};

struct DeferFactory
{
    template<invocable Callback>
    DeferCallback<Callback> operator<<(Callback&& callback) const
    {
        return DeferCallback<Callback>(ASL_FWD(callback));
    }
};

} // namespace asl

#define ASL_DEFER auto ASL_CONCAT(_defer_, __COUNTER__) = ::asl::DeferFactory{} <<

