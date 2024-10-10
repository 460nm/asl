#include "asl/print.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// @Todo Optimize this, maybe make buffered
class Win32ConsoleWriter : public asl::writer
{
    HANDLE m_handle;
    
public:
    explicit Win32ConsoleWriter(HANDLE handle)
        : m_handle{handle}
    {}
    
    void write(const char* str, int64_t len) override
    {
        ::WriteConsoleA(m_handle, str, static_cast<DWORD>(len), nullptr, nullptr);
    }
};

asl::writer* asl::print_internals::get_stdout_writer()
{
    static Win32ConsoleWriter writer{::GetStdHandle(STD_OUTPUT_HANDLE)};
    return &writer;
}

asl::writer* asl::print_internals::get_stderr_writer()
{
    static Win32ConsoleWriter writer{::GetStdHandle(STD_ERROR_HANDLE)};
    return &writer;
}

