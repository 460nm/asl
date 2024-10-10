#include "asl/format.hpp"

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

int main()
{
    StringSink sink;

    // @Todo Use the testing framework

    asl::format(&sink, "Hello, world!");
    assert(strcmp(sink.cstr(), "Hello, world!") == 0);

    sink.reset();
    asl::format(&sink, "");
    assert(strcmp(sink.cstr(), "") == 0);

    sink.reset();
    asl::format(&sink, "Hello, {}!", "world");
    assert(strcmp(sink.cstr(), "Hello, world!") == 0);

    sink.reset();
    asl::format(&sink, "Hello, {}! {}", "world");
    assert(strcmp(sink.cstr(), "Hello, world! <ERROR>") == 0);

    sink.reset();
    asl::format(&sink, "Hello, pup!", "world");
    assert(strcmp(sink.cstr(), "Hello, pup!") == 0);

    sink.reset();
    asl::format(&sink, "{}", "CHEESE");
    assert(strcmp(sink.cstr(), "CHEESE") == 0);

    sink.reset();
    asl::format(&sink, "{   ", "CHEESE");
    assert(strcmp(sink.cstr(), "<ERROR>   ") == 0);

    sink.reset();
    asl::format(&sink, "{", "CHEESE");
    assert(strcmp(sink.cstr(), "<ERROR>") == 0);

    sink.reset();
    asl::format(&sink, "a{{b");
    assert(strcmp(sink.cstr(), "a{b") == 0);

    sink.reset();
    asl::format(&sink, "{{{}}} }", "CHEESE");
    assert(strcmp(sink.cstr(), "{CHEESE} }") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 0, 1, 2);
    assert(strcmp(sink.cstr(), "0 1 2") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 10, 11, 12);
    assert(strcmp(sink.cstr(), "10 11 12") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 100, 101, 102);
    assert(strcmp(sink.cstr(), "100 101 102") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {}", 1000, 1001, 1002);
    assert(strcmp(sink.cstr(), "1000 1001 1002") == 0);

    sink.reset();
    asl::format(&sink, "{} {} {} {}", -1, -23, -456, -7890);
    assert(strcmp(sink.cstr(), "-1 -23 -456 -7890") == 0);

    return 0;
}
