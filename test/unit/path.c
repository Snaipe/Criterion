#include <stddef.h>

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

Test(path, cri_path_build)
{
    const char s = '/';
    char *o = NULL;
    size_t ss = 0;

    cr_expect_eq(cri_path_build(s, &o, &ss, NULL), 0);

    cri_path_build(s, &o, &ss, "", NULL); cr_expect_str_eq(o, "");
    cri_path_build(s, &o, &ss, " ", NULL); cr_expect_str_eq(o, " ");
    cri_path_build(s, &o, &ss, "/", NULL); cr_expect_str_eq(o, "/");
    cri_path_build(s, &o, &ss, "//", NULL); cr_expect_str_eq(o, "//");
    cri_path_build(s, &o, &ss, "///", NULL); cr_expect_str_eq(o, "///");

    cri_path_build(s, &o, &ss, "a", NULL); cr_expect_str_eq(o, "a");
    cri_path_build(s, &o, &ss, "//a//", NULL); cr_expect_str_eq(o, "//a//");

    cri_path_build(s, &o, &ss, "/a", "b", "c", NULL); cr_expect_str_eq(o, "/a/b/c");
    cri_path_build(s, &o, &ss, "a", "b", "c/", NULL); cr_expect_str_eq(o, "a/b/c/");
    cri_path_build(s, &o, &ss, "/a", "b", "c/", NULL); cr_expect_str_eq(o, "/a/b/c/");

    cri_path_build(s, &o, &ss, "a/b", "c//d", "e", NULL); cr_expect_str_eq(o, "a/b/c//d/e");
    cri_path_build(s, &o, &ss, "/a/b", "//c//d//", "e//", NULL); cr_expect_str_eq(o, "/a/b/c//d/e//");

    cri_path_build(s, &o, &ss, "/", "a", "/", NULL); cr_expect_str_eq(o, "/a/");
    cri_path_build(s, &o, &ss, "//", "a", "//", NULL); cr_expect_str_eq(o, "//a//");
    cri_path_build(s, &o, &ss, "///", "a", "///", NULL); cr_expect_str_eq(o, "///a///");

    cri_path_build(s, &o, &ss, "a", "///", NULL); cr_expect_str_eq(o, "a///");
    cri_path_build(s, &o, &ss, "///", "a", NULL); cr_expect_str_eq(o, "///a");

    cri_path_build(s, &o, &ss, "a", "///", "b", NULL); cr_expect_str_eq(o, "a/b");
    cri_path_build(s, &o, &ss, "a", "/", "b", NULL); cr_expect_str_eq(o, "a/b");

    cri_path_build(s, &o, &ss, "", "a", "", "b//", "/", "", NULL); cr_expect_str_eq(o, "a/b/");
    cri_path_build(s, &o, &ss, "a", "b/", "", "/", "///", "c", NULL); cr_expect_str_eq(o, "a/b/c");

    cri_path_build(s, &o, &ss, "a", "b", "c", NULL); cr_expect_str_eq(o, "a/b/c");
    cri_path_build(s, &o, &ss, "/a/", "/b/", "/c", "d/", "e", NULL); cr_expect_str_eq(o, "/a/b/c/d/e");
    cri_path_build(s, &o, &ss, "//a//", "//b//", "//c//", NULL); cr_expect_str_eq(o, "//a/b/c//");

    free(o);
}

Test(path, cri_path_build_win)
{
    const char s = '\\';
    char *o = NULL;
    size_t ss = 0;

    cri_path_build(s, &o, &ss, "C:\\", "\\Users\\", "Snaipe", NULL); cr_expect_str_eq(o, "C:\\Users\\Snaipe");
    cri_path_build(s, &o, &ss, "C:/", "\\Users//", "Snaipe\\", NULL); cr_expect_str_eq(o, "C:\\Users\\Snaipe\\");
    cri_path_build(s, &o, &ss, "\\", "//test\\", "\\", NULL); cr_expect_str_eq(o, "\\test\\");

    free(o);
}
