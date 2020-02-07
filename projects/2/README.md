# Project 2

Implement a text adventure game.

## Room Generator Program
Source file: `colbertz.buildrooms.c`
Binary: `colbertz.buildrooms`

1. Create a directory _colbertz.rooms.PID_
2. Generate 7 room files with hard-coded names
3. Populate room data to files

[Notes about code planning and room generation](https://oregonstate.instructure.com/courses/1780106/pages/2-dot-2-program-outlining-in-program-2)

**Room Name**
+ Unique (cannot be assigned to more than one room)
+ Max 8 characters, alpha only
+ Randomly assigned from hard-coded list of length 10

**Room Type**
+ Possible values: `START_ROOM`, `END_ROOM`, `MID_ROOM`
+ Room type is randomly assigned
+ Only 1 room each can be assigned `START_ROOM` or `END_ROOM`
+ All rooms that are not `START_ROOM` or `END_ROOM` should be assigned `MID_ROOM`

**Connections to Other Rooms**
+ Each room has at least 3 outbound connections
+ Each room has at most 6 inbound connections
+ Connections are randomly assigned on room creation
+ Outbound connections must have a matching inbound connection
+ A room cannot connect to itself
+ A room cannot have more than two connections to the same room

##### Room File Format
```
ROOM NAME: RoomA
CONNECTION 1: RoomB
CONNECTION 2: RoomC
CONNECTION 3: RoomD
CONNECTION 4: RoomE
...
ROOM TYPE: START_ROOM
```

## Adventure Program
Source file: `colbertz.adventure.c`
Binary: `colbertz.adventure`

1. Read room data in from most recently created rooms directory
2. Present a player interface with elements:
  1. `CURRENT LOCATION: roomname`
  2. `POSSIBLE CONNECTIONS: room1, room2, room3.`
  3. Prompt `WHERE TO? >` (no space after `>`)
3. Read input from user
  + For _exact_ name of a connected room, write a newline, update location,
    update path history, update step count, and prompt again.
  + If name is not matched _exactly_ (case-sensitive), print an error line with
    newlines on either side. Do not increment step count.
4. On reaching the `END_ROOM`, stop the game. Print the following:
  1. `YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!`
  2. `YOU TOOK n STEPS. YOUR PATH TO VICTORY WAS:`
  3. List names of rooms on path, not including start room.
5. Exit with status 0

### Time Keeping
Goal: Return the current time of day using a second thread and mutexes.

When the time command is entered, second thread should write the current time to file `currentTime.txt` in the current directory. Main thread reads time from file, and prints it for the user in this format: `1:03pm, Tuesday, September 13, 2016`

Then, print out the prompt again (without location info). **Probably need to split `ui_prompt` into two different functions!

_Recommended practice:_ 
+ Start main thread by locking a mutex, then spawn second thread. 
+ Second thread should attempt to lock this mutex, which blocks it until the mutex is unlocked by main thread. 
+ Keep second thread running in the background, in waiting. When time command comes in, unlock the mutex and use `_join()` method to wait on second thread to complete.
+ Re-lock the mutex, and re-create second thread.