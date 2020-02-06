# `adventure` Program

## Libraries / Includes
+ `stdio.h` : basic IO
+ `string.h` : string manipulation
+ `sys/types.h` : system data types
+ `dirent.h` : directory IO
+ `unistd.h` : POSIX and fs stuff
+ `sys/stat.h` : directory stats

## Global Vars / Const
+ `ROOM_COUNT` is the count of all rooms in map

## Main Routine Outline
### Vars to Declare
struct room** roomList
struct room* currentLocation
struct room** pathHistory
int stepCounter

## Structs and Enums
+ Use `room` struct from `buildrooms.c` (will have to make a copy of it here...)

## Methods

`struct room* room_import(char*)` : 
Import room data from file and allocate a new room struct to hold the data

`DIR* roomdir_open()` : 
Find the most recently created room directory in the current working directory,
and open it.

`void roomdir_import(DIR*, struct room**)` : 
Given an open dir stream, import each `.rm` file to a new room struct and add
room pointer to the given array.

`void ui_prompt(char*, struct room*)` : 
Prints information about the current location to console (from given room
struct), and prompts the user for input. Has no return value, but stores the
user input in the given `char*`.
Use `readline()`, but copy the string to the given location and free the memory
allocated by readline when finished!
[manual](https://linux.die.net/man/3/readline)

`int game_checkEnd(struct room*)` : 
Return 1 if the game is over, 0 if not. Takes pointer to current location room.

`void game_end(int, struct room**)` : 
Print game ending stuff. Takes int number of steps, pointer to pathHistory.

