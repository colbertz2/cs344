#include <stdio.h>
#include <stdlib.h>

int main() {
    char* buffer = NULL;
    size_t buffer_size = 0;
    size_t characters = -1;

    printf("buffer location: %p\n", buffer);
    characters = getline(&buffer, &buffer_size, stdin);

    printf("buffer location: %p\n", buffer);
    printf("%s", buffer);
    printf("%d\n", (int)buffer_size);
    printf("%d\n", (int)characters);

    return 0;
}