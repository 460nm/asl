#include "asl/format.hpp"

void asl::format_internals::format(
    writer* writer,
    const char* fmt,
    const type_erased_arg* args,
    int64_t arg_count)
{
    formatter f(writer);
    
    f.write("Hello", 5);
}
