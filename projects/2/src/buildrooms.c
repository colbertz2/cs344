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
  "",
