/* this module handles all the methods that handling thread comunitcation with each other, mainly functions involving using 
	of events and mutex 
*/
#ifndef _THREAD_COMM_H
#define _THREAD_COMM_H
#include <windows.h>
#include "msg_protocol.h"

/* create all events and mutex for running the server! must call this function */
ErrorCode_t InitThreadCommunicationModule();

/* used to signle the event of second player connected */
ErrorCode_t SignleSecondPlayerConnected();

/* used for reset the event of second player connected */
ErrorCode_t ResetSecondPlayerConnectedEvent();

/* blocking function: wait until the event of the oppent made a decision is invoked. */
ErrorCode_t WaitForOpponentDecision(PROTOCOL_ENUM my_decision, PROTOCOL_ENUM * opponent_decision);

/* used for reset the event of opponent decision event */
ErrorCode_t ResetOpponentDecisionEvent();

/* used for reset the event of second player replay event */
ErrorCode_t ResetSecondPlayerReplayEvent();

/* a function that called from inside a client thead, the function checks if the GameSeassion file exist and if not, creates it
	the bool *created is true if the function indeed created the file */
ErrorCode_t TryCreateSessionFile(bool *created);

ErrorCode_t WaitForSecondPlayerToConnect(bool * second_player_status, bool * created_session_file);

ErrorCode_t WaitForSecondPlayerReplay(bool * second_player_replay);

/* this function handles the writing and readign of the clients move to and from the GameSession file */
ErrorCode_t WriteAndReadMoves(MOVES_ENUM my_move, MOVES_ENUM * oppent_move, bool first_player);

/* write a move to file */
int WriteMove(char *move);

/* delete the GameSession file*/
int DeleteGameSessionFile();

#endif
