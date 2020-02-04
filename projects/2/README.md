# Project 2

Implement a text adventure game.

## Specifications
Source file: `colbertz.buildrooms.c`
Binary: `colbertz.buildrooms`

### Room Generator Program
1. Create a directory _colbertz.rooms.PID_
2. Generate 7 room files with hard-coded names
3. Populate room data to files

#### Room Specifications
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
