#ifndef _THREAD_COMM_H
#define _THREAD_COMM_H
#include <windows.h>
#include "msg_protocol.h"
ErrorCode_t InitThreadCommunicationModule();
ErrorCode_t SignleSecondPlayerConnected();
ErrorCode_t ResetSecondPlayerEvent();
ErrorCode_t TryCreateSessionFile(bool *created);
ErrorCode_t WaitForSecondPlayerToConnect(bool * second_player_status, bool * created_session_file);
ErrorCode_t WaitForSecondPlayerReplay(bool * second_player_replay);
ErrorCode_t WriteAndReadMoves(MOVES_ENUM my_move, MOVES_ENUM * oppent_move, bool first_player);
int WriteMove(char *move);
MOVES_ENUM ReadMove();
int DeleteGameSessionFile();

#endif
