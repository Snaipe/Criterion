#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(bug521, str) {
  static const char *const foo = "foo";
  cr_assert(eq(str, foo, "foo"));
  cr_assert(ne(str, foo, "oof"));
}

Test(bug521, wcs) {
  static const wchar_t *const foo = L"foo";
  cr_assert(eq(wcs, foo, L"foo"));
  cr_assert(ne(wcs, foo, L"oof"));
}

Test(bug521, ptr) {
  static const char *const foo = "foo";
  cr_assert(eq(ptr, foo, foo));
}