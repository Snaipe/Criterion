#include "criterion/criterion.h"
#include "criterion/alloc.h"

struct Obj {
    int foo;
    long bar;

    Obj() {}
    Obj(int foo, long bar) : foo(foo), bar(bar) {}
};

Test(alloc, object) {
    Obj *o = criterion::new_obj<Obj>(42, 314);

    cr_assert_not_null(o);

    cr_assert_eq(o->foo, 42);
    cr_assert_eq(o->bar, 314);

    criterion::delete_obj(o);
}

Test(alloc, array) {
    Obj *o = criterion::new_arr<Obj>(3);

    cr_assert_not_null(o);

    new (&o[0])Obj(1, 2);
    new (&o[1])Obj(2, 4);
    new (&o[2])Obj(3, 6);

    for (int i = 0; i < 3; ++i) {
        cr_assert_eq(o[i].foo, i + 1);
        cr_assert_eq(o[i].bar, 2 * (i + 1));
    }

    criterion::delete_arr(o);
}
