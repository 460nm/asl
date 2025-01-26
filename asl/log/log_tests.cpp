#include <asl/log/log.hpp>
#include <asl/testing/testing.hpp>

ASL_TEST(log)
{
    asl::log::log(asl::log::kInfo, asl::source_location{}, "Hello, {}!", "world"_sv);

    ASL_LOG_ERROR("Oh no! {}", 42);
}

