// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/defer.hpp"
#include "asl/logging/logging.hpp"
#include "asl/strings/string_builder.hpp"
#include "asl/strings/string_view.hpp"
#include "asl/testing/testing.hpp"

ASL_TEST(log)
{
    asl::log::log(asl::log::level::kInfo, asl::source_location{}, "Hello, {}!", "world"_sv);

    ASL_LOG_ERROR("Oh no! {}", 42);
}

ASL_TEST(custom_writer)
{
    asl::StringWriter string_writer{};
    asl::log::DefaultLogger<asl::StringWriter<>&> logger(string_writer);

    asl::log::register_logger(&logger);
    ASL_DEFER [&logger]() {
        asl::log::unregister_logger(&logger);
    };

    ASL_LOG_INFO("Hello");
    auto sv = string_writer.as_string_view();

    ASL_TEST_EXPECT(sv == "[  INFO   ] src/asl/logging/logging_tests.cpp:28: Hello\n");
}

