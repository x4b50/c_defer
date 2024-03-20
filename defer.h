#include <stdlib.h>
#include <string.h>

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
typedef struct __def_node {
    __defer_list val;
    struct __def_node *next;
} __def_node;

// add a new list of defers to ll
void __dl_push(__def_node *head, __defer_list val);
// remove the last list of defers
void __dl_pop(__def_node *head);
// get the last list of defers
__defer_list *__dl_last(__def_node *head);

// macro for adding defer to either the list for the function or creating a new one
#define defer(v) { \
    __defer_list *_d = __dl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        _d->count += 1; \
        _d->list[_d->count] = v; \
    } else { \
        __defer_list _d = {.list=v, .count=0}; \
        strncpy(_d.fname, __func__, DEFER_NAME_LEN); \
        __dl_push(&__defers, _d); \
    } \
}

// free all defers allocated in the given function and return
#define return(v) { \
    __defer_list *_d = __dl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        for (int _i=_d->count; _i>=0; _i--) { \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defers); \
    } \
} return v

// debug just to demonstrate that it works
#define return_dbg(v) { \
    __defer_list *_d = __dl_last(&__defers); \
    char _func[DEFER_NAME_LEN]; \
    strncpy(_func, __func__, DEFER_NAME_LEN); \
    if (strcmp(_d->fname, _func) == 0) { \
        printf("freeing:\n"); \
        for (int _i=_d->count; _i>=0; _i--) { \
            printf("%ld\n", (long)_d->list[_i]); \
            free(_d->list[_i]); \
        } \
        __dl_pop(&__defers); \
    } \
} return v

#ifdef DEFER_IMPL
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
        __def_node h;
        *head = h;
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

// linked list of defer lists for each function
__def_node __defers;
#endif
