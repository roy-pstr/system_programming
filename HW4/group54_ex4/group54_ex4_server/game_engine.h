#ifndef _GAME_ENGINE_H
#define _GAME_ENGINE_H
#include "defines.h"
MOVES_ENUM ServerRaffleMove();

void GetGameResults(char game_results[][PARAM_STR_MAX_LEN], MOVES_ENUM player1_move, MOVES_ENUM player2_move, char * other_player_name);

#endif


