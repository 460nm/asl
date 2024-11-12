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
    
    void write(asl::span<const asl::byte> s) override
    {
        fwrite(s.data(), 1, static_cast<size_t>(s.size()), m_handle);
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
