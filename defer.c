#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// /*
// can be replaced w/ linked list if you want a more versatile solution
#define DEFER_COUNT 32
#define DEFER_NAME_LEN 128
typedef struct {
    char fname[DEFER_NAME_LEN];
    void *list[DEFER_COUNT];
    int count;
} __defer_list;

typedef struct __def_node {
    __defer_list val;
    struct __def_node *next;
} __def_node;

void __dl_push(__def_node *head, __defer_list val) {
    __def_node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (__def_node*) malloc(sizeof(__def_node));
    current->next->val = val;
    current->next->next = NULL;
}

void __dl_pop(__def_node *head) {
    if (head->next == NULL) {
        free(head);
    }
    __def_node *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

__defer_list *__dl_last(__def_node *head) {
    __defer_list *ret;
    if (head->next == NULL) {
        ret = &head->val;
        return ret;
    }
    __def_node *current = head->next;
    while (current->next != NULL) {
        current = current->next;
    }
    ret = &current->val;
    return ret;
}

__def_node __defers;

#define defer(v) { \
    __defer_list *_d = __dl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, 128); \
    if (strcmp(_d->fname, _func) == 0) { \
        _d->count += 1; \
        _d->list[_d->count] = v; \
    } else { \
        __defer_list _d = {.list=v, .count=0}; \
        strncpy(_d.fname, __func__, 128); \
        __dl_push(&__defers, _d); \
    } \
}

#define return(v) { \
    __defer_list *_d = __dl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, 128); \
    if (strcmp(_d->fname, _func) == 0) { \
        for (int _i=_d->count; _i>=0; _i--) { \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defers); \
    } \
} return v

// just to demonstrate that it works
#define return_dbg(v) { \
    __defer_list *_d = __dl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, 128); \
    if (strcmp(_d->fname, _func) == 0) { \
        for (int _i=_d->count; _i>=0; _i--) { \
            printf("%ld\n", (long)_d->list[_i]); \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defers); \
    } \
} return v

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
