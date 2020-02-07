/****************************************************************************
 * NAME                                                                     *
 *    adventure                                                             *
 *                                                                          *
 * DESCRIPTION                                                              *
 *    Runs a text adventure game based on room files in the most recently   *
 *    created directory colbertz.room.* in the current directory.           *
 *                                                                          *
 * AUTHOR                                                                   *
 *    Zach Colbert (colbertz@oregonstate.edu)                               *
 *                                                                          *
 * NOTES                                                                    *
 *    For CS344, W2020 (online)                                             *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>

#define ROOM_COUNT 7
#define NAME_MAX_LENGTH 8
#define TYPE_MAX_LENGTH 10
#define CONNECT_OUT_MAX 6
#define HIST_MAX 100

// room struct holds all properties of a single room
struct room {
  int id;
  char name[NAME_MAX_LENGTH + 1];
  char type[TYPE_MAX_LENGTH + 1];
  int pathcount;
  char paths[CONNECT_OUT_MAX][NAME_MAX_LENGTH + 1];
};

/***********************
 * METHOD DECLARATIONS *
 ***********************/

// Import room data from file and allocate a new room struct to hold it
struct room* room_import(char*);

// Find the most recently created room dir and return it's name
char* roomdir_open();

// Imports all rooms from most recent room dir
void roomdir_import(struct room**);

// Prints location to console and prompts user for input
char* ui_prompt(struct room*);

// Return 1 if the game is over, 0 if not
int game_checkEnd(struct room*);

// Print game-ending stuff
void game_end(int, struct room**);


/****************
 * MAIN ROUTINE *
 ****************/

int main() {
  struct room *rooms[ROOM_COUNT], *hist[HIST_MAX], *current, *next = NULL;
  char *response;
  int i, steps = 0;

  // Read room data in from most recent rooms directory
  roomdir_import(rooms);

  // Set current location to start room
  for (i = 0; i < ROOM_COUNT; i++) {
    if (strcmp(rooms[i]->type, "START_ROOM") == 0) {
      current = rooms[i];
    }
  }

  // Loop, prompting user for input until game end
  while (game_checkEnd(current) == 0) {
    // Print user prompt and get response
    response = ui_prompt(current);

    // @TODO: Match time command first!

    // Try matching user response to one of the room names
    // NOTE: strcmp is case-sensitive!
    for (i = 0; i < ROOM_COUNT; i++) {
      if (strcmp(rooms[i]->name, response) == 0) {
        next = rooms[i];
      }
    }

    // Deallocate the response, now we're done with it
    free(response);
    response = NULL;

    // If no matching room was found, print an error and loop again
    if (next == NULL) {
      printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
      continue;
    }

    // Otherwise, move to the selected room
    hist[steps] = next;   // append the next room to history
    steps++;              // increment step count
    current = next;       // move current location pointer
    next = NULL;          // clear the next pointer before looping
  }

  // When the loop breaks, we've reached the end of the game
  game_end(steps, hist);

  return 0;
}


/**********************
 * METHOD DEFINITIONS *
 **********************/

/******************************************************************************
 * Import room data from file and allocate a new room struct to hold the data.
 *
 * PARAMS
 *    char* fpath - Relative path to room file.
 *
 * RETURNS
 *    struct room* - New room struct, with members initialized.
 *
 * PRECONDITIONS
 *    File needs to be readable.
 ******************************************************************************/
struct room* room_import(char* fpath) {
  const int BUFFER_SIZE = 25;
  int i;
  FILE* fptr;
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  struct room* r;

  // Initialize read buffer
  memset(buffer, '\0', BUFFER_SIZE * sizeof(char));

  // Open the file for reading
  fptr = fopen(fpath, "r");

  // Stop if unable to open file
  if (fptr == NULL) { 
    fprintf(stderr, "File %s is unreadable. Exiting...\n", fpath);
    exit(1);
  }

  // Allocate new room struct, initialize members
  r = malloc(sizeof(struct room));
  r->id = -1;
  memset(r->name, '\0', (NAME_MAX_LENGTH + 1) * sizeof(char));
  memset(r->type, '\0', (TYPE_MAX_LENGTH + 1) * sizeof(char));
  r->pathcount = 0;
  for (i = 0; i < CONNECT_OUT_MAX; i++) { 
    memset(r->paths[i], '\0', NAME_MAX_LENGTH + 1);
  }

  // Room name
  fscanf(fptr, "ROOM NAME: %s", buffer);    // Read name from file
  strcpy(r->name, buffer);                  // Copy name from buffer to struct
  memset(buffer, '\0', strlen(buffer));

  // Room connections
  fscanf(fptr, "%s %*s %s", buffer2, buffer);
  while (strcmp(buffer2, "CONNECTION") == 0) {
    strcpy(r->paths[r->pathcount], buffer);  // cpy buffer to paths list
    r->pathcount++;                          // track number of paths
    fscanf(fptr, "%s %*s %s", buffer2, buffer);   // Read the next line
  }

  // Last line read does not start with connection
  // --> It must be the room type!
  strcpy(r->type, buffer);      // Copy buffer to room type member

  // Done! Close file
  fclose(fptr);

  return r;
}

/******************************************************************************
 * Find the most recently created room directory in the current working
 * directory and return it's name.
 *
 * PARAMS
 *    None
 *
 * RETURNS
 *    char* - String containing directory name.
 *
 * PRECONDITIONS
 *    At least one directory with name like colbertz.rooms.PID must exist.
 *    Directory must be readable.
 * 
 * NOTES
 *    Drew some great inspiration from course page 2.4 on directories,
 *    but did my best not to copy verbatim! At least I renamed some
 *    vars and walked through each line to make sure I know what's
 *    going on.
 ******************************************************************************/
char* roomdir_open() {
  DIR* cwd;
  struct dirent *fentry;
  char target[] = "colbertz.rooms.";
  struct stat atts;
  int latestTime = -1;
  char latestName[256], *result;
  memset(latestName, '\0', sizeof(target));

  // Open current directory
  cwd = opendir(".");

  // Stop if directory can't be opened
  if (cwd == NULL) {
    fprintf(stderr, "Directory '.' is unreadable. Exiting...\n");
    exit(1);
  }

  // For each item in the directory
  while ((fentry = readdir(cwd)) != NULL) {
    // If the entry's name matches the target name
    if (strstr(fentry->d_name, target) != NULL) {
      // Save dir attributes
      stat(fentry->d_name, &atts);
      
      // If this dir is newer than the last
      if ((int)atts.st_mtime > latestTime) {
        // Replace the last newest dir with this one
        latestTime = (int)atts.st_mtime;
        memset(latestName, '\0', sizeof(latestName));
        strcpy(latestName, fentry->d_name);
      }
    }
  }

  // Close the current directory
  closedir(cwd);

  // If no directory was found, stop
  if (latestTime == -1) {
    fprintf(stderr, "No valid room directories found. Exiting...\n");
    exit(1);
  }

  // Allocate space on the heap for return value
  result = malloc(strlen(latestName) + 1);

  // Copy directory name to heap location
  strcpy(result, latestName);

  return result;
}

/******************************************************************************
 * Opens the most recent room directory in the current directory, and
 * imports each room file to a new room struct. New structs are added
 * to the given array.
 *
 * PARAMS
 *    struct room** arr - Array of room pointers in which to store new rooms
 *
 * RETURNS
 *    None
 *
 * PRECONDITIONS
 *    Directory stream must be open for reading.
 *    Number of rooms in directory must be exactly ROOM_COUNT.
 *    To prevent overflow, this method will only import that many rooms.
 ******************************************************************************/
void roomdir_import(struct room** arr) {
  char* dname = roomdir_open();
  DIR* ds;
  struct dirent *f;
  char path[100];
  int i = 0;

  // Open room directory
  ds = opendir(dname);

  if (ds == NULL) {
    fprintf(stderr, "Directory %s is unreadable. Exiting...\n", dname);
    exit(1);
  }

  // For each room file in the directory
  while ((f = readdir(ds)) != NULL) {
    if (strstr(f->d_name, ".rm") != NULL) {
      // Concat dir name and file name
      memset(path, '\0', sizeof(path));
      sprintf(path, "%s/%s", dname, f->d_name);

      // Import room file to struct, add to array
      arr[i] = room_import(path);

      // Advance to next array index
      i++;
    }
  }

  // Clean up
  closedir(ds);
  free(dname);
}

/******************************************************************************
 * Prints location to console and prompts user for input
 *
 * PARAMS
 *    struct room* loc - Current player location
 *
 * RETURNS
 *    char* - C string containing user response
 *
 * PRECONDITIONS
 *    resp must be mutable.
 *    loc must not be NULL
 ******************************************************************************/
char* ui_prompt(struct room* loc) {
  assert(loc);

  int i = 0;
  size_t buffer_size = 0;
  char* buffer = NULL;
  
  // Print player info
  printf("\nCURRENT LOCATION: %s\n", loc->name);
  printf("POSSIBLE CONNECTIONS: %s", loc->paths[i]);
  for (i = 1; i < loc->pathcount; i++) {
    printf(", %s", loc->paths[i]);
  }
  printf(".\n");

  // Prompt for player input
  printf("WHERE TO? >");
  i = getline(&buffer, &buffer_size, stdin);

  // Remove newline from the response
  if (buffer[i - 1] == '\n') {
    buffer[i - 1] = '\0';
  }

  // Point to getline's buffer
  return buffer;
}

/******************************************************************************
 * Return 1 if the game is over, 0 if not.
 *
 * PARAMS
 *    struct room* loc - Current player location.
 *
 * RETURNS
 *    int - 1 if the game is over, 0 otherwise.
 *
 * PRECONDITIONS
 *    loc must not be NULL.
 ******************************************************************************/
int game_checkEnd(struct room* loc) {
  assert(loc);

  // If room type is END_ROOM, the game is over
  if (strcmp(loc->type, "END_ROOM") == 0) {
    return 1;
  }

  return 0;
}

/******************************************************************************
 * Print game-ending stuff.
 *
 * PARAMS
 *    int n - Number of steps the player took during the game.
 *    struct room** hist - Player's path history. Array of room pointers.
 *
 * RETURNS
 *    None
 *
 * PRECONDITIONS
 *    hist must have size n.
 *    Pointers in hist must not be NULL.
 ******************************************************************************/
void game_end(int n, struct room** hist) {
  int i;

  // print congratulations
  printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");

  // print step count
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", n);

  // print list of rooms
  for (i = 0; i < n; i++) {
    printf("%s\n", hist[i]->name);
  }
}

