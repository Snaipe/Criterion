#include <criterion/criterion.h>
#include <criterion/logging.h>

using criterion::logging::info;
using criterion::logging::warn;
using criterion::logging::error;

Test(logging, stream) {
    info << "This is an informational message. They are not displayed "
        "by default."
         << std::endl;
    warn << "This is a warning. They indicate some possible malfunction "
        "or misconfiguration in the test."
         << std::endl;
    error << "This is an error. They indicate serious problems and "
        "are usually shown before the test is aborted."
          << std::endl;
}
