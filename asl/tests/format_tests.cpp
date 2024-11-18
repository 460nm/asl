#include "asl/format.hpp"
#include "asl/testing/testing.hpp"
#include "asl/allocator.hpp"

static_assert(asl::formattable<decltype("Hello")>);

class StringSink : public asl::Writer
{
    // @Todo Use string, once we have it, or a buffer
    isize_t m_current_len{};
    char*   m_data{};
    
public:
    void write(asl::span<const asl::byte> str) override
    {
        m_data = reinterpret_cast<char*>(asl::GlobalHeap::realloc(
            m_data,
            asl::layout::array<char>(m_current_len),
            asl::layout::array<char>(m_current_len + str.size())));
        
        asl::memcpy(m_data + m_current_len, str.data(), str.size());
        
        m_current_len += str.size();
    }

    constexpr asl::string_view str() const { return {m_data, m_current_len}; }

    void reset()
    {
        m_current_len = 0;
        asl::GlobalHeap::dealloc(m_data, asl::layout::array<char>(m_current_len));
        m_data = nullptr;
    }
};

ASL_TEST(format_args)
{
    StringSink sink;

    // @Todo Introduce ASL_TEST_EXPECT_EQ, or ASL_TEST_EXPECT_STREQ

    asl::format(&sink, "Hello, world!");
    ASL_TEST_EXPECT(sink.str() == "Hello, world!"_sv);

    sink.reset();
    asl::format(&sink, "");
    ASL_TEST_EXPECT(sink.str() == ""_sv);

    sink.reset();
    asl::format(&sink, "Hello, {}!", "world");
    ASL_TEST_EXPECT(sink.str() == "Hello, world!"_sv);

    sink.reset();
    asl::format(&sink, "Hello, {}! {}", "world");
    ASL_TEST_EXPECT(sink.str() == "Hello, world! <ERROR>"_sv);

    sink.reset();
    asl::format(&sink, "Hello, pup!", "world");
    ASL_TEST_EXPECT(sink.str() == "Hello, pup!"_sv);

    sink.reset();
    asl::format(&sink, "{}", "CHEESE");
    ASL_TEST_EXPECT(sink.str() == "CHEESE"_sv);

    sink.reset();
    asl::format(&sink, "{   ", "CHEESE");
    ASL_TEST_EXPECT(sink.str() == "<ERROR>   "_sv);

    sink.reset();
    asl::format(&sink, "{", "CHEESE");
    ASL_TEST_EXPECT(sink.str() == "<ERROR>"_sv);

    sink.reset();
    asl::format(&sink, "a{{b");
    ASL_TEST_EXPECT(sink.str() == "a{b"_sv);

    sink.reset();
    asl::format(&sink, "{{{}}} }", "CHEESE");
    ASL_TEST_EXPECT(sink.str() == "{CHEESE} }"_sv);
}

ASL_TEST(format_integers)
{
    StringSink sink;
    
    sink.reset();
    asl::format(&sink, "{} {} {}", 0, 1, 2);
    ASL_TEST_EXPECT(sink.str() == "0 1 2"_sv);

    sink.reset();
    asl::format(&sink, "{} {} {}", 10, 11, 12);
    ASL_TEST_EXPECT(sink.str() == "10 11 12"_sv);

    sink.reset();
    asl::format(&sink, "{} {} {}", 100, 101, 102);
    ASL_TEST_EXPECT(sink.str() == "100 101 102"_sv);

    sink.reset();
    asl::format(&sink, "{} {} {}", 1000, 1001, 1002);
    ASL_TEST_EXPECT(sink.str() == "1000 1001 1002"_sv);

    sink.reset();
    asl::format(&sink, "{} {} {} {}", -1, -23, -456, -7890);
    ASL_TEST_EXPECT(sink.str() == "-1 -23 -456 -7890"_sv);
}

ASL_TEST(format_boolean)
{
    StringSink sink;
    
    sink.reset();
    asl::format(&sink, "{} {}", true, false);
    ASL_TEST_EXPECT(sink.str() == "true false"_sv);
}
