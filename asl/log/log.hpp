#pragma once

#include <asl/format.hpp>
#include <asl/utility.hpp>
#include <asl/box.hpp>

namespace asl::log
{

enum level : uint8_t
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

// @Todo Write and use an intrusive doubly-linked list
class Logger
{
    Logger* m_next{};

public:
    Logger() = default;
    ASL_DEFAULT_COPY_MOVE(Logger);
    virtual ~Logger() = default;

    virtual void log(const message&) = 0;

    friend void register_logger(box<Logger>);

    constexpr Logger* next_logger() const { return m_next; }
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
    explicit constexpr DefaultLogger(W&& writer) : m_writer{ASL_FWD(writer)} {}

    constexpr void log(const message& m) override
    {
        log_inner(deref<Writer>(m_writer), m);
    }
};

void register_logger(box<Logger>);

// @Todo Add a way to remove loggers (including all)

template<typename T, typename... Args>
requires constructible_from<T, Args&&...> && convertible_from<Logger*, T*>
void register_logger(Args&&... args)
{
    register_logger(make_box<T>(ASL_FWD(args)...));
}

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
        format_internals::type_erased_arg type_erased_args[] = {
            format_internals::type_erased_arg(args)...
        };
        log_inner(l, fmt, type_erased_args, sl);
    }
}

} // namespace asl::log

// @Todo Compile-time configuration of logging

#define ASL_LOG_DEBUG(...) ::asl::log::log(::asl::log::kDebug, ::asl::source_location{}, __VA_ARGS__)
#define ASL_LOG_INFO(...) ::asl::log::log(::asl::log::kInfo, ::asl::source_location{}, __VA_ARGS__)
#define ASL_LOG_WARNING(...) ::asl::log::log(::asl::log::kWarning, ::asl::source_location{}, __VA_ARGS__)
#define ASL_LOG_ERROR(...) ::asl::log::log(::asl::log::kError, ::asl::source_location{}, __VA_ARGS__)
