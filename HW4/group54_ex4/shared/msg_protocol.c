#include "msg_protocol.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* parameters linked list functions */
param_node *CreateParamNode(char *param)
{
	param_node *new_element = NULL;
	if (NULL == (new_element = (param_node*)malloc(sizeof(param_node))))
	{
		printf("Node Memory Allocation Failed");
		return new_element;
	}
	strcpy_s(new_element->param, PARAM_STR_MAX_LEN, param);
	new_element->length = (int)strlen(param);
	new_element->next = NULL;
	return new_element;

}
void AddParamToList(param_node **head, char *param)
{
	param_node *new_node = CreateParamNode(param);
	if (*head == NULL) {
		*head = new_node;
		return;
	}
	param_node *curr_node = *head;
	while (curr_node->next != NULL) {
		curr_node = curr_node->next;
	}
	curr_node->next = new_node;
}
void FreeParamsList(param_node *head)
{
	param_node *current = head;
	param_node *next = NULL;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
void LinkedListToParam(LB_Node *head, param_node **head_msg)
{
	int idx = 0;
	LB_Node *curr_node = head;
	char str_for_next[PARAM_STR_MAX_LEN];
	while (curr_node != NULL)
	{
		if (idx == 0)
		{
			idx++;
			sprintf(str_for_next, "Name\t\tWon\t\tLost\t\tW/L Ratio");
			AddParamToList(head_msg, &str_for_next[0]);
			continue;
		}
		else if (curr_node->ratio == -1)
		{
			sprintf(str_for_next, "%s\t\t%d\t\t%d\t\t", curr_node->name, curr_node->won, curr_node->lost);
		}
		else
		{
			sprintf(str_for_next, "%s\t\t%d\t\t%d\t\t%.3f", curr_node->name, curr_node->won, curr_node->lost, curr_node->ratio);
		}
		AddParamToList(head_msg, &str_for_next[0]);
		curr_node = curr_node->next;
	}
}
char * GetParam(param_node *head, int ind) {
	param_node *curr_node = head;
	for (int i = 0; i < ind; i++)
	{
		curr_node = curr_node->next;
	}
	return &curr_node->param[0];
}
void printParamsList(param_node *head)
{
	param_node *temp = head;
	//printf("Name\t\tWon\t\tLost\t\tW/L Ratio\n");
	while (temp != NULL)
	{
		printf("%s\n", temp->param);
		temp = temp->next;
	}
}


/* protocol_t functions */
void InitProtocol(protocol_t * msg)
{
	if (ShouldHaveParams(msg)) {
		FreeParamsList(msg->param_list_head);
	}
	msg->type = ERROR_MSG_TYPE;
	int i = 0;
	msg->size_in_bytes = 0;
	msg->param_list_head = NULL;
}
void FreeProtocol(protocol_t * msg)
{
	if (msg->param_list_head != NULL) {
		FreeParamsList(msg->param_list_head);
	}
}
void CreateProtocol(protocol_t * msg, PROTOCOL_ENUM type, char **param_list, int param_list_size)
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
void CreateProtocolList(protocol_t * msg, PROTOCOL_ENUM type, param_node **param_list)
{
	InitProtocol(msg);
	msg->type = type;
	msg->size_in_bytes = (int)strlen(PROTOCOLS_STRINGS[msg->type]);
	if (ShouldHaveParams(msg))
	{
		msg->size_in_bytes++; /* for he ':' */
	}
	msg->param_list_head = *param_list; /*shallow copy*/
	*param_list = NULL;
	param_node *curr_node = msg->param_list_head;
	while (curr_node != NULL) {
		msg->size_in_bytes += curr_node->length + 1;
		curr_node = curr_node->next;
	}
	msg->size_in_bytes++;
}
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
ErrorCode_t ProtocolToString(protocol_t * msg, char **p_msg_str)
{
	ErrorCode_t ret_val = SUCCESS;
	int str_ind = 0;
	char *msg_str = *p_msg_str;
	
	const char * type_str = &PROTOCOLS_STRINGS[GetType(msg)][0];
	int type_len = (int)strlen(type_str);
	/* copy type */
	while (str_ind< type_len) {
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
ErrorCode_t ParseParams(char * msg_str, protocol_t * msg)
{
	ErrorCode_t ret_val = SUCCESS;
	
	int str_ind = (int)strlen(PROTOCOLS_STRINGS[GetType(msg)]);
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
void PrintProtocol(protocol_t * msg)
{
	char *msg_str;
	AllocateString(&msg_str, msg->size_in_bytes);
	ProtocolToString(msg, &msg_str);
	printf("Protocol <msg>:<param_list> : %s , %d", msg_str, msg->size_in_bytes);
	free(msg_str);
}


