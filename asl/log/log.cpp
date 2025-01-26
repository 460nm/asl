#include "asl/log/log.hpp"
#include "asl/print.hpp"
#include "asl/string_builder.hpp"

// @Todo Don't use internal get_stdout_writer, make console module

static asl::log::DefaultLogger g_default_logger{asl::print_internals::get_stdout_writer()};
static asl::log::Logger* g_head = &g_default_logger;

static constexpr asl::string_view kLevelName[] = {
    "  DEBUG  ",
    "  INFO   ",
    " WARNING ",
    "  ERROR  ",
};

void asl::log::register_logger(box<Logger> logger_box)
{
    auto* logger = leak(ASL_MOVE(logger_box));
    logger->m_next = exchange(g_head, logger);
}

void asl::log::DefaultLogger::log(const message& msg)
{
    asl::format(m_writer, "[{}] {}:{}: {}\n",
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

    for (auto* it = g_head; it != nullptr; it = it->next_logger())
    {
        it->log(m);
    }
}

