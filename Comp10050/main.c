/*
 ============================================================================
 Name        : Crossfire2.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CrossfireOperations.h"

int main(void) {
	int NUM_PLAYERS, row = 0 , col = 0;
	srand(time(NULL));
	struct slot *upLeft;
	struct slot *upRight;
	struct slot *downLeft;
	struct slot *downRight;
	struct player players[6];
	bool gameMode = true;
	enum turnChoice choice;

	setbuf(stdout, NULL);	//Make sure output in eclipse happens at correct point
	createBoard(7, &upLeft, &upRight, &downLeft, &downRight);
	NUM_PLAYERS = getNumPlayers();
	for(int i=0; i<NUM_PLAYERS; i++){
		players[i] = getPlayer(i+1, players);
	}
	for(int i=0; i<NUM_PLAYERS; i++){
		players[i] = setupPlayer(players[i], upLeft);
	}

	while(gameMode == true) { //while loop for taking each turn.
		for(int i=0; i<NUM_PLAYERS; i++) { //loops through the array of players
			if (players[i].inplay == true){ //checks if the player is still playing.
				if(players[i].life < 1){ //checks if the player has died
					players[i].inplay = false; //removes them from the game if they have.
				} else {
					printf("\n\n----%s----\n", players[i].name);
					choice = getTurnChoice(); //player chooses what to do.
					switch(choice) {
						case 0 : players[i] = playerMove( players[i], getMoveChoice(players[i].pos.row, players[i].pos.column));break;
						case 1 : performAttack(upLeft, NUM_PLAYERS, players, players[i]); break;
						case 2 : players[i] = playerLeave(players[i]);
					}
				}
			}

		}
		gameMode = checkGameMode(players); //checks if at least to players are still playing before starting the next turn.
	}

	puts("Game over");
	return 0;
}
