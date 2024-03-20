#include <stdio.h>
#include <stdlib.h>

#define DEFER_IMPL
// #define DEFER_PEDANTIC
#include "defer.h"

int test_defer() {
    defers(printf("cleanup function in defer\n"));
    int *tst = calloc(100, sizeof(int));
    defer(tst);
    printf("%ld\n", (long)tst);
    return_dbg(1);
}

int main() {
    int *tst = calloc(100, sizeof(int));
    defer(tst);
    for (int i=0; i<10; i++) {
        tst[i] += i;
        printf("%d ", tst[i]);
    } printf("\n");

    int *t = calloc(10, sizeof(int));
    defer(t);
    for (int i=0; i<10; i++) {
        t[i] += i;
        printf("%d ", t[i]);
    } printf("\n");

    int *t1 = calloc(100, sizeof(int));
    int *t2 = calloc(100, sizeof(int));
    defer(t1);
    defer(t2);

    printf("%ld\n", (long)tst);
    printf("%ld\n", (long)t);
    printf("%ld\n", (long)t1);
    printf("%ld\n", (long)t2);

    printf("-------------\n");
    test_defer();
    printf("-------------\n");
    return_dbg(0);
}
