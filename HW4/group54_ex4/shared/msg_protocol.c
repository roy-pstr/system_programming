#include "msg_protocol.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
/*Function for malloc*/
ErrorCode_t AllocateString(char **str_ptr, int len) {
	if (NULL == (*str_ptr = (char *)malloc(len)))
	{
		printf("Memory Allocation failed! Try again...\n");
		return(MALLOC_STRING_FAIL);
	}
	return SUCCESS;
}

void InitProtocol(protocol_t * msg)
{
	msg->type = ERROR_MSG_TYPE;
	int i = 0;
	for (; i < PROTOCOL_PARAM_LIST_SIZE; i++) /* initialize params to "" */
	{
		strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, "");
	}
	msg->size_in_bytes = 0;
}

void SetProtocol(protocol_t * msg, PROTOCOL_ENUM type, char **param_list, int param_list_size)
{
	InitProtocol(msg);
	msg->type = type;
	int i = 0;
	if (param_list != NULL) { 
		for (; i < param_list_size; i++) /* copy given parameters */
		{
			AddParam(param_list[i], msg);
		}
	}
	for (;i < PROTOCOL_PARAM_LIST_SIZE; i++) /* initialize the rest to "" */
	{
		strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, "");
	}

	msg->size_in_bytes = 0;
}

ErrorCode_t ParseMessage(char * msg_str, int msg_length, protocol_t * msg)
{
	/* must first initailze the protocol struct to make sure all params are initailized! */
	InitProtocol(msg);
	ErrorCode_t ret_val = SUCCESS;
	const char del[2] = ":";
	char *msg_cpy, *token = NULL;
	ret_val=AllocateString(&msg_cpy, msg_length);
	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!\n");
	strcpy_s(msg_cpy, msg_length, msg_str);
	token = strtok(msg_cpy, del);
	msg->type = FindType(token);
	if (msg->type == ERROR_MSG_TYPE) {
		ret_val = PROTOCOL_MSG_TYPE_ERROR;
		printf("The given protocol is message is illegal.\n");
		goto EXIT;
	}

	if (ShouldHaveParams(msg)) {
		token = strtok(NULL, del); /* look if there is params */
		if (token == NULL) {
			printf("error when trying to parse param_list.\n");
		}
		ret_val = ParseParams(token, msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "ParseParams() failed.\n");
	}
	
	msg->size_in_bytes = msg_length;
EXIT:
	if (msg_cpy != NULL) {
		free(msg_cpy);
	}
	return ret_val;
}

ErrorCode_t ProtocolToString(protocol_t * msg, char **msg_str)
{
	ErrorCode_t ret_val = SUCCESS;
	strcpy_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, PROTOCOLS_STRINGS[GetType(msg)]);
	if (ShouldHaveParams(msg)) {
		if (STRINGS_ARE_EQUAL(msg->param_list[0], "")) { /* no params!! */
				printf("No parameters passed for a protocol type: %s\n", PROTOCOLS_STRINGS[GetType(msg)]);
				return INVALID_MESSAGE_PROTOCOL;
		}
		strcat_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, ":"); /* start of param_list */
		if (GetType(msg)==SERVER_LEADERBOARD) {
			strcat_s(*msg_str, LEADERBOARD_STR_MAX_LEN, msg->leaderboard_param);
			strcat_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, "\n");
			return ret_val;
		}
		strcat_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, msg->param_list[0]);
		for (int i = 1; i < PROTOCOL_PARAM_LIST_SIZE; i++)
		{
			if (STRINGS_ARE_EQUAL(msg->param_list[i], "")) { /* end of param_list */
				break; }
			strcat_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, ";");
			strcat_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, msg->param_list[i]);
			
		}
		strcat_s(*msg_str, PROTOCOL_MESSAGE_MAX_LEN, "\n");
	}
	return ret_val;
}

PROTOCOL_ENUM GetType(protocol_t * msg)
{
	if (NULL != msg) {
		return msg->type;
	}
	return ERROR_MSG_TYPE;
}

PROTOCOL_ENUM FindType(char * type_str)
{
	
	if (NULL != type_str) {
		for (int i = 0; i < PROTOCOL_ENUM_LAST; i++)
		{
			if (STRINGS_ARE_EQUAL(type_str, PROTOCOLS_STRINGS[i])) {
				return (PROTOCOL_ENUM)i;
			}
		}
	}
	return ERROR_MSG_TYPE;
}

bool ShouldHaveParams(protocol_t * msg)
{
	switch (msg->type)
	{
	case CLIENT_REQUEST:
		return true;
	case CLIENT_PLAYER_MOVE:
		return true;
	case SERVER_DENIED:
		return true;
	case SERVER_INVITE:
		return true;
	case SERVER_GAME_RESULTS:
		return true;
	case SERVER_OPPONENT_QUIT:
		return true;
	case SERVER_LEADERBOARD:
		return true;
	default:
		return false;
	}
}

ErrorCode_t AddParam(char * param, protocol_t * msg)
{
	ErrorCode_t ret_val = SUCCESS;
	if (GetType(msg) == SERVER_LEADERBOARD) {
		strcpy_s(msg->leaderboard_param, LEADERBOARD_STR_MAX_LEN, param);
		
		strcpy_s(msg->param_list[0], PARAM_STR_MAX_LEN, "dummy_param");
	}
	else {
		for (int i = 0; i < PROTOCOL_PARAM_LIST_SIZE; i++)
		{
			if (STRINGS_ARE_EQUAL(msg->param_list[i], "")) { /* last param_list param */
				strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, param);
				break;
			}
		}
	}
	return ret_val;
}

ErrorCode_t ParseParams(char * params_list, protocol_t * msg)
{
	ErrorCode_t ret_val = SUCCESS;
	char * token = NULL;
	//printf("ParseParams: %s\n", params_list);
	token = strtok(params_list, ";");
	AddParam(token, msg);
	while (NULL != (token = strtok(NULL, ";"))) {
		AddParam(token, msg);
	}
	return ret_val;
}

void PrintProtocol(protocol_t * msg)
{
	char *msg_str;
	AllocateString(&msg_str, PROTOCOL_MESSAGE_MAX_LEN);
	ProtocolToString(msg, &msg_str);
	printf("Protocol <msg>:<param_list> : %s", msg_str);
	free(msg_str);
}
