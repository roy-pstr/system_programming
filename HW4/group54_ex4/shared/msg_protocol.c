#include "msg_protocol.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void InitParams(param_list_t *list) {
	strcpy_s(list->param1, PARAM_STR_MAX_LEN, "");
	strcpy_s(list->param2, PARAM_STR_MAX_LEN, "");
	strcpy_s(list->param3, PARAM_STR_MAX_LEN, "");
	strcpy_s(list->param4, PARAM_STR_MAX_LEN, "");
}
void SetParamList(param_list_t *list, char *param1, char *param2, char *param3, char *param4) {
	strcpy_s(list->param1, PARAM_STR_MAX_LEN, param1);
	strcpy_s(list->param2, PARAM_STR_MAX_LEN, param2);
	strcpy_s(list->param3, PARAM_STR_MAX_LEN, param3);
	strcpy_s(list->param4, PARAM_STR_MAX_LEN, param4);
}
void CopyParams(param_list_t *from, param_list_t *to) {
	strcpy_s(to->param1, PARAM_STR_MAX_LEN, from->param1);
	strcpy_s(to->param2, PARAM_STR_MAX_LEN, from->param2);
	strcpy_s(to->param3, PARAM_STR_MAX_LEN, from->param3);
	strcpy_s(to->param4, PARAM_STR_MAX_LEN, from->param4);
}
int GetParamsLen(param_list_t *list) {
	int len = 1; /* init for ':' */
	if (STRINGS_ARE_EQUAL(list->param1, "")) {
		len += (int)strlen(list->param1) + 1;
		DEBUG_PRINT(printf("param1: %s, %d", list->param1, strlen(list->param1)));
	}
	if (STRINGS_ARE_EQUAL(list->param2, "")) {
		len += (int)strlen(list->param2) + 1;
	}
	if (STRINGS_ARE_EQUAL(list->param3, "")) {
		len += (int)strlen(list->param3) + 1;
	}
	if (STRINGS_ARE_EQUAL(list->param4, "")) {
		len += (int)strlen(list->param4) + 1;
	}
	return (len==1) ? 0:(len+1); /* +1 for \n */
}
int GetProtocolLen(protocol_t * msg) {
	int len = (strlen(PROTOCOLS_STRINGS[msg->type]) + GetParamsLen(&msg->params_list)) + 1; /* +1 for \0 */
	return len;
}

void InitProtocol(protocol_t * msg)
{
	msg->type = ERROR_MSG_TYPE;
	int i = 0;
	for (; i < PROTOCOL_PARAM_LIST_SIZE; i++) /* initialize params to "" */
	{
		strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, "");
	}
	strcpy_s(msg->leaderboard_param, PARAM_STR_MAX_LEN, "");
	msg->size_in_bytes = 0;
	InitParams(&msg->params_list);
}

void SetProtocolNew(protocol_t * msg, PROTOCOL_ENUM type, param_list_t *param_list)
{
	InitProtocol(msg);
	msg->type = type;
	int str_length = (int)strlen(PROTOCOLS_STRINGS[msg->type]);
	CopyParams(param_list, &msg->params_list);
	msg->size_in_bytes = GetProtocolLen(msg);
	//msg->size_in_bytes = PROTOCOL_MESSAGE_MAX_LEN;
	DEBUG_PRINT(printf("SetProtocolNew: %s,%s,%s,%s\n", msg->params_list.param1, msg->params_list.param2, msg->params_list.param3, msg->params_list.param4));
	DEBUG_PRINT(printf("SetProtocolNew len: %d\n", msg->size_in_bytes));
	//DEBUG_PRINT(printf("SetProtocol len: %d\n", str_length));
	//DEBUG_PRINT(PrintProtocol(msg));
}

void SetProtocol(protocol_t * msg, PROTOCOL_ENUM type, char **param_list, int param_list_size)
{
	InitProtocol(msg);
	msg->type = type;
	int str_length = (int)strlen(PROTOCOLS_STRINGS[msg->type]);
	int i = 0;
	if (param_list != NULL) { 
		str_length += 1; /* for the ':' */
		for (; i < param_list_size; i++) /* copy given parameters */
		{
			DEBUG_PRINT(printf("SetProtocol: %s\n", param_list[i]));
			AddParam(param_list[i], msg);
			str_length += (int)strlen(param_list[i]) + 1;
		}
	}
	for (;i < PROTOCOL_PARAM_LIST_SIZE; i++) /* initialize the rest to "" */
	{
		strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, "");
	}
	msg->size_in_bytes = str_length + 1;
	//DEBUG_PRINT(printf("SetProtocol len: %d\n", str_length));
	//DEBUG_PRINT(PrintProtocol(msg));
}

ErrorCode_t ParseMessage(char * msg_str, int msg_length, protocol_t * msg)
{
	/* must first initailze the protocol struct to make sure all params are initailized! */
	InitProtocol(msg);
	ErrorCode_t ret_val = SUCCESS;
	const char del[2] = ":";
	char *msg_cpy = NULL, *token = NULL;
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
		msg_cpy = NULL;
	}
	return ret_val;
}

ErrorCode_t ProtocolToString(protocol_t * msg, char **msg_str)
{
	ErrorCode_t ret_val = SUCCESS;
	strcpy_s(*msg_str, msg->size_in_bytes, PROTOCOLS_STRINGS[GetType(msg)]);
	if (ShouldHaveParams(msg)) {
		//if (STRINGS_ARE_EQUAL(msg->param_list[0], "")) { /* no params!! */
		//		printf("No parameters passed for a protocol type: %s\n", PROTOCOLS_STRINGS[GetType(msg)]);
		//		return INVALID_MESSAGE_PROTOCOL;
		//}
		strcat_s(*msg_str, msg->size_in_bytes, ":"); /* start of param_list */
		if (GetType(msg)==SERVER_LEADERBOARD) {
			strcat_s(*msg_str, msg->size_in_bytes, msg->leaderboard_param);
			strcat_s(*msg_str, msg->size_in_bytes, "\n");
			return ret_val;
		}
		if (!STRINGS_ARE_EQUAL(msg->params_list.param1, "")) {
			strcat_s(*msg_str, msg->size_in_bytes, msg->params_list.param1);
		}
		if (!STRINGS_ARE_EQUAL(msg->params_list.param2, "")) {
			strcat_s(*msg_str, msg->size_in_bytes, ";");
			strcat_s(*msg_str, msg->size_in_bytes, msg->params_list.param2);
		}
		if (!STRINGS_ARE_EQUAL(msg->params_list.param3, "")) {
			strcat_s(*msg_str, msg->size_in_bytes, ";");
			strcat_s(*msg_str, msg->size_in_bytes, msg->params_list.param3);
		}
		if (!STRINGS_ARE_EQUAL(msg->params_list.param4, "")) {
			strcat_s(*msg_str, msg->size_in_bytes, ";");
			strcat_s(*msg_str, msg->size_in_bytes, msg->params_list.param4);
		}
		//strcat_s(*msg_str, msg->size_in_bytes, "\n");
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

ErrorCode_t AddToParamList(char * param, param_list_t * params)
{
	ErrorCode_t ret_val = SUCCESS;
	if (STRINGS_ARE_EQUAL(params->param1, "")) {
		strcpy_s(params->param1, PARAM_STR_MAX_LEN, param);
		return ret_val;
	}
	else if (STRINGS_ARE_EQUAL(params->param2, "")) {
		strcpy_s(params->param2, PARAM_STR_MAX_LEN, param);
		return ret_val;
	}
	else if (STRINGS_ARE_EQUAL(params->param3, "")) {
		strcpy_s(params->param3, PARAM_STR_MAX_LEN, param);
		return ret_val;
	}
	else if (STRINGS_ARE_EQUAL(params->param4, "")) {
		strcpy_s(params->param4, PARAM_STR_MAX_LEN, param);
		return ret_val;
	}
	return ret_val;
}

ErrorCode_t AddParam(char * param, protocol_t * msg)
{
	ErrorCode_t ret_val = SUCCESS;
	if (GetType(msg) == SERVER_LEADERBOARD) {
		strcpy_s(msg->leaderboard_param, LEADERBOARD_STR_MAX_LEN, param);
		strcpy_s(msg->param_list[0], PARAM_STR_MAX_LEN, "dummy_param");
	}
	else {
		AddToParamList(param, &msg->params_list);
		//for (int i = 0; i < PROTOCOL_PARAM_LIST_SIZE; i++)
		//{
		//	if (STRINGS_ARE_EQUAL(msg->param_list[i], "")) { /* last param_list param */
		//		strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, param);
		//		break;
		//	}
		//}
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
	AllocateString(&msg_str, msg->size_in_bytes);
	ProtocolToString(msg, &msg_str);
	printf("Protocol <msg>:<param_list> : %s , %d\n", msg_str, msg->size_in_bytes);
	free(msg_str);
}
