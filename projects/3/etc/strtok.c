#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
    int i, j, n_tokens = 10;
    char *buffer, *copy, **tokens;

    buffer = calloc(100, sizeof(char));
    memset(buffer, '\0', 100);
    copy = calloc(100, sizeof(char));
    memset(copy, '\0', 100);
    tokens = calloc(n_tokens, sizeof(char*));
    for (i = 0; i < n_tokens; i++) { tokens[i] = NULL; }

    strcpy(buffer, "mkdir test test1  te<st  &   < >    > test2");
    strcpy(copy, buffer);

    i = 0;
    tokens[i] = strtok(copy, " ");
    
    while (tokens[i] != NULL) {
        if (   strcmp(tokens[i], "<") == 0 
            || strcmp(tokens[i], ">") == 0
            || strcmp(tokens[i], "&") == 0
        ) { 
            tokens[i] = NULL;
        } else {
            i++;
        }

        if (i >= n_tokens) {
            // Allocate more space!
            n_tokens *= 2;
            tokens = (char **)reallocarray(tokens, (size_t) n_tokens, sizeof(char*));
            for (j = i; j < n_tokens; j++) { tokens[j] = NULL; }
        }

        tokens[i] = strtok(NULL, " ");
    }

    for (j = 0; j < i; j++) {
        printf("%s\n", tokens[j]);
    }

    free(buffer);
    free(copy);
    free(tokens);
}