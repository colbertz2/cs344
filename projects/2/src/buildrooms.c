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
#include <unistd.h>


/***********************
 * STRUCTS / CONSTANTS *
 ***********************/

// Total number of rooms to allocate
#define ROOM_COUNT 7

// Number of possible room names (elements in ROOM_NAMESPACE)
#define NAMESPACE_SIZE 10

// Max length of room names
#define NAME_MAX_LENGTH 8

// room struct holds all properties of a single room
struct room {
  int id;
  char* name;
  char* type;
  int pathcount;
  struct room* paths[6]
};

// List of all possible room names
char NAMESPACE[NAMESPACE_SIZE][NAME_MAX_LENGTH] = {
  "study",
  "kitchen",
  "ballrm",
  "conserv",
  "bllrdrm",
  "library",
  "hall",
  "lounge",
  "dinerm",
  "cellar"
};


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
void room_export(struct room*, int);

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
  // Coming soon!
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
  //
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
  //
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
  //
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
  //
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
 *    None
 *****************************************************************************/
int room_allowOutbound(struct room* r) {
  //
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
  //
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
 *****************************************************************************/
void room_connect(struct room* a, struct room* b) {
  //
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
 *    None
 *****************************************************************************/
int room_cmp(struct room* a, struct room* b) {
}

/*****************************************************************************
 * Given a file descriptor, write contents of struct to a room file.
 * 
 * PARAMS
 *    struct room* r - Room from which properties should be written.
 *    int - C file descriptor
 * 
 * RETURNS
 *    None
 * 
 * PRECONDITIONS
 *    Properties of room struct must be initialized.
 *    File descriptor must be open for writing.
 *****************************************************************************/
void room_export(struct room* r, int fd) {
  //
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
  //
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
  //
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
}