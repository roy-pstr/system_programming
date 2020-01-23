/* this module handels the message protocol defined in this project. includes parsing, types definition and param_list handling
	the protocol defined as:
		noparams: PROTOCOL_TYPE\n
		withparams: PROTOCOL_TYPE:<param1>;<param2>;...<paramn>\n
*/
#ifndef MSG_PROTOCOL_H
#define MSG_PROTOCOL_H
#include <stdbool.h>
#include "utils.h"
#include "csv_handler.h"

/* defenition of all protocol message types */
typedef enum  {
	SERVER_MAIN_MENU,
	SERVER_APPROVED,
	SERVER_DENIED,
	SERVER_INVITE,
	SERVER_PLAYER_MOVE_REQUEST,
	SERVER_GAME_RESULTS,
	SERVER_GAME_OVER_MENU,
	SERVER_OPPONENT_QUIT,
	SERVER_NO_OPPONENTS,
	SERVER_LEADERBOARD,
	SERVER_LEADERBORAD_MENU,
	CLIENT_REQUEST,
	CLIENT_MAIN_MENU,
	CLIENT_CPU,
	CLIENT_VERSUS,
	CLIENT_LEADERBOARD,
	CLIENT_PLAYER_MOVE,
	CLIENT_REPLAY,
	CLIENT_REFRESH,
	CLIENT_DISCONNECT,
	PROTOCOL_ENUM_LAST,
	ERROR_MSG_TYPE,
	NULL_TYPE,
}PROTOCOL_ENUM;
static const char *PROTOCOLS_STRINGS[] = {
	"SERVER_MAIN_MENU",
	"SERVER_APPROVED",
	"SERVER_DENIED",
	"SERVER_INVITE",
	"SERVER_PLAYER_MOVE_REQUEST",
	"SERVER_GAME_RESULTS",
	"SERVER_GAME_OVER_MENU",
	"SERVER_OPPONENT_QUIT",
	"SERVER_NO_OPPONENTS",
	"SERVER_LEADERBOARD",
	"SERVER_LEADERBORAD_MENU",
	"CLIENT_REQUEST",
	"CLIENT_MAIN_MENU",
	"CLIENT_CPU",
	"CLIENT_VERSUS",
	"CLIENT_LEADERBOARD",
	"CLIENT_PLAYER_MOVE",
	"CLIENT_REPLAY",
	"CLIENT_REFRESH",
	"CLIENT_DISCONNECT" 
};

/* linked list for parameter_list inside protocol_t */
typedef struct p_node_st
{
	char param[PARAM_STR_MAX_LEN];
	int length;
	struct p_node_st *next;
} param_node;

param_node *CreateParamNode(char *param);

/* params linked list external functions */
char * GetParam(param_node * head, int ind);

/* copy leaderboard from leadboard linked list to parameters linked list */
void LinkedListToParam(LB_Node *head, param_node **head_msg);

/* print each param value (as string) in new line */
void printParamsList(param_node *head);

/* add param to list in the end of the linked list */
void AddParamToList(param_node **head, char *param);

/* protocol_t for handling all the protocol message parsing */
typedef struct {
	PROTOCOL_ENUM type; 
	int size_in_bytes;
	param_node *param_list_head;
} protocol_t;

/* initailzie all the protocol parameters to NULL or its equaivelnt */
void InitProtocol(protocol_t * msg);

/* release all the memory the protocol allocated */
void FreeProtocol(protocol_t * msg);

/* create a protocol from param_list of array of params */
void CreateProtocol(protocol_t * msg, PROTOCOL_ENUM type, char ** param_list, int param_list_size);

/* create a protocol from param_list of linked_list */
void CreateProtocolList(protocol_t * msg, PROTOCOL_ENUM type, param_node ** param_list);

/* input: a string, output: protocol_t msg defined by the input string */
ErrorCode_t ParseMessage(char * msg_str, int msg_length, protocol_t * msg);

/* msg_str is already allocated to 'size_in_bytes'. 
	the output will be a stirng in the following format:
	<protocol_type>:<param_list> (see full description in the start of this header file)
*/
ErrorCode_t ProtocolToString(protocol_t * msg, char ** p_msg_str);

/* returns the protocl_t type */
PROTOCOL_ENUM GetType(protocol_t *msg);

/* given a string of a protocol -> returns the correspondig enum describing the protocol type */
PROTOCOL_ENUM FindType(char *type_str);

/* returns weather or not this protocol message should contain param_list */
bool ShouldHaveParams(protocol_t *msg);

/* given string of parameters -> parsing them into the param_linked list */
ErrorCode_t ParseParams(char * params_list, protocol_t *msg);

/* debug */
void PrintProtocol(protocol_t *msg);
#endif // MSG_PROTOCOL_H


