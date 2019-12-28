#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "thread_handler.h"
#include "guest_thread.h"
#include "file_handler.h"

/*Read and get data from rooms.txt*/
int LoadRoomList(char *dir, int *rooms_number, Room_t *room_arr)
{
	int ret_val = SUCCESS;
	char *rooms_file_path, room_file_line[MAX_LINE_LEN], *line_res;// *final_filepath, *student_dir;
	int room_price, room_capacitance, i, num_of_rooms = 0;
	FILE *fp_rooms = NULL;
	/* allocate memory for strings and create the filepath*/
	ret_val = AllocateString(&rooms_file_path, (strlen(dir) + strlen(ROOMS_TXT_FILE) + 2)); //malloc, must free!
	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!");
	MergeStrings(rooms_file_path, dir, ROOMS_TXT_FILE);
	if (NULL == (fp_rooms = fopen(rooms_file_path, "r")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	while (fgets(room_file_line, MAX_LINE_LEN, fp_rooms)) // get ids from studentsIds.txt and call son process 
	{
		num_of_rooms = num_of_rooms + 1;
		for (i = 0; room_file_line[i] != ' '; i++);
		room_price = (int)strtol((room_file_line+i), &line_res, 10);
		room_capacitance = (int)strtol(line_res, &line_res, 10);
		room_arr->price = room_price;
		room_arr->capacity = room_capacitance;
		strtok(room_file_line, " ");
		strcpy(room_arr->name, room_file_line);

		if (NULL == (room_arr->room_mutex = CreateMutexSimple())) {
			printf("Error when creating Mutex: %d\n", GetLastError());
			ret_val = MUTEX_CREATE_FAILED;
			goto EXIT;
		}
		strcpy_s(room_file_line, MAX_LINE_LEN, "");
		room_arr++;
	}
EXIT:
	if (NULL != rooms_file_path)
	{
		free(rooms_file_path);
	}
	if (NULL != fp_rooms)
	{
		fclose(fp_rooms);
	}
	*rooms_number = num_of_rooms;
	return ret_val;
}
/*Read and get data from names.txt*/
int LoadGuestList(char *dir, int *guests_number, Guest_t *guests_arr)
{
	char *names_file_path, names_file_line[MAX_LINE_LEN];
	int guest_budget, i, num_of_guests = 0;
	int ret_val = SUCCESS;
	FILE *fp_names = NULL;
	/* allocate memory for strings and create the filepath*/
	ret_val = AllocateString(&names_file_path, (strlen(dir) + strlen(NAMES_TXT_FILE) + 2)); //malloc, must free!
	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!");
	MergeStrings(names_file_path, dir, NAMES_TXT_FILE);
	if (NULL == (fp_names = fopen(names_file_path, "r")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	
	}
	while (fgets(names_file_line, MAX_LINE_LEN, fp_names)) // get ids from studentsIds.txt and call son process 
	{
		num_of_guests = num_of_guests + 1;
		for (i = 0; names_file_line[i] != ' '; i++);
		guest_budget = (int)atol((names_file_line + i));
		strtok(names_file_line, " ");
		guests_arr->budget = guest_budget;
		strcpy(guests_arr->name, names_file_line);
		guests_arr++;
		strcpy_s(names_file_line, MAX_LINE_LEN, "");
	}
EXIT:
	if (NULL != names_file_path)
	{
		free(names_file_path);
	}
	if (NULL != fp_names)
	{
		fclose(fp_names);
	}
	*guests_number = num_of_guests;
	return ret_val;
}

/*Function for malloc*/
int AllocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		return(ERROR_CODE);
	}
	return SUCCESS;
}

/*Mergin 2 strings function for filapaths*/
int MergeStrings(char *target, char *first, char *second)
{
	strcpy(target, first);
	strcat(target, second);
	return SUCCESS;
}

/*Function for opening RoomLog file for write*/
int OpenLogFile(FILE ** fp_roomlog, char *dir)
{
	char *roomlog_file_path;
	int ret_val = SUCCESS;
	ret_val = AllocateString(&roomlog_file_path, (strlen(dir) + strlen(LOG_FILE) + 2)); //malloc, must free!
	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!");
	MergeStrings(roomlog_file_path, dir, LOG_FILE);
	if (NULL == (*fp_roomlog = fopen(roomlog_file_path, "w")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		//return FILE_ERROR;
	}
EXIT:
	if (NULL != roomlog_file_path) {
		free(roomlog_file_path);
	}
	return ret_val;
}

/*Writing into RoomLog file Function*/
int WriteLog(char *room, char *name, char *in_or_out, int day, FILE *fp)
{
	char line_to_file[MAX_LINE_LEN];
	char day_str[MAX_LINE_LEN];
	sprintf(day_str, "%d", day);
	strcpy(line_to_file, room);
	strcat(line_to_file, " ");
	strcat(line_to_file, name);
	strcat(line_to_file, " ");
	strcat(line_to_file, in_or_out);
	strcat(line_to_file, " ");
	strcat(line_to_file, day_str);
	fprintf(fp, "%s\n", line_to_file);
	return SUCCESS;
}