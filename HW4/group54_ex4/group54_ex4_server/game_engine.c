#include "game_engine.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Raffle move for client Vs computer*/
MOVES_ENUM ServerRaffleMove()
{
	srand(time(NULL));
	MOVES_ENUM result = -1;
	result = rand() % 5;
	printf("%d\n", result);
	return result;

}

/*Switch case function to decide who won - return 1 for user one to win, 2 for user two to win, 0 for draw*/
int CheckWhoWon(MOVES_ENUM first_move, MOVES_ENUM second_move)
{
	/*ROCK -> SCISSORS || LIZARD*/
	/*PAPER -> ROCK || SPOCK*/
	/*SCISSORS -> PAPER || LIZARD*/
	/*LIZARD -> PAPER || SPOCK*/
	/*SPOCK -> ROCK || SCISSORS*/
	if (first_move == second_move) //draw
	{
		return 0;
	}
	/*now its first or second only!*/
	if (first_move == ROCK)
	{
		if (second_move == SCISSORS || second_move == LIZARD)
			return 1;
		else
			return 2;
	}
	if (first_move == PAPER)
	{
		if (second_move == ROCK || second_move == SPOCK)
			return 1;
		else
			return 2;
	}
	if (first_move == SCISSORS)
	{
		if (second_move == PAPER || second_move == LIZARD)
			return 1;
		else
			return 2;
	}
	if (first_move == LIZARD)
	{
		if (second_move == PAPER || second_move == SPOCK)
			return 1;
		else
			return 2;
	}
	if (first_move == SPOCK)
	{
		if (second_move == ROCK || second_move == SCISSORS)
			return 1;
		else
			return 2;
	}

	return -1;
}
/*Not done yet*/

void GetGameResults(char **game_results, MOVES_ENUM player1_move, char* player1_name, MOVES_ENUM player2_move, char* player2_name) {
	int winner_idx = CheckWhoWon(player1_move, player2_move);
	strcpy(game_results[0], player2_name);
	strcpy(game_results[1], MOVES_STRINGS[player2_move]);
	printf("MOVE = %s", MOVES_STRINGS[player1_move]);
	strcpy(game_results[2], MOVES_STRINGS[player1_move]);
	if (winner_idx == 1) {
		strcpy(game_results[3], player1_name);
	}
	else
	{
		if (winner_idx == 2)
		{
			strcpy(game_results[3], player2_name);
		}
		else
		{
			strcpy(game_results[3], "NONE");
		}
	}
}