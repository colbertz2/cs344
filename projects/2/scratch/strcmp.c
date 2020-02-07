#include <stdio.h>
#include <string.h>

int main() {
    char a[] = "TEST";
    char b[] = "test";
    char c[] = "Test";

    printf("%d\n", strcmp(a, a));
    printf("%d\n", strcmp(a, b));
    printf("%d\n", strcmp(a, c));

    return 0;
}