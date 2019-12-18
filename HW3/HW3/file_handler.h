#ifndef _FILE_HANDLER_H
#define _FILE_HANDLER_H

int HandleRoomsFile(char *dir, Room *room_arr);

int HandleNamesFile(char *dir, Guest *guest_arr);

int AllocateString(char **str_ptr, int len);

int MergeStrings(char *target, char *first, char *second);

int WriteRoomLog(char *room, char *name, char *in_or_out, int day, char *dir);
//#define INPUT_FOLDER "C:\Users\roypa\OneDrive\Documents\GitHub\system_programming\HW3\inputs\"

#endif