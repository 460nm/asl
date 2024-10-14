#include "asl/print.hpp"

#include <cstdio>

// @Todo Optimize this, maybe make buffered
class ConsoleWriter : public asl::writer
{
    FILE* m_handle;
    
public:
    explicit ConsoleWriter(FILE* handle)
        : m_handle{handle}
    {}
    
    void write(const char* str, int64_t len) override
    {
        fwrite(str, 1, static_cast<size_t>(len), m_handle);
    }
};

asl::writer* asl::print_internals::get_stdout_writer()
{
    static ConsoleWriter writer{stdout};
    return &writer;
}

asl::writer* asl::print_internals::get_stderr_writer()
{
    static ConsoleWriter writer{stderr};
    return &writer;
}
