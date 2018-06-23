#include "criterion/criterion.h"
#include "compat/path.h"

Test(path, basename_compat)
{
    cr_expect_str_eq(basename_compat("/a/b/c/dir/basename.ext"), "basename.ext");
    cr_expect_str_eq(basename_compat("C:\\a\\b\\c\\dir\\basename.ext"), "basename.ext");
    cr_expect_str_eq(basename_compat("C:\\a\\b\\c/dir/basename.ext"), "basename.ext");

    cr_expect_str_eq(basename_compat("/a/basename/"), "basename/");
    cr_expect_str_eq(basename_compat("./"), "./");
    cr_expect_str_eq(basename_compat("../../"), "../");

    cr_expect_str_eq(basename_compat("a//b\\\\base"), "base");

    cr_expect_str_eq(basename_compat("basename"), "basename");
    cr_expect_str_eq(basename_compat(""), "");
}
