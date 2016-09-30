#include <criterion/criterion.h>
#include <criterion/logging.h>

class myobj {
public:
    ~myobj()
    {
        cr_log_warn("Unwind was OK");
    }
};

Test(misc, failing) {
    myobj obj;

    cr_assert(0);
}
