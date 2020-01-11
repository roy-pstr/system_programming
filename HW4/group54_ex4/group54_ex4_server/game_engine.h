#ifndef _GAME_ENGINE_H
#define _GAME_ENGINE_H
#include "utils.h"
MOVES_ENUM ServerRaffleMove();

void GetGameResults(char **game_results, MOVES_ENUM player1_move, char* player1_name, MOVES_ENUM player2_move, char* player2_name);

#endif


