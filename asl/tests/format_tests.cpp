#include "asl/format.hpp"
#include "asl/testing/testing.hpp"

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdio>

// @Todo Improve this to use our utilities, not the C stdlib

static_assert(asl::formattable<decltype("Hello")>);

class StringSink : public asl::writer
{
    int64_t m_current_len{};
    char*   m_data{};
    
public:
    void write(const char* str, int64_t len) override
    {
        m_data = (char*)realloc(m_data, (size_t)(m_current_len + len + 1));
        memcpy(m_data + m_current_len, str, (size_t)len);
        m_current_len += len;
        m_data[m_current_len] = '\0';
    }

    constexpr const char* cstr() const { return m_data; }

    void reset()
    {
        m_current_len = 0;
        free(m_data);
        m_data = nullptr;
    }
};

ASL_TEST(Format)
{
    StringSink sink;

    // @Todo Introduce ASL_TEST_ASSERT_EQ, or ASL_TEST_ASSERT_STREQ
    // @Todo Don't use strcmp for string comparison

    asl::format(&sink, "Hello, world!");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "Hello, world!") == 0);

    sink.reset();
    asl::format(&sink, "");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "") == 0);

    sink.reset();
    asl::format(&sink, "Hello, {}!", "world");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "Hello, world!") == 0);

    sink.reset();
    asl::format(&sink, "Hello, {}! {}", "world");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "Hello, world! <ERROR>") == 0);

    sink.reset();
    asl::format(&sink, "Hello, pup!", "world");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "Hello, pup!") == 0);

    sink.reset();
    asl::format(&sink, "{}", "CHEESE");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "CHEESE") == 0);

    sink.reset();
    asl::format(&sink, "{   ", "CHEESE");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "<ERROR>   ") == 0);

    sink.reset();
    asl::format(&sink, "{", "CHEESE");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "<ERROR>") == 0);

    sink.reset();
    asl::format(&sink, "a{{b");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "a{b") == 0);

    sink.reset();
    asl::format(&sink, "{{{}}} }", "CHEESE");
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "{CHEESE} }") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 0, 1, 2);
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "0 1 2") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 10, 11, 12);
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "10 11 12") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 100, 101, 102);
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "100 101 102") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 1000, 1001, 1002);
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "1000 1001 1002") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {} {}", -1, -23, -456, -7890);
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "-1 -23 -456 -7890") == 0);

    sink.reset();
    asl::format(&sink, "{} {}", true, false);
    ASL_TEST_ASSERT(strcmp(sink.cstr(), "true false") == 0);
}
