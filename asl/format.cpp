#include "asl/format.hpp"

void asl::format_internals::format(
    writer* writer,
    const char* fmt,
    const type_erased_arg* args,
    int64_t arg_count)
{
    formatter f(writer);


    int64_t arg = 0;

    const char* begin = fmt;
    while (*fmt != '\0')
    {
        if (fmt[0] == '{')
        {
            if (fmt[1] == '}')
            {
                if (arg < arg_count)
                {
                    f.write(begin, fmt - begin);
                    fmt += 2;
                    begin = fmt;

                    args[arg].fn(f, args[arg].data);
                    arg += 1;
                }
                else
                {
                    f.write(begin, fmt - begin);
                    fmt += 2;
                    begin = fmt;

                    f.write("<ERROR>", 7);
                }
            }
            else if (fmt[1] == '{')
            {
                fmt += 1;
                f.write(begin, fmt - begin);
                fmt += 1;
                begin = fmt;
            }
            else
            {
                f.write(begin, fmt - begin);
                fmt += 1;
                begin = fmt;

                f.write("<ERROR>", 7);
            }
        }
        else if (fmt[0] == '}' && fmt[1] == '}')
        {
            fmt += 1;
            f.write(begin, fmt - begin);
            fmt += 1;
            begin = fmt;
        }
        else
        {
            fmt += 1;
        }
    }

    f.write(begin, fmt - begin);
}
