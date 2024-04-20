#include <stdlib.h>
#include <string.h>

#ifndef DEFER_DEF
#define DEFER_DEF

#ifndef DEFER_COUNT
#define DEFER_COUNT 8
#endif
#ifndef DEFER_NAME_LEN
#define DEFER_NAME_LEN 64
#endif

// a list of defer allocations in a given function
typedef struct {
    char fname[DEFER_NAME_LEN];
    void *list[DEFER_COUNT];
    int count;
} __defer_list;

// linked list if defers
typedef struct __defer_node {
    __defer_list val;
    struct __defer_node *next;
} __defer_node;

// add a new list of defers to ll
void __dl_push(__defer_node *head, __defer_list val);
// remove the last list of defers
void __dl_pop(__defer_node *head);
// get the last list of defers
__defer_list *__dl_last(__defer_node *head);

// macro for adding defer to either the list for the function or creating a new one
#define defer(v) { \
    __defer_list *_d = __dl_last(&__defer); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        _d->count += 1; \
        _d->list[_d->count] = v; \
    } else { \
        __defer_list _d = {.list=v, .count=0}; \
        strncpy(_d.fname, __func__, DEFER_NAME_LEN); \
        __dl_push(&__defer, _d); \
    } \
}

#ifndef DEFER_PEDANTIC
// a list of defer allocations in a given function
typedef struct {
    char fname[DEFER_NAME_LEN];
    void (*list[DEFER_COUNT])(void);
    int count;
} __defers_list;

// linked list if defers
typedef struct __defers_node {
    __defers_list val;
    struct __defers_node *next;
} __defers_node;

// add a new list of defers to ll
void __dsl_push(__defers_node *head, __defers_list val);
// remove the last list of defers
void __dsl_pop(__defers_node *head);
// get the last list of defers
__defers_list *__dsl_last(__defers_node *head);

// macro for adding defer to either the list for the function or creating a new one
#define defers(v) { \
    void f(void) {v;} \
    __defers_list *_d = __dsl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        _d->count += 1; \
        _d->list[_d->count] = &f; \
    } else { \
        __defers_list _d = {.list=&f, .count=0}; \
        strncpy(_d.fname, __func__, DEFER_NAME_LEN); \
        __dsl_push(&__defers, _d); \
    } \
}
#endif

// free all defers allocated in the given function and return
#ifndef DEFER_PEDANTIC
#define return(v) { \
    __defers_list *_ds = __dsl_last(&__defers); \
    char _funcs[DEFER_NAME_LEN]; \
    strncpy(_funcs, __func__, DEFER_NAME_LEN); \
    if (strcmp(_ds->fname, _funcs) == 0) { \
        for (int _i=_ds->count; _i>=0; _i--) { \
            (_ds->list[_i])(); \
        } \
        __dsl_pop(&__defers); \
    } \
    __defer_list *_d = __dl_last(&__defer); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        for (int _i=_d->count; _i>=0; _i--) { \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defer); \
    } \
} return v

// debug just to demonstrate that it works
#define return_dbg(v) { \
    __defers_list *_ds = __dsl_last(&__defers); \
    char _funcs[DEFER_NAME_LEN]; \
    strncpy(_funcs, __func__, DEFER_NAME_LEN); \
    if (strcmp(_ds->fname, _funcs) == 0) { \
        for (int _i=_ds->count; _i>=0; _i--) { \
            (_ds->list[_i])(); \
        } \
        __dsl_pop(&__defers); \
    } \
    __defer_list *_d = __dl_last(&__defer); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        printf("freeing:\n"); \
        for (int _i=_d->count; _i>=0; _i--) { \
            printf("%ld\n", (long)_d->list[_i]); \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defer); \
    } \
} return v
#else // DEFER_PEDANTIC
#define return(v) { \
    __defer_list *_d = __dl_last(&__defer); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        for (int _i=_d->count; _i>=0; _i--) { \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defer); \
    } \
} return v

#define return_dbg(v) { \
    __defer_list *_d = __dl_last(&__defer); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        printf("freeing:\n"); \
        for (int _i=_d->count; _i>=0; _i--) { \
            printf("%ld\n", (long)_d->list[_i]); \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defer); \
    } \
} return v
#endif // DEFER_PEDANTIC
#endif // DEFER_DEF

#ifdef DEFER_IMPL
void __dl_push(__defer_node *head, __defer_list val) {
    __defer_node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (__defer_node*) malloc(sizeof(__defer_node));
    current->next->val = val;
    current->next->next = NULL;
}

void __dl_pop(__defer_node *head) {
    if (head->next == NULL) {
        __defer_node h;
        *head = h;
    }
    __defer_node *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

__defer_list *__dl_last(__defer_node *head) {
    __defer_list *ret;
    if (head->next == NULL) {
        ret = &head->val;
        return ret;
    }
    __defer_node *current = head->next;
    while (current->next != NULL) {
        current = current->next;
    }
    ret = &current->val;
    return ret;
}

// linked list of defer lists for each function
__defer_node __defer;

#ifndef DEFER_PEDANTIC
void __dsl_push(__defers_node *head, __defers_list val) {
    __defers_node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (__defers_node*) malloc(sizeof(__defers_node));
    current->next->val = val;
    current->next->next = NULL;
}

void __dsl_pop(__defers_node *head) {
    if (head->next == NULL) {
        __defers_node h;
        *head = h;
    }
    __defers_node *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

__defers_list *__dsl_last(__defers_node *head) {
    __defers_list *ret;
    if (head->next == NULL) {
        ret = &head->val;
        return ret;
    }
    __defers_node *current = head->next;
    while (current->next != NULL) {
        current = current->next;
    }
    ret = &current->val;
    return ret;
}
__defers_node __defers;
#endif
#endif // DEFER_IMPL
