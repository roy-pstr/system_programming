#include "msg_protocol.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* linked list for params list */
param_node *CreateParamNode(char *param)
{
	param_node *new_element = NULL;
	if (NULL == (new_element = (param_node*)malloc(sizeof(param_node))))
	{
		printf("Node Memory Allocation Failed");
		return new_element;
	}
	strcpy_s(new_element->param, PARAM_STR_MAX_LEN, param);
	printf("[Ilay]: new_element->param is %s\n", new_element->param);
	new_element->length = (int)strlen(param);
	printf("[Ilay]: new_element->length is %d\n", new_element->length);
	new_element->next = NULL;
	return new_element;

}
void AddParamToList(param_node **head, char *param)
{
	param_node *new_node = CreateParamNode(param);
	if (NULL == (*head)) {
		*head = new_node;
	}
	else
	{
		param_node *curr_node = *head;
		while (curr_node->next != NULL) {
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
}
void printParamsList(param_node *head)
{
	param_node *temp = head;
	printf("Name\t\tWon\t\tLost\t\tW/L Ratio\n");
	while (temp != NULL)
	{
		printf("%s\n", temp->param);
		//print_buffer2(temp->param, 5); DEBUG
		temp = temp->next;
	}
}
void FreeParamsList(param_node **head)
{
	param_node *current = *head;
	param_node *next = NULL;
	
	while (current != NULL)
	{
		next = current->next;
		printf("[Ilay]: inside FreeParamsList, freeing 1 pointer.\n");
		free(current);
		current = next;
	}

	*head = NULL;
}

char * GetParam(param_node *head, int ind) {
	param_node *curr_node = head;
	for (int i = 0; i < ind; i++)
	{
		curr_node = curr_node->next;
	}
	return &curr_node->param[0];
}
void InitProtocol(protocol_t * msg)
{
	if (ShouldHaveParams(msg)) {
		printf("[Ilay]: inside InitProtocol, calling FreeParamsList\n");
		FreeParamsList(&(msg->param_list_head));
		//msg->param_list_head = NULL;
	}
	msg->type = ERROR_MSG_TYPE;
	int i = 0;
	//for (; i < PROTOCOL_PARAM_LIST_SIZE; i++) /* initialize params to "" */
	//{
	//	strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, "");
	//}
	strcpy_s(msg->leaderboard_param, PARAM_STR_MAX_LEN, "");
	msg->size_in_bytes = 0;
	msg->param_list_head = NULL;
}
void FreeProtocol(protocol_t * msg)
{
	if (msg->param_list_head != NULL) {
		printf("[Ilay]: inside FreeProtocol, calling FreeParamsList\n");
		FreeParamsList(&(msg->param_list_head));
		//msg->param_list_head = NULL;
	}
}
void SetProtocol(protocol_t * msg, PROTOCOL_ENUM type, char **param_list, int param_list_size)
{
	InitProtocol(msg);
	msg->type = type;
	msg->size_in_bytes = (int)strlen(PROTOCOLS_STRINGS[msg->type]);
	if (ShouldHaveParams(msg))
	{
		msg->size_in_bytes++; /* for he ':' */
	}
	for (int i = 0; i < param_list_size; i++) /* copy given parameters */
	{
		AddParamToList(&msg->param_list_head, param_list[i]);
		msg->size_in_bytes += (int)strlen(param_list[i]) + 1;
	}
	msg->size_in_bytes++;
}
void SetProtocolList(protocol_t * msg, PROTOCOL_ENUM type, param_node *param_list)
{
	InitProtocol(msg);
	msg->type = type;
	msg->size_in_bytes = (int)strlen(PROTOCOLS_STRINGS[msg->type])+1;
	if (ShouldHaveParams(msg))
	{
		msg->size_in_bytes++; /* for he ':' */
	}
	msg->param_list_head = param_list;
	param_node *curr_node = msg->param_list_head;
	while (curr_node != NULL) {
		msg->size_in_bytes += curr_node->length+1;
		curr_node = curr_node->next;
	}
	msg->size_in_bytes++;
}
//void SetProtocolOLD(protocol_t * msg, PROTOCOL_ENUM type, char **param_list, int param_list_size)
//{
//	InitProtocol(msg);
//	msg->type = type;
//	int str_length = (int)strlen(PROTOCOLS_STRINGS[msg->type]);
//	int i = 0;
//	if (param_list != NULL) { 
//		str_length += 1; /* for the ':' */
//		for (; i < param_list_size; i++) /* copy given parameters */
//		{
//			AddParam(param_list[i], msg);
//			str_length += (int)strlen(param_list[i]) + 1;
//		}
//	}
//	for (;i < PROTOCOL_PARAM_LIST_SIZE; i++) /* initialize the rest to "" */
//	{
//		strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, "");
//	}
//	msg->size_in_bytes = str_length + 1;
//	//DEBUG_PRINT(printf("SetProtocol len: %d\n", str_length));
//	//DEBUG_PRINT(PrintProtocol(msg));
//}
//
//ErrorCode_t ParseMessage_old(char * msg_str, int msg_length, protocol_t * msg)
//{
//	/* must first initailze the protocol struct to make sure all params are initailized! */
//	InitProtocol(msg);
//	ErrorCode_t ret_val = SUCCESS;
//	const char del[2] = ":";
//	char *msg_cpy = NULL, *token = NULL;
//	ret_val=AllocateString(&msg_cpy, msg_length);
//	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!\n");
//	strcpy_s(msg_cpy, msg_length, msg_str);
//	token = strtok(msg_cpy, del);
//	msg->type = FindType(token);
//	if (msg->type == ERROR_MSG_TYPE) {
//		ret_val = PROTOCOL_MSG_TYPE_ERROR;
//		printf("The given protocol is message is illegal.\n");
//		goto EXIT;
//	}
//
//	if (ShouldHaveParams(msg)) {
//		token = strtok(NULL, del); /* look if there are params */
//		if (token == NULL) {
//			printf("error when trying to parse param_list.\n");
//		}
//		if (GetType(msg) == SERVER_LEADERBOARD) {
//			AddParam(token, msg);
//		}
//		else {
//			ret_val = ParseParams(token, msg);
//			GO_TO_EXIT_ON_FAILURE(ret_val, "ParseParams() failed.\n");
//		}
//		
//	}
//	
//	msg->size_in_bytes = msg_length;
//EXIT:
//	if (msg_cpy != NULL) {
//		free(msg_cpy);
//		msg_cpy = NULL;
//	}
//	return ret_val;
//}
ErrorCode_t ParseMessage(char * msg_str, int msg_length, protocol_t * msg)
{
	ErrorCode_t ret_val = SUCCESS;
	/* must first initailze the protocol struct to make sure all params are initailized! */
	InitProtocol(msg);

	msg->type = FindType(msg_str);
	if (msg->type == ERROR_MSG_TYPE) {
		ret_val = PROTOCOL_MSG_TYPE_ERROR;
		printf("The given protocol is message is illegal.\n");
		goto EXIT;
	}

	if (ShouldHaveParams(msg)) {
		//int str_ind = strlen(PROTOCOLS_STRINGS[msg->type]) + 1;
		//if (msg_str[str_ind] != ':') {
		//	printf("The given protocol message does not have parameters!\n");
		//	ret_val = PROTOCOL_MESSAGE_INVALID;
		//	goto EXIT;
		//}
		//str_ind++;
		ret_val = ParseParams(msg_str, msg);
		GO_TO_EXIT_ON_FAILURE(ret_val, "ParseParams() failed.\n");
	}
	
	msg->size_in_bytes = msg_length;
EXIT:
	return ret_val;
}
//ErrorCode_t ProtocolToString_OLD(protocol_t * msg, char **msg_str)
//{
//	ErrorCode_t ret_val = SUCCESS;
//	strcpy_s(*msg_str, msg->size_in_bytes, PROTOCOLS_STRINGS[GetType(msg)]);
//	if (ShouldHaveParams(msg)) {
//		if (STRINGS_ARE_EQUAL(msg->param_list[0], "")) { /* no params!! */
//				printf("No parameters passed for a protocol type: %s\n", PROTOCOLS_STRINGS[GetType(msg)]);
//				return PROTOCOL_MESSAGE_INVALID;
//		}
//		strcat_s(*msg_str, msg->size_in_bytes, ":"); /* start of param_list */
//		if (GetType(msg)==SERVER_LEADERBOARD) {
//			strcat_s(*msg_str, msg->size_in_bytes, msg->leaderboard_param);
//			strcat_s(*msg_str, msg->size_in_bytes, "\n");
//			return ret_val;
//		}
//		strcat_s(*msg_str, msg->size_in_bytes, msg->param_list[0]);
//		for (int i = 1; i < PROTOCOL_PARAM_LIST_SIZE; i++)
//		{
//			if (STRINGS_ARE_EQUAL(msg->param_list[i], "")) { /* end of param_list */
//				break; }
//			strcat_s(*msg_str, msg->size_in_bytes, ";");
//			strcat_s(*msg_str, msg->size_in_bytes, msg->param_list[i]);
//			
//		}
//		strcat_s(*msg_str, msg->size_in_bytes, "\n");
//	}
//	return ret_val;
//}
ErrorCode_t ProtocolToString(protocol_t * msg, char **p_msg_str)
{
	ErrorCode_t ret_val = SUCCESS;
	int str_ind = 0;
	char *msg_str = *p_msg_str;
	
	const char * type_str = &PROTOCOLS_STRINGS[GetType(msg)][0];
	int type_len = strlen(type_str);
	/* copy type */
	while (str_ind < type_len) {
		msg_str[str_ind] = *type_str;
		str_ind++;
		type_str++;
	}
	msg_str[str_ind] = ShouldHaveParams(msg) ? ':':'\n';
	str_ind++;
	if (ShouldHaveParams(msg)) {
		if (msg->param_list_head == NULL) {
			printf("No parameters passed for a protocol type: %s\n", PROTOCOLS_STRINGS[GetType(msg)]);
			return PROTOCOL_MESSAGE_INVALID;
		}
		
		/* copy params */
		param_node *curr_node = msg->param_list_head;
		//char *params_str = *p_params_str;
		while (curr_node != NULL) {
			int curr_param_ind = 0;
			while (curr_param_ind< curr_node->length) {
				msg_str[str_ind] = curr_node->param[curr_param_ind];
				curr_param_ind++;
				str_ind++;
			}
			msg_str[str_ind] = ';';
			str_ind++;
			curr_node = curr_node->next;
		}
		msg_str[str_ind-1] = '\n';
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

PROTOCOL_ENUM FindType(char * msg_str)
{
	char type_name[PROTOCOL_TYPE_STR_MAX_LEN];
	int ind = 0;
	while (msg_str[ind] != ':' && msg_str[ind] != '\n') {
		type_name[ind] = msg_str[ind];
		ind++;
	}
	type_name[ind] = '\0';
	for (int i = 0; i < PROTOCOL_ENUM_LAST; i++)
	{
		if (STRINGS_ARE_EQUAL(type_name, PROTOCOLS_STRINGS[i])) {
			return (PROTOCOL_ENUM)i;
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
void copyparam(char *dest, int size, char *source) {
	for (int i = 0; i < size; i++)
	{
		if (source[i] == '\n') {
			break;
		}
		dest[i] = source[i];
	}
}
//ErrorCode_t AddParam(char * param, protocol_t * msg)
//{
//	ErrorCode_t ret_val = SUCCESS;
//	if (GetType(msg) == SERVER_LEADERBOARD) {
//		strcpy_s(msg->leaderboard_param, LEADERBOARD_STR_MAX_LEN, param);
//		strcpy_s(msg->param_list[0], PARAM_STR_MAX_LEN, "dummy_param");
//	}
//	else {
//		for (int i = 0; i < PROTOCOL_PARAM_LIST_SIZE; i++)
//		{
//			if (STRINGS_ARE_EQUAL(msg->param_list[i], "")) { /* last param_list param */
//				//strcpy_s(msg->param_list[i], PARAM_STR_MAX_LEN, param);
//				copyparam(msg->param_list[i], PARAM_STR_MAX_LEN, param);
//				break;
//			}
//		}
//	}
//	AddParamToList(&msg->param_list_head, param);
//	return ret_val;
//}
ErrorCode_t ParseParams(char * msg_str, protocol_t * msg)
{
	ErrorCode_t ret_val = SUCCESS;
	
	int str_ind = strlen(PROTOCOLS_STRINGS[GetType(msg)]); 
	if (msg_str[str_ind] != ':') {/* must point to ':' */
		printf("The given protocol message does not have parameters!\n");
		ret_val = PROTOCOL_MESSAGE_INVALID;
		goto EXIT;
	}
	str_ind++;

	char curr_param[PARAM_STR_MAX_LEN];
	int copy_from = str_ind;
	int param_len = 0;
	while (msg_str[str_ind] != '\n') {
		if (msg_str[str_ind] == ';') {
			/*copy*/
			strncpy_s(curr_param, PARAM_STR_MAX_LEN, &msg_str[copy_from], param_len);
			AddParamToList(&msg->param_list_head, &curr_param[0]);
			copy_from = str_ind + 1;
			param_len = 0;
		}
		else {
			param_len++;
		}
		str_ind++;
	}
	strncpy_s(curr_param, PARAM_STR_MAX_LEN, &msg_str[copy_from], (param_len));
	AddParamToList(&msg->param_list_head, &curr_param[0]);
EXIT:
	return ret_val;
}
//ErrorCode_t ParseParamsOLD(char * params_list, protocol_t * msg)
//{
//	ErrorCode_t ret_val = SUCCESS;
//	char *ptr = params_list, *start;
//	char curr_param[PARAM_STR_MAX_LEN];
//	int curr_len = 1;
//	start = params_list;
//	while (*ptr != '\n') {
//		if (*ptr == ';') {
//			strncpy_s(curr_param, PARAM_STR_MAX_LEN, start, curr_len-1); /* copy param without ; */
//			AddParam(curr_param, msg);
//			//DEBUG_PRINT(printf("ParseParams: [%s]\n", curr_param));
//			curr_len = 0;
//			start = ptr + 1;
//		}
//		curr_len++;
//		ptr++;
//	}
//	strncpy_s(curr_param, PARAM_STR_MAX_LEN, start, (curr_len-1)); /* copy last param without \n */
//	AddParam(curr_param, msg);
//	//DEBUG_PRINT(printf("ParseParams: [%s]\n", curr_param));
//	//char * token = NULL;
//	//token = strtok(params_list, ";");
//	//AddParam(token, msg);
//	//while (NULL != (token = strtok(NULL, ";"))) {
//	//	AddParam(token, msg);
//	//}
//	return ret_val;
//}

void PrintProtocol(protocol_t * msg)
{
	char *msg_str;
	AllocateString(&msg_str, msg->size_in_bytes);
	ProtocolToString(msg, &msg_str);
	printf("Protocol <msg>:<param_list> : %s , %d", msg_str, msg->size_in_bytes);
	free(msg_str);
}


