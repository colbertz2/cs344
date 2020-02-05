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
}


/***********************
 * METHOD DECLARATIONS *
 ***********************/

// Allocate a room struct and assign it a name
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
  return 0
}


/**********************
 * METHOD DEFINITIONS *
 **********************/

// Allocate a room struct and assign it a name
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