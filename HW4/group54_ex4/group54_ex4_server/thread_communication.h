#ifndef _THREAD_COMM_H
#define _THREAD_COMM_H
#include <windows.h>

ErrorCode_t InitThreadCommunicationModule();
ErrorCode_t SignleSecondPlayerConnected();
ErrorCode_t ResetSecondPlayerEvent();
ErrorCode_t TryCreateSessionFile(bool *created);
ErrorCode_t WriteAndReadMoves(MOVES_ENUM my_move, MOVES_ENUM * oppent_move, bool first_player);
int WriteMove(char *move);
MOVES_ENUM ReadMove();
int DeleteGameSessionFile();
ErrorCode_t WaitForSecondPlayer();

#endif
