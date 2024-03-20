# C defer
A very simple implementation and demonstration of defer behavior in C language.

## Example
```c
int main() {
    int *tst = calloc(100, sizeof(int));
    defer(tst);
    for (int i=0; i<10; i++) {
        tst[i] += i;
        printf("%d ", tst[i]);
    }
    return(0);
}
```
