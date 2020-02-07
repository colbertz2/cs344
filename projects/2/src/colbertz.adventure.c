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

// room struct holds all properties of a single room
struct room {
  int id;
  char name[NAME_MAX_LENGTH + 1];
  char type[TYPE_MAX_LENGTH + 1];
  int pathcount;
  char* paths[CONNECT_OUT_MAX];
};

/***********************
 * METHOD DECLARATIONS *
 ***********************/

// Import room data from file and allocate a new room struct to hold it
struct room* room_import(char*);

// Find the most recently created room dir and open it
DIR* roomdir_open();

// Imports all rooms from most recent room dir
void roomdir_import(struct room**);

// Prints location to console and prompts user for input
void ui_prompt(char*, struct room*);

// Return 1 if the game is over, 0 if not
int game_checkEnd(struct room*);

// Print game-ending stuff
void game_end(int, struct room**);


/****************
 * MAIN ROUTINE *
 ****************/

int main() {
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
  for (i = 0; i < CONNECT_OUT_MAX; i++) { r->paths[i] = NULL; }

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
 * directory and open it.
 *
 * PARAMS
 *    None
 *
 * RETURNS
 *    DIR* - Open directory stream.
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
DIR* roomdir_open() {
  DIR* cwd;
  struct dirent *fentry;
  char target[] = "colbertz.rooms.";
  struct stat atts;
  int latestTime = -1;
  char latestName[256];
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

  // Open the latest rooms directory
  cwd = opendir(latestName);
  
  // Stop if directory can't be opened
  if (cwd == NULL) {
    fprintf(stderr, "Directory %s is unreadable. Exiting...", latestName);
    exit(1);
  }

  // Return open DIR
  return cwd;
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
  DIR* ds = roomdir_open();
  struct dirent *f;
  int i = 0;

  // For each room file in the directory
  while ((f = readdir(ds)) != NULL) {
    // Import room file to struct, add to array
    arr[i] = room_import(f->d_name);

    // Advance to next array index
    i++;
  }
}

/******************************************************************************
 * Prints location to console and prompts user for input
 *
 * PARAMS
 *    char *resp - C string to store user's response in
 *    struct room* loc - Current player location
 *
 * RETURNS
 *    None
 *    (Stores player response in given char* memory)
 *
 * PRECONDITIONS
 *    resp must be mutable.
 *    loc must not be NULL
 ******************************************************************************/
void ui_prompt(char* resp, struct room* loc) {
  assert(loc);

  int i = 0;
  size_t buffer_size = 0;
  char* buffer = NULL;
  
  // Print player info
  printf("CURRENT LOCATION: %s\n", loc->name);
  printf("POSSIBLE CONNECTIONS: %s", loc->paths[i]);
  for (i = 1; i < loc->pathcount; i++) {
    printf(", %s", loc->paths[i]);
  }
  printf(".\n");

  // Prompt for player input
  printf("WHERE TO? >");
  getline(&buffer, &buffer_size, stdin);

  // Point to getline's buffer
  resp = buffer;
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
  printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!");

  // print step count
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", n);

  // print list of rooms
  for (i = 0; i < n; i++) {
    printf("%s\n", hist[i]->name);
  }
}

