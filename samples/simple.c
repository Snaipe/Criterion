#include <stdio.h>
#include <stdlib.h>
#include "criterion.h"

Test(misc, simple) {
}

Test(misc, failing) {
    exit(1);
}

/*void criterion_init(void) {
    puts("criterion_init");
}*/

void criterion_fini(void) {
    puts("criterion_fini");
}
