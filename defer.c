#include <stdio.h>
#include <stdlib.h>

// /*
// can be replaced w/ linked list if you want a more versatile solution
#define DEFER_COUNT 32
void * __defer_list[DEFER_COUNT];
int __defer_count = 0;

static inline void defer(void *const v) {
    __defer_list[__defer_count] = v;
    __defer_count += 1;
}

#define return(v) \
    for (int _i=__defer_count-1; _i>=0; _i--) { \
        free(__defer_list[_i]); \
    } __defer_count=0; return v

// just to demonstrate that it works
#define return_dbg(v) \
    for (int _i=__defer_count-1; _i>=0; _i--) { \
        printf("%ld\n", (long)__defer_list[_i]); \
        free(__defer_list[_i]); \
    } __defer_count=0; return v

int empty_defer() {
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

    int _ = empty_defer();
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
