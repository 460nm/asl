#include "asl/assert.hpp"
#include "asl/print.hpp"


void asl::report_assert_failure(const char* msg, const source_location& sl)
{
    eprint("------------------------------------------------------------\n");
    eprint("Assertion failure at {}, line {}:\n", sl.file, sl.line);
    eprint("{}\n", msg);
    eprint("------------------------------------------------------------\n");
}
