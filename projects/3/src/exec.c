#include "exec.h"

// Parses command string and runs exec()
void _execute_cmd(char* cmd) {
    int i, n_tok, tok_size = 10;
    char *buffer, **tokens;

    // avoid strtok destruction of command line by making a copy
    buffer = calloc(CMD_MAX + 1, sizeof(char));
    memset(buffer, '\0', CMD_MAX + 1);
    strcpy(buffer, cmd);

    // array of strings to hold tokens
    tokens = calloc(tok_size, sizeof(char*));
    for (i = 0; i < tok_size; i++) { tokens[i] = NULL; }

    // Read first token from string
    i = 0;
    tokens[i] = strtok(buffer, " ");

    // Loop until there are no more tokens
    while (tokens[i] != NULL) {
        // Ignore shell characters by overwriting them
        if (   strcmp(tokens[i], "<") == 0
            || strcmp(tokens[i], ">") == 0
            || strcmp(tokens[i], "&") == 0
        ) {
            tokens[i] = NULL;
        } else {
            i++;
            n_tok = i;
        }

        // Don't run out of space in tokens array!
        if (i >= tok_size && tok_size <= NARG_MAX) {
            // Don't allocate space for more than 512 args
            tok_size = (tok_size * 2) % NARG_MAX;
            tokens = (char **)reallocarray(tokens, (size_t) tok_size, sizeof(char*));
            for (i = n_tok; i < tok_size; i++) { tokens[i] = NULL; }
            i = n_tok;
        }

        // If number of args exceeds 512
        if (i > NARG_MAX) {
            // Print an error and stop
            fprintf(stderr, "smallsh: Too many arguments\n");
            fflush(stderr);
            free(buffer);
            free(tokens);
            exit(512);
        }

        // Read next token from string
        tokens[i] = strtok(NULL, " ");
    }

    // Execute tokenized command
    execvp(tokens[0], tokens);

    // If exec returns, something went wrong
    fprintf(stderr, "smallsh: %s: %s\n", strerror(errno), tokens[0]);
    fflush(stderr);

    // Clean up and exit
    free(buffer);
    free(tokens);

    exit(1);
}