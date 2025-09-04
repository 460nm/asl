// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/formatting/format.hpp"
#include "asl/containers/intrusive_list.hpp"

namespace asl::log
{

enum class level : uint8_t
{
    kDebug = 0,
    kInfo,
    kWarning,
    kError,
};

struct message
{
    level level;
    string_view message;
    source_location location;
};

class Logger : public intrusive_list_node<Logger>
{
public:
    Logger() = default;
    ASL_DEFAULT_COPY_MOVE(Logger);
    virtual ~Logger() = default;

    virtual void log(const message&) = 0;
};

class DefaultLoggerBase : public Logger
{
protected:
    static void log_inner(Writer&, const message&);
};

template<derefs_as<Writer> W>
class DefaultLogger : public DefaultLoggerBase
{
    W m_writer;

public:
    template<typename U>
    explicit constexpr DefaultLogger(U&& writer)
        requires constructible_from<W, U&&>
        : m_writer{std::forward<U>(writer)}
    {}

    constexpr void log(const message& m) override
    {
        log_inner(deref<Writer>(m_writer), m);
    }
};

void register_logger(Logger*);
void unregister_logger(Logger*);

void remove_default_logger();

// @Todo Add a way to remove loggers (including all)

void log_inner(level l, string_view fmt, span<const format_internals::type_erased_arg> args, const source_location& sl);

template<formattable... Args>
void log(level l, const source_location& sl, string_view fmt, const Args&... args)
{
    if constexpr (sizeof...(Args) == 0)
    {
        log_inner(l, fmt, {}, sl);
    }
    else
    {
        const format_internals::type_erased_arg type_erased_args[] = {
            format_internals::type_erased_arg(args)...
        };
        log_inner(l, fmt, type_erased_args, sl);
    }
}

} // namespace asl::log

#if !defined(ASL_LOG_LEVEL) || ASL_LOG_LEVEL >= 4
    #define ASL_LOG_DEBUG(...)   ::asl::log::log(::asl::log::level::kDebug,   ::asl::source_location{}, __VA_ARGS__)
#else
    #define ASL_LOG_DEBUG(...)
#endif

#if !defined(ASL_LOG_LEVEL) || ASL_LOG_LEVEL >= 3
    #define ASL_LOG_INFO(...)    ::asl::log::log(::asl::log::level::kInfo,    ::asl::source_location{}, __VA_ARGS__)
#else
    #define ASL_LOG_DEBUG(...)
#endif

#if !defined(ASL_LOG_LEVEL) || ASL_LOG_LEVEL >= 2
    #define ASL_LOG_WARNING(...) ::asl::log::log(::asl::log::level::kWarning, ::asl::source_location{}, __VA_ARGS__)
#else
    #define ASL_LOG_DEBUG(...)
#endif

#if !defined(ASL_LOG_LEVEL) || ASL_LOG_LEVEL >= 1
    #define ASL_LOG_ERROR(...)   ::asl::log::log(::asl::log::level::kError,   ::asl::source_location{}, __VA_ARGS__)
#else
    #define ASL_LOG_DEBUG(...)
#endif
