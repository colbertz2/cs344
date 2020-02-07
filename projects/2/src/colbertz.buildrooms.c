/****************************************************************************
 * NAME                                                                     *
 *    buildrooms                                                            *
 *                                                                          *
 * DESCRIPTION                                                              *
 *    Builds rooms for the adventure program and saves them in the current  *
 *    working directory under colbertz.rooms.[PID]                          *
 *                                                                          *
 * AUTHOR                                                                   *
 *    Zach Colbert (colbertz@oregonstate.edu)                               *
 *                                                                          *
 * NOTES                                                                    *
 *    For CS344, W2020 (online)                                             *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>


/***********************
 * STRUCTS / CONSTANTS *
 ***********************/

// Total number of rooms to allocate
#define ROOM_COUNT 7

// Number of possible room names (elements in ROOM_NAMESPACE)
#define NAMESPACE_SIZE 10

// Max length of room names
#define NAME_MAX_LENGTH 8

// Max length of room type string
#define TYPE_MAX_LENGTH 10

// Max number of connections
#define CONNECT_OUT_MAX 6
#define CONNECT_IN_MAX 3

// room struct holds all properties of a single room
struct room {
  int id;
  char name[NAME_MAX_LENGTH + 1];
  char type[TYPE_MAX_LENGTH + 1];
  int pathcount;
  struct room* paths[CONNECT_OUT_MAX];
};

// List of all possible room names
char NAMESPACE[NAMESPACE_SIZE][NAME_MAX_LENGTH + 1] = {
  "study",
  "kitchen",
  "ballrm",
  "consvtry",
  "billrdrm",
  "library",
  "hall",
  "lounge",
  "diningrm",
  "cellar"
};

// Keep track of which names are already in use
int NAME_IN_USE[NAMESPACE_SIZE];

/***********************
 * METHOD DECLARATIONS *
 ***********************/


struct room* room_create();

// Deallocate room struct
void room_free(struct room*);

// Select a room from array by index
struct room* room_get(struct room**, int);

// Select a room at random from array
struct room* room_$get(struct room**);

// Return 1 if outbound connections are allowed for a room, 0 otherwise
int room_allowOutbound(struct room*);

// Return 1 if rooms are already connected, 0 otherwise
int room_isConnected(struct room*, struct room*);

// Connect two rooms (without validation)
void room_connect(struct room*, struct room*);

// Return 1 if rooms are the same (equality comparison)
int room_cmp(struct room*, struct room*);

// Given a file descriptor, write contents of struct to a room file
int room_export(struct room*, int);

// Return 1 if all rooms have 3-6 outbound connections, 0 otherwise
int map_isfull(struct room**);

// Loop until map is full, creating random room connections
void map_$fill(struct room**);

// Assign start, end, mid room types
void map_$setTypes(struct room**);


/****************
 * MAIN ROUTINE *
 ****************/

int main() {
  int i, fd;
  char fname[50];
  char dname[25] = "colbertz.rooms.";
  struct room *rooms[ROOM_COUNT], *r;

  srand(getpid());      // Seed random library with PID

  for (i = 0; i < ROOM_COUNT; i++) {
    r = room_create();
    r->id = i;
    rooms[i] = r;         // Add new room to array
  }

  map_$fill(rooms);       // Fill out map connections
  map_$setTypes(rooms);   // Set room types

  // Make directory for room files
  sprintf(dname, "colbertz.rooms.%d", getpid());
  mkdir(dname, 0755);

  // Create files and write room contents out
  for (i = 0; i < ROOM_COUNT; i++) {
    r = rooms[i];
    
    // Create empty file
    sprintf(fname, "%s/%s.rm", dname, r->name);
    fd = open(fname, O_CREAT | O_WRONLY, 0644);

    // Write struct data to file
    room_export(r, fd);

    // Close file and free struct
    close(fd);
  }

  // Free rooms in memory
  for (i = 0; i < ROOM_COUNT; i++) { room_free(rooms[i]); }

  return 0;
}


/**********************
 * METHOD DEFINITIONS *
 **********************/

/*****************************************************************************
 * Allocate a room struct and assign it a name from global NAMESPACE.
 * 
 * PARAMS
 *    None
 * 
 * RETURNS
 *    struct room* - pointer to new room on the heap
 * 
 * PRECONDITIONS
 *    Run srand() in the main function before calling this method.
 *    Array of strings NAMESPACE must be available globally.
 *****************************************************************************/
struct room* room_create() {
  struct room* r;
  int n;

  // Allocate heap space for new room
  r = malloc(sizeof(struct room));

  // Initialize everything but the name property
  r->id = -1;
  r->pathcount = 0;
  memset(r->name, '\0', NAME_MAX_LENGTH + 1);
  memset(r->type, '\0', TYPE_MAX_LENGTH + 1);
  for (n = 0; n < CONNECT_OUT_MAX; n++) { r->paths[n] = NULL; }

  /* RANDOMLY SELECT A ROOM NAME FROM LIST */
  do {
    n = rand() % NAMESPACE_SIZE;    // random int from 0 to 9
  } while (NAME_IN_USE[n] == 1);    // Get another if this one is in use

  strcpy(r->name, NAMESPACE[n]);    // Set name property of struct
  NAME_IN_USE[n] = 1;               // Mark this name as in-use

  return r;
}

/*****************************************************************************
 * Deallocate room struct. Does not free any connected rooms, just this one.
 * 
 * PARAMS
 *    struct room* r - pointer to room to free in memory
 * 
 * RETURNS
 *    None
 * 
 * PRECONDITIONS
 *    None
 *****************************************************************************/
void room_free(struct room* r) {
  // Simple solutions are best!
  free(r);
}

/*****************************************************************************
 * Select a room from array by index.
 * 
 * PARAMS
 *    struct room** arr - pointer to array of room pointers
 *    int i - index of array element
 * 
 * RETURNS
 *    struct room* - pointer to room at index i
 * 
 * PRECONDITIONS
 *    Size of arr is equal to ROOM_COUNT. Index boundary will only be enforced
 *    to less than that number, not actual size of array.
 *****************************************************************************/
struct room* room_get(struct room** arr, int i) {
  assert(i < ROOM_COUNT);
  return arr[i];
}

/*****************************************************************************
 * Select a room at random from array.
 * 
 * PARAMS
 *    struct room ** arr - pointer to array of room pointers
 * 
 * RETURNS
 *    struct room* - pointer to room at random index
 * 
 * PRECONDITIONS
 *    Size of arr is equal to ROOM_COUNT.
 *    Run srand() in main function before calling this method.
 *****************************************************************************/
struct room* room_$get(struct room** arr) {
  int i = rand() % ROOM_COUNT;    // Random int from 0 to 6
  return arr[i];
}

/*****************************************************************************
 * Return 1 if outbound connections are allowed for a room, 0 otherwise.
 * i.e. if number of outbound connections is less than 6, then return true.
 * 
 * PARAMS
 *    struct room* r - Room on which to check connections
 * 
 * RETURNS
 *    int - 1 if outbound connections are allowed, 0 if not
 * 
 * PRECONDITIONS
 *    r must not be NULL
 *****************************************************************************/
int room_allowOutbound(struct room* r) {
  assert(r);
  if (r->pathcount < 6) { return 1; }
  return 0;
}

/*****************************************************************************
 * Return 1 if rooms are already connected, 0 otherwise
 * 
 * PARAMS
 *    struct room *a, *b - Pointers to rooms that should be checked.
 * 
 * RETURNS
 *    int - 1 if connected, 0 if not
 * 
 * PRECONDITIONS
 *    None
 *****************************************************************************/
int room_isConnected(struct room* a, struct room* b) {
  int i;

  // Loop through paths on a
  for (i = 0; i < a->pathcount; i++) {
    // If path pointer from a is equal to b, return 1
    if (room_cmp(a->paths[i], b)) { return 1; }
  }

  return 0;
}

/*****************************************************************************
 * Connect two rooms (without validation). Ensures mutual connection between
 * rooms and increments connection counter for each room object.
 * 
 * PARAMS
 *    struct room *a, *b - Pointers to rooms that should be connected.
 * 
 * RETURNS
 *    None
 * 
 * PRECONDITIONS
 *    Rooms should not already be connected.
 *    Check to see if outbound connections are allowed before calling.
 *    a and b must not be NULL.
 *****************************************************************************/
void room_connect(struct room* a, struct room* b) {
  assert(a);
  assert(b);

  // Draw a connection from a to b
  a->paths[a->pathcount] = b;
  a->pathcount++;

  // Draw a connection from b to a
  b->paths[b->pathcount] = a;
  b->pathcount++;
}

/*****************************************************************************
 * Return 1 if rooms are the same (equality comparison).
 * 
 * PARAMS
 *    struct room *a, *b - Rooms to be compared
 * 
 * RETURNS
 *    int - 1 if rooms are the same, 0 if not
 * 
 * PRECONDITIONS
 *    a,b must not be NULL
 *****************************************************************************/
int room_cmp(struct room* a, struct room* b) {
  assert(a);
  assert(b);

  int ti, tn, tt;

  // Simple test: If pointers are the same, return 1
  if (a == b) { return 1; }

  // If other properties match, return 1
  ti = a->id == b->id;
  if (strcmp(a->name, b->name) == 0) { tn = 1; }
  if (strcmp(a->type, b->type) == 0) { tt = 1; }

  if (ti && tn && tt) { return 1; }

  return 0;
}

/*****************************************************************************
 * Given a file descriptor, write contents of struct to a room file.
 * 
 * PARAMS
 *    struct room* r - Room from which properties should be written.
 *    int - C file descriptor
 * 
 * RETURNS
 *    int - file descriptor after writing
 * 
 * PRECONDITIONS
 *    Properties of room struct must be initialized.
 *    File descriptor must be open for writing.
 *    r must not be NULL
 *****************************************************************************/
int room_export(struct room* r, int fd) {
  assert(r);

  int i, line_max = 14 + NAME_MAX_LENGTH + 1;
  char outstr[line_max];

  // Initialize write buffer
  memset(outstr, '\0', line_max * sizeof(char));

  // Write room name to file
  sprintf(outstr, "ROOM NAME: %s\n", r->name);
  write(fd, outstr, strlen(outstr) * sizeof(char));

  // Write room connections to file
  for (i = 0; i < r->pathcount; i++) {
    // Reinitialize buffer
    memset(outstr, '\0', strlen(outstr) * sizeof(char));

    sprintf(outstr, "CONNECTION %d: %s\n", i + 1, r->paths[i]->name);
    write(fd, outstr, strlen(outstr) * sizeof(char));
  }

  // Write room type to file
  memset(outstr, '\0', strlen(outstr) * sizeof(char));

  sprintf(outstr, "ROOM TYPE: %s\n", r->type);
  write(fd, outstr, strlen(outstr) * sizeof(char));

  return fd;
}

/*****************************************************************************
 * Return 1 if all rooms have 3-6 outbound connections, 0 otherwise.
 * 
 * PARAMS
 *    struct room** arr - Pointer to head of array containing room pointers
 * 
 * RETURNS
 *    int - 1 if all rooms have 3-6 outbound connections, 0 if not
 * 
 * PRECONDITIONS
 *    arr must have size ROOM_COUNT
 *****************************************************************************/
int map_isfull(struct room** arr) {
  struct room *r;
  int i, out, count = 0;
  char* warnstr = "WARNING: Room %d named %s has >6 connections!\n";

  for (i = 0; i < ROOM_COUNT; i++) {
    r = arr[i];
    out = r->pathcount;     // Number of outbound connections from this room

    // Warning for developer if too many connections
    if (out > 6) { fprintf(stderr, warnstr, r->id, r->name); }
    
    // Count all the rooms with req'd number of connections
    if (out > 3) { count++; }
  }

  if (count == ROOM_COUNT) { return 1; }

  return 0;
}

/*****************************************************************************
 * Loop until map is full, creating random room connections.
 * 
 * PARAMS
 *    struct room** arr - Pointer to head of array containing room pointers
 * 
 * RETURNS
 *    None
 * 
 * PRECONDITIONS
 *    arr must have size ROOM_COUNT.
 *    Run srand() in main function before calling this method.
 *****************************************************************************/
void map_$fill(struct room** arr) {
  struct room *A, *B;
  int x, y, z;

  // Loop until we've met minimum number of connections
  while (!map_isfull(arr)) {
    // Get a random room with connections allowed
    do {
      A = room_$get(arr);
    } while (!room_allowOutbound(A));

    // Get another random room with connections allowed
    // AND it's not the same room
    // AND there's no existing connection
    do {
      B = room_$get(arr);

      x = room_allowOutbound(B);
      y = room_cmp(A, B);
      z = room_isConnected(A, B);
    } while (!x || y || z);

    room_connect(A, B);   // Sets mutual connection between rooms
  }
}

/*****************************************************************************
 * Assign start, end, mid room types.
 * 
 * PARAMS
 *    struct room** arr - Pointer to head of array of room pointers.
 * 
 * RETURNS
 *    None
 * 
 * PRECONDITIONS
 *    arr must have size ROOM_COUNT.
 *    Run srand() in main function before calling this method.
 *****************************************************************************/
void map_$setTypes(struct room** arr) {
  int i, j, k;
  struct room* r;

  // Get random index in 0 to ROOM_COUNT - 1 for start_room
  i = rand() % ROOM_COUNT;

  // Get random index for end_room
  do {
    j = rand() % ROOM_COUNT;
  } while (i == j);

  // Loop through all rooms and set their type property
  for (k = 0; k < ROOM_COUNT; k++) {
    r = arr[k];

    if (k == i) { strcpy(r->type, "START_ROOM"); continue; }
    if (k == j) { strcpy(r->type, "END_ROOM"); continue; }

    strcpy(r->type, "MID_ROOM");
  }
}
