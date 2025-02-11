#include "asl/log/log.hpp"
#include "asl/testing/testing.hpp"
#include "asl/string_builder.hpp"

ASL_TEST(log)
{
    asl::log::log(asl::log::kInfo, asl::source_location{}, "Hello, {}!", "world"_sv);

    ASL_LOG_ERROR("Oh no! {}", 42);
}

static asl::StringWriter g_string_writer{};

ASL_TEST(custom_writer)
{
    asl::log::register_logger<asl::log::DefaultLogger<asl::StringWriter<>&>>(g_string_writer);

    ASL_LOG_INFO("Hello");
    auto sv = g_string_writer.as_string_view();

    ASL_TEST_EXPECT(sv == "[  INFO   ] asl/log/log_tests.cpp:18: Hello\n");
}

