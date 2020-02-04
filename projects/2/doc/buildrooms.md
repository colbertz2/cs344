# `buildrooms` Program

See program specs [in the readme](../README.md#room-generator-program).

## Main Routine Outline
1. Allocate 7 room structs and loop through them, randomly assigning a name from the name list to each one
2. Make room connections (use model from [Canvas](https://oregonstate.instructure.com/courses/1780106/pages/2-dot-2-program-outlining-in-program-2))
3. Room type assignment
    1. Randomly select an int 0-7, assign that index in room array START
    2. Randomly select an int 0-7 (not same as before), assign that index in room array END
    3. Assign all other rooms MID type
4. Create directory `colbertz.rooms.PID` with permissions 755
5. File creation loop (x7)
    1. Grab room i from structs
    2. Create empty file with permissions 644
    3. Write data from struct to file
5. Close all files and free heap memory

## Structs / Enums

Contains all the data about a room that will need to be written to a file.
### struct room
+ `int id`
+ `char* name`
+ `char* type`
+ `int pathcount`
+ `struct room* paths[6]`

## Methods/Subroutines
### room Struct Methods
Allocate a room struct and assign it a name
`struct room* room_create()`

Deallocate room struct
`void room_free(struct room*)`

Select a room by index
`struct room* room_get(int)`

Select a room at random
`struct room* room_$get()`

Return 1 if outbound connections are allowed for a room, 0 otherwise
`int room_allowOutbound(struct room*)`

Return 1 if rooms are already connected, 0 otherwise
`int room_isConnected(struct room*, struct room*)`

Connect two rooms (without validation)
`void room_connect(struct room*, struct room*)`

Return 1 if rooms are the same (comparison function)
`void room_cmp(struct room*, struct room*)`

Given a file descriptor, write contents of struct to a room file
`void room_export(file_descriptor, struct room*)`


### Mapping
Return 1 if all rooms have 3-6 outbound connections, 0 otherwise
`int map_isfull(struct room*)`

Loop until map is full, creating random room connections
`void map_$fill()`

Assign start, end, mid type rooms
`void map_$settypes()`