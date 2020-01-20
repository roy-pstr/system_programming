#ifndef MSG_PROTOCOL_H
#define MSG_PROTOCOL_H
#include <stdbool.h>
#include "utils.h"


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
typedef enum  {
	FULL,
	USERNAME_EXIST
} SERVER_DENIED_REASONS_IND;
static const char *SERVER_DENIED_REASONS[] = {
	"Server is full: No slots available for client.",
	"Username is already connected to server."
};

typedef struct p_node_st
{
	char param[PARAM_STR_MAX_LEN];
	int length;
	struct p_node_st *next;
} param_node;

typedef struct {
	PROTOCOL_ENUM type; /* message type as defiend in server_protocol or client_protocol */
	//char param_list[PROTOCOL_PARAM_LIST_SIZE][PARAM_STR_MAX_LEN]; /* <param1>;<param2>;<param3>\n*/
	char leaderboard_param[LEADERBOARD_STR_MAX_LEN];
	int size_in_bytes;
	param_node *param_list_head;
} protocol_t;






ErrorCode_t AllocateString(char ** str_ptr, int len);

char * GetParam(param_node * head, int ind);

void InitProtocol(protocol_t * msg);

void FreeProtocol(protocol_t * msg);

void SetProtocol(protocol_t * msg, PROTOCOL_ENUM type, char ** param_list, int param_list_size);

void SetProtocolList(protocol_t * msg, PROTOCOL_ENUM type, param_node * param_list);

ErrorCode_t ParseMessage(char * msg_str, int msg_length, protocol_t * msg);

/* msg_str is already allocated to 'size_in_bytes'. 
	the output will be a sting in the following format:
	<protocol_type>:<param_list>
*/

ErrorCode_t ProtocolToString(protocol_t * msg, char ** p_msg_str);

PROTOCOL_ENUM GetType(protocol_t *msg);

/* gets a string of a protocol and returns the correspondig enum describing the protocol type */
PROTOCOL_ENUM FindType(char *type_str);

bool ShouldHaveParams(protocol_t *msg);

ErrorCode_t AddParam(char * param, protocol_t *msg);

ErrorCode_t ParseParams(char * params_list, protocol_t *msg);
/* debug */
void PrintProtocol(protocol_t *msg);
#endif // MSG_PROTOCOL_H