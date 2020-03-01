#include "exec.h"

/*  Opens necessary file streams and parses command line
    Redirects stdin or stdout as directed

    STATUS CODES:
      0 :: Success
      -1 :: Input redirection failed
      1 :: Output redirection failed
      2 :: Both redirections failed
*/
int _set_redirects(char* cmd) {
    int i, inf, outf, status = 0;
    char* buffer, *token;

    // Avoid strtok destruction of command line by making a copy
    buffer = calloc(CMD_MAX + 1, sizeof(char));
    memset(buffer, '\0', CMD_MAX + 1);
    strcpy(buffer, cmd);

    // Tokenize word following input redirect character
    token = strtok(buffer, "<");
    token = strtok(NULL, "<");

    if (token != NULL) {
      // Trim leading space
      for (i = 1; i < strlen(token); i++) {
        token[i-1] = token[i];
      }
      token[strlen(token) - 1] = '\0';
      
      // Isolate the word following the redirect char
      if (strstr(token, " ") != NULL) { token = strtok(token, " "); }

      // Trim trailing newline if one exists
      i = strlen(token) - 1;
      if (token[i] == '\n') { token[i] = '\0'; }

      // DEV
      // fprintf(stderr, "Input Redirect: %s\n", token);
      // fflush(stderr);

      // Open input file
      inf = open(token, O_RDONLY);
      if (inf == -1) {
        fprintf(stderr, "smallsh: %s: %s\n", strerror(errno), token);
        fflush(stderr);
        status = -1;    // Indicates a problem with input redir to the caller
      } else {
        dup2(inf, 0);   // Redirect stdin to file
      }
    
      // Reset buffered copy of command line so we can tokenize again
      memset(buffer, '\0', CMD_MAX + 1);
      strcpy(buffer, cmd);
    }

    // Tokenize word following output redirect character
    token = strtok(buffer, ">");
    token = strtok(NULL, ">");

    if (token != NULL) {
      // Trim leading space
      for (i = 1; i < strlen(token); i++) {
        token[i-1] = token[i];
      }
      token[strlen(token) - 1] = '\0';
      
      // Isolate the word following the redirect char
      if (strstr(token, " ") != NULL) { token = strtok(token, " "); }

      // Trim trailing newline if one exists
      i = strlen(token) - 1;
      if (token[i] == '\n') { token[i] = '\0'; }

      // DEV
      // fprintf(stderr, "Output Redirect: %s\n", token);
      // fflush(stderr);

      // Open output file
      outf = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (outf == -1) {
        fprintf(stderr, "smallsh: %s: %s\n", strerror(errno), token);
        fflush(stderr);
        // Indicate problem with output redir to the caller
        if (status != 0) { status = 2; } else { status = 1; }
      } else {
        dup2(outf, 1);
      }
    }

    // Clean up
    free(buffer);

    return status;
}

// Parses command string and runs exec()
void _execute_cmd(char* cmd) {
    int i, n_tok, tok_size = 10;
    char *buffer, **tokens, *target;

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
        if (   strstr(tokens[i], "<") != NULL
            || strstr(tokens[i], ">") != NULL
            || strstr(tokens[i], "&") != NULL
        ) {
            tokens[i] = NULL;       // Overwrite this token
            strtok(NULL, " ");      // Skip the next one
        } else {
            i++;
            n_tok = i;
        }

        // Don't run out of space in tokens array!
        if (i >= tok_size && tok_size <= NARG_MAX) {
            // Don't allocate space for more than 512 args
            tok_size = (tok_size * 2) % NARG_MAX;
            tokens = (char **)realloc(tokens, (size_t)tok_size * sizeof(char*));
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

    // Trim trailing newline from last token
    target = strstr(tokens[i - 1], "\n");
    if (target != NULL) { memset(target, '\0', sizeof(char)); }

    /* DEV print each token */
    // for (i = 0; i < n_tok; i++) {
    //     printf("%s\n", tokens[i]);
    // }

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
