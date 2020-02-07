#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 25

int main() {
    FILE* inf;
    char buffer[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];

    inf = fopen("colbertz.rooms.18542/ballrm.rm", "r");

    memset(buffer, '\0', BUFFER_SIZE);
    memset(buffer2, '\0', BUFFER_SIZE);

    fscanf(inf, "ROOM NAME: %s", buffer);
    printf("%s\n", buffer);
    memset(buffer, '\0', strlen(buffer));
    // printf("Room name from file: %s\n", buffer);

    fscanf(inf, "%s %*s %s", buffer2, buffer);
    while (strcmp(buffer2, "CONNECTION") == 0) {
        printf("%s\n", buffer);
        fscanf(inf, "%s %*s %s", buffer2, buffer);
    }

    printf("%s\n", buffer);

    fclose(inf);

    return 0;
}