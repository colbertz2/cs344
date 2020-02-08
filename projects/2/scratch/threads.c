#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

char* ui_prompt();
void* time_routine(void *);

pthread_mutex_t mtx;

int main() {
    char* resp = NULL;
    pthread_t tt;

    // Init mutex and lock to block second thread
    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_lock(&mtx);

    // Start second thread for time command
    pthread_create(&tt, NULL, &time_routine, NULL);

    while (1) {
        resp = ui_prompt();

        if (strcmp(resp, "exit") == 0) {
            free(resp);
            break;
        } else if (strcmp(resp, "time") == 0) {
            free(resp);

            // Unlock mutex so thread can execute
            pthread_mutex_unlock(&mtx);

            // Block until thread completes
            pthread_join(tt, NULL);

            // Re-lock mutex and create a new thread
            pthread_mutex_lock(&mtx);
            pthread_create(&tt, NULL, &time_routine, NULL);

        } else {
            printf("%s\n\n", resp);
            free(resp);
            resp = NULL;
        }
    }

    // Clean up
    pthread_mutex_destroy(&mtx);
    pthread_cancel(tt);

    return 0;
}

char* ui_prompt() {
    int i = 0;
    size_t buffer_size = 0;
    char* buffer = NULL;

    printf(" >");
    i = getline(&buffer, &buffer_size, stdin);

    if (buffer[i-1] == '\n') { buffer[i-1] = '\0'; }

    return buffer;
}

void* time_routine(void *v) {
  int lock = -1;
  size_t strmax = 80;
  char fname[] = "currentTime.txt";
  char timestr[(int)strmax];
  FILE* fs;
  time_t utctime;
  struct tm *loctime;

  // Wait for mutex to unlock
  while (lock != 0) {
    sleep(0.010);
    lock = pthread_mutex_lock(&mtx);
  }

  fs = fopen(fname, "w+");

  time(&utctime);
  loctime = localtime(&utctime);

  memset(timestr, '\0', sizeof(timestr));
  strftime(timestr, strmax, "%l:%M%P, %A, %B %d, %Y", loctime);

  fprintf(fs, "%s\n", timestr);

  fclose(fs);
  pthread_mutex_unlock(&mtx);

  pthread_exit(0);
}

