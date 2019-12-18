#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
/*Read and get data from rooms.txt*/
int HandleRoomsFile(char *dir, Room *room_arr)
{
	char *rooms_file_path, room_file_line[MAX_LINE_LEN], *line_res;// *final_filepath, *student_dir;
	int room_price, room_capacitance, i, num_of_rooms = 0;
	FILE *fp_rooms;
	/* allocate memory for strings and create the filepath*/
	AllocateString(&rooms_file_path, (strlen(dir) + strlen(ROOMS_TXT_FILE) + 2)); //malloc, must free!
	MergeStrings(rooms_file_path, dir, ROOMS_TXT_FILE);
	if (NULL == (fp_rooms = fopen(rooms_file_path, "r")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE); // DEBUG EXIT
	}
	while (fgets(room_file_line, MAX_LINE_LEN, fp_rooms)) // get ids from studentsIds.txt and call son process 
	{
		num_of_rooms = num_of_rooms + 1;
		for (i = 0; room_file_line[i] != ' '; i++);
		room_price = (int)strtol((room_file_line+i), &line_res, 10);
		room_capacitance = (int)strtol(line_res, &line_res, 10);
		room_arr->price = room_price;
		room_arr->capacity = room_capacitance;
		printf("PRICE = %d\n", room_arr->price);
		printf("CAP = %d\n", room_arr->capacity);
		strtok(room_file_line, " ");
		strcpy(room_arr->name, room_file_line);
		printf("ROOM NAME = %s\n", room_arr->name);
		room_arr++;
	}
	free(rooms_file_path);
	fclose(fp_rooms);
	return 0;
}
/*Read and get data from names.txt*/
int HandleNamesFile(char *dir, Guest *guest_arr)
{
	char *names_file_path, names_file_line[MAX_LINE_LEN];
	int guest_budget, i, num_of_guests = 0;
	FILE *fp_names;
	/* allocate memory for strings and create the filepath*/
	AllocateString(&names_file_path, (strlen(dir) + strlen(NAMES_TXT_FILE) + 2)); //malloc, must free!
	MergeStrings(names_file_path, dir, NAMES_TXT_FILE);
	if (NULL == (fp_names = fopen(names_file_path, "r")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE); // DEBUG EXIT
	}
	while (fgets(names_file_line, MAX_LINE_LEN, fp_names)) // get ids from studentsIds.txt and call son process 
	{
		num_of_guests = num_of_guests + 1;
		for (i = 0; names_file_line[i] != ' '; i++);
		guest_budget = (int)atol((names_file_line + i));
		strtok(names_file_line, " ");
		strcpy(guest_arr->name, names_file_line);
		guest_arr->budget = guest_budget;
		printf("BUDGET = %d\n", guest_arr->budget);
		printf("GUEST NAME = %s\n", guest_arr->name);
		guest_arr++;
	}
	free(names_file_path);
	fclose(fp_names);
	return 0;
}
int AllocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		return(ERROR_CODE);
	}
	return 0;
}
int MergeStrings(char *target, char *first, char *second)
{
	strcpy(target, first);
	strcat(target, second);
	return 0;
}
int WriteRoomLog(char *room, char *name, char *in_or_out, int day, char *dir)
{
	char line_to_file[MAX_LINE_LEN]; // DEBUG
	char day_str[MAX_LINE_LEN], *roomlog_file_path;
	FILE * fp_roomlog;
	sprintf(day_str, "%d", day);
	strcpy(line_to_file, room);
	strcat(line_to_file, " ");
	strcat(line_to_file, name);
	strcat(line_to_file, " ");
	strcat(line_to_file, in_or_out);
	strcat(line_to_file, " ");
	strcat(line_to_file, day_str);
	AllocateString(&roomlog_file_path, (strlen(dir) + strlen(ROOMS_TXT_FILE) + 2)); //malloc, must free!
	MergeStrings(roomlog_file_path, dir, LOG_FILE);
	if (NULL == (fp_roomlog = fopen(roomlog_file_path, "w")))
	{
		printf("File ERROR\n");
		exit(ERROR_CODE);
	}
	fprintf(fp_roomlog ,"%s\n", line_to_file);
	free(roomlog_file_path);
	fclose(fp_roomlog);
}