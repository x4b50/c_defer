# C defer
A header only implementation of defer behavior in C language.

## Example
```c
#define DEFER_IMPL
#include "defer.h"

int main() {
    defers(printf("this gets executed when returning from a function"));
    int *tst = calloc(100, sizeof(int));
    defer(tst);
    for (int i=0; i<10; i++) {
        tst[i] += i;
        printf("%d ", tst[i]);
    }
    // return with parenthesis, because it gets overwritten with a macro
    return(0);
}
```
