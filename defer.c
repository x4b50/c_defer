#include <stdio.h>
#include <stdlib.h>

// /*
#define DEFER_IMPL
#include "defer.h"

int test_defer() {
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
    int _ = test_defer();
    printf("-------------\n");
    return_dbg(0);
}
// */

/*
#define defer(v, e) e; free(v);
#define defers(t, n, v, e) {t *n = v; e; free(n);}
int main() {
    int *t = malloc(sizeof(int)*100);
    defer(t, {
        for (int i=0; i<10; i++) {
            t[i] = i;
            printf("%d ", t[i]);
        }
        printf("\n%d\n", t[0]);
    })
    printf("%d\n", t[0]);

    defers(int, tst, calloc(100, sizeof(int)), {
        for (int i=0; i<10; i++) {
            tst[i] += i;
            printf("%d ", tst[i]);
        }
        printf("\n%d\n", tst[0]);
    })
    // printf("%d\n", tst[0]);
}
// */
