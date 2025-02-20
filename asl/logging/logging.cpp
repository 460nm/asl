#include "asl/logging/logging.hpp"
#include "asl/io/print.hpp"
#include "asl/strings/string_builder.hpp"

// @Todo Don't use internal get_stdout_writer, make console module

static asl::log::DefaultLogger<asl::Writer*> g_default_logger{asl::print_internals::get_stdout_writer()};

// @Todo Protect the loggers list being a mutex
static asl::IntrusiveList<asl::log::Logger> g_loggers(&g_default_logger);

void asl::log::register_logger(Logger* logger)
{
    g_loggers.push_front(logger);
}

void asl::log::unregister_logger(Logger* logger)
{
    g_loggers.detach(logger);
}

void asl::log::remove_default_logger()
{
    if (g_default_logger.m_next != nullptr)
    {
        g_loggers.detach(&g_default_logger);
    }
}

static constexpr asl::string_view kLevelName[] = {
    "  DEBUG  ",
    "  INFO   ",
    " WARNING ",
    "  ERROR  ",
};

void asl::log::DefaultLoggerBase::log_inner(Writer& writer, const message& msg)
{
    asl::format(&writer, "[{}] {}:{}: {}\n",
        kLevelName[msg.level], // NOLINT
        msg.location.file,
        msg.location.line,
        msg.message);
}

void asl::log::log_inner(
    level l,
    string_view fmt, span<const format_internals::type_erased_arg> args,
    const source_location& sl)
{
    // @Todo Use temporary allocator
    StringWriter msg_writer{};
    asl::format_internals::format(&msg_writer, fmt, args);

    message m{
        .level = l,
        .message = msg_writer.as_string_view(),
        .location = sl,
    };

    for (auto& logger: g_loggers)
    {
        logger.log(m);
    }
}

