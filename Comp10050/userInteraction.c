/*
 * userInteraction.c

 *
 *  Created on: 29 Mar 2017
 *      Author: liliana
 */

#include <stdio.h>
#include <string.h>
#include "crossfireOperations.h"

int getNumPlayers(void) {
	int NUM_PLAYERS = 7;
	while ((NUM_PLAYERS < 2) || (NUM_PLAYERS > 6)) {
		printf("Please enter the number of players : (6 max, at least 2)\n");
		scanf("%d", &NUM_PLAYERS);
	}
	return NUM_PLAYERS;
}

struct player getPlayer(int num, struct player *playerArray) {
	struct player playerI;
	bool nameTaken;		//Bool for checking if 2 players have the same name
	do {
		nameTaken = 0;
		printf("Input the name of player %d\n", num);
		scanf("%s", &playerI.name);
		for (int i = 0; i < num; i++) {		//Loop through players already assigned
			if (strcmp(playerI.name, playerArray[i].name) == 0) {	//If one has the same name as another, repeat the loop
				nameTaken = 1;
			}
		}

		if(nameTaken == 1) {
			puts("That name is already taken");
		}
	} while (nameTaken != 0);
	printf("Input the type of player %d\n", num);
	printf("(0 for Ogre\n1 for Human\n2 for Elf\n3 for Wizard)\n");
	scanf("%d", &playerI.type);
	return playerI;
}

enum turnChoice getTurnChoice(void){
	enum turnChoice choice;
	printf("\nPlease choose to move (0), attack (1) or quit(2)\n");
	scanf("%d", &choice);
	return choice;
}

enum direction getMoveChoice(int row, int col) {
	enum direction dir;
	printf("You are at position (%d,%d)\n", row, col);
	printf("Input the direction you wish to move:");
	if(row != 0) {
		printf("\n0 for up");
	}
	if(row != 6) {
		printf("\n1 for down");
	}
	if(col != 0) {
		printf("\n2 for left");
	}
	if(col != 6) {
		printf("\n3 for right");
	}
	puts("");
	scanf("%d", &dir);
	return dir;
}

void getDesiredElement(int boardSize, int * row, int * col){
	printf("Please enter the column and the Row of the desired slot \n");
	printf("Note that size of the board is %d\n", boardSize);

		//it will cycle asking the user to insert the row
		//until the value of the desired row is >= 0 or < of the
		// size of the board
		do {
			printf("Row: ");
			scanf("%d", row);
			printf("%d\n", *row);
			if(row < 0 && *row >= boardSize)
				printf("Error: Incorrect row dimension\n");
		} while(*row < 0 && *row >= boardSize);

		//it will cycle asking the user to insert the column
		//until the value of the desired row is >= 0 or < of the
		// size of the board
		do {
			printf("Column: ");
			scanf("%d", col);
			printf("%d\n", *col);
			if(*col < 0 && *col >= boardSize)
				printf("Error: Incorrect column dimension\n");
		} while(*col < 0 && *col >= boardSize);
}
