#include <stdio.h>
#include <stdlib.h>
#include "crossFireOperations.h"
#include <string.h>

void findSlots(int reqDist, int currDist, struct slot *currSlot, struct slot *foundSlots, int *count, bool explored[7][7]) {
	if (currDist == reqDist) {	//If the distance from the starting slot slot is the required distance
		if (explored[currSlot ->row][currSlot ->column] == false) {	//And the slot has not been explored
			*(foundSlots + *count) = *currSlot;	//Add the slot to the array
			(*count)++;	//and increment count
			explored [currSlot->row][currSlot->column] = true;	//We say that the slot is explored now
		}
	} else {	//If the distance isn't correct, call the function to check adjacent slots
		if(currSlot->up	!= NULL){
			findSlots(reqDist, currDist +1, currSlot->up, foundSlots, count, explored);
		}
		if(currSlot->right != NULL){
		 	findSlots(reqDist, currDist +1, currSlot->right, foundSlots, count,	explored);
		}
		if(currSlot->left != NULL){
			findSlots(reqDist, currDist +1, currSlot->left, foundSlots,	count, explored);
		}
		if(currSlot->down != NULL){
		 	findSlots(reqDist, currDist +1, currSlot->down, foundSlots,	count, explored);
		}
	}
}

int distantAttack(struct slot *upLeft, int numPlayers, struct player *playersArray, struct player attacker) {
	struct slot *foundSlots;	//Array of slots the correct distance from the attacker
	bool *foundPlayers;	//Array of bools, each corrisponding to a player attackable. If we know a player at 1 in players can be attacked, foundPlayers[1] would be true.
	bool explored[7][7];
	int countSlots = 0;
	int attackablePlayers[numPlayers];	//Indexs of players that can be attacked to make it easier to ask
	int attackChoice;		//Index of attackable player the user has chosen to attack

	struct slot *currSlot = reachDesiredElement(attacker.pos.row,attacker.pos.column, upLeft);

	for (int i = 0; i < 7; i++) {
		for(int	j = 0; j < 7; j++){
			explored[i][j] = false;
		}
	}
	foundPlayers = (bool*) malloc(numPlayers * sizeof(bool));
	for (int i = 0; i < numPlayers;i++) {
		foundPlayers[i] = false;
	}

	for (int i = 2; i < 5; i++) {	//Find all slots which are between 1 and 5 slots away from the attacker
		countSlots = 0;

		foundSlots = (struct slot*) calloc(16, sizeof(struct slot));	//Reset the foundSlots array for each iteration
		findSlots(i, 0, currSlot, foundSlots, &countSlots, explored);

		for(int j = 0;j < numPlayers && foundPlayers[j] == false; j++) {	//Check if a player is in any of the relevant slots
			for (int k = 0; k < countSlots && foundPlayers[j] == false; k++) {	//Loop through found slots for each player, until we find that it is in an attackable slot
				if (playersArray[j].pos.column == foundSlots[k].column && playersArray[j].pos.row == foundSlots[k].row &&((strcmp(attacker.name, playersArray[j].name)) != 0)) {	//Also make sure players cannot attack themself
					foundPlayers[j] = true;
				}
			}
		}
	}

	int counter = 0;	//Count of how many players can be attacked
	for (int i = 0; i < numPlayers; i++) {	//Format the attackable players so that we can ask nicely
		if (foundPlayers[i] == true) {
			attackablePlayers[counter] = i;
			counter++;
		}
	}
	if (counter > 0) {
		puts("Who would you like to attack?");
		for (int i = 0; i < counter; i++) {	//Loop through the array of players in range and ask the player
			printf("Enter %d to attack %s\n", i, playersArray[attackablePlayers[i]].name);
		}
		scanf("%d", &attackChoice);
	} else {
		puts("There are currently no players in range");
		return -1;	//Return -1 to indicate that there are no attackable players
	}

	if (attackChoice < counter && attackChoice >= 0) {	//Make sure the user enter a valid number
		if (playersArray[attackablePlayers[attackChoice]].dexterity >= attacker.dexterity) {
			printf("%s's life points remain unchanged", playersArray[attackablePlayers[attackChoice]].name);
		} else {
			playersArray[attackablePlayers[attackChoice]].life -= (0.3 * attacker.strength);
			printf("%s hits %s for %.0f damage\n%s now has %d life points", attacker.name, playersArray[attackablePlayers[attackChoice]].name, (0.3 * attacker.strength), playersArray[attackablePlayers[attackChoice]].name, playersArray[attackablePlayers[attackChoice]].life);
		}
	} else {
		puts("That is not a valid choice");
		return -1;
	}


	return 0;	//Indicate that the attack was successful
}

int magicAttack (struct slot *upLeft, int numPlayers, struct player *playersArray, struct player attacker) {
	if ((attacker.magic_skills + attacker.smartness) <= 150) {
		puts("You do not have the stats to perform a magic attack");
		return -1;
	}

	int attackChoice;		//Index of attackable player the user has chosen to attack

	puts("Which player would you like to attack?");
	for(int i = 0; i < numPlayers; i++) {
		if (strcmp(attacker.name, playersArray[i].name) != 0) {	//Do not display the option for a player to attack themself
			printf("Enter %d to attack %s\n",i,playersArray[i].name);
		}
	}
	scanf("%d", &attackChoice);
	if (attackChoice < numPlayers && attackChoice >= 0 && (strcmp(attacker.name, playersArray[attackChoice].name) != 0)) {	//If the user choice a valid player to attack
		playersArray[attackChoice].life -= 0.5 * attacker.magic_skills + 0.2 * attacker.smartness;

		printf("%s hit %s for %.1f damage\n%s now has %d life points\n", attacker.name, playersArray[attackChoice].name, (0.5 * attacker.magic_skills + 0.2 * attacker.smartness), playersArray[attackChoice].name, playersArray[attackChoice].life);
	} else {
		puts("That is not a valid choice");
		return -1;	//Return negative to indicate the attack did not take place
	}


	return 0;
}

int nearAttack (int numPlayers, struct player *playersArray, struct player attacker) {
	struct player **playersInRange;	//Store pointers to players in range for near attack
	int playersCount = 0;	//Count of player in attack range
	playersInRange = (struct player **) malloc(numPlayers * sizeof(struct player*));
	int attackChoice;

	for (int i = 0; i < numPlayers; i++) {
		if ((strcmp(attacker.name, playersArray[i].name) != 0) && (		//If the player is not themself
			(attacker.pos.row == playersArray[i].pos.row && attacker.pos.column == playersArray[i].pos.column) ||	//and the player is in the same slot, or one of the adjacent slots
			(attacker.pos.left->row == playersArray[i].pos.row && attacker.pos.left->column == playersArray[i].pos.column) ||
			(attacker.pos.right->row == playersArray[i].pos.row && attacker.pos.right->column == playersArray[i].pos.column) ||
			(attacker.pos.up->row == playersArray[i].pos.row && attacker.pos.up->column == playersArray[i].pos.column) ||
			(attacker.pos.down->row == playersArray[i].pos.row && attacker.pos.down->column == playersArray[i].pos.column)
		)) {
			playersInRange[playersCount] = &playersArray[i];
			playersCount++;
		}
	}

	if (playersCount > 0) {
		if (playersCount > 1) {
			for (int i = 0; i < playersCount; i++) {
				printf("Enter %d to attack %s\n", i, playersInRange[i]->name);
			}
			scanf("%d", &attackChoice);
		} else {
			attackChoice = 0;
		}
		if (attackChoice < playersCount && attackChoice >= 0) {	//If the user entered a valid input
			if (playersInRange[attackChoice]->strength <= 70) {
				playersInRange[attackChoice]->life -= 0.5 * attacker.strength;
				printf("%s hits %s for %.1f damage.\n%s now has %d life points\n", attacker.name, playersInRange[attackChoice]->name, (0.5 * attacker.strength), playersInRange[attackChoice]->name, playersInRange[attackChoice]->life);
			} else {
				attacker.life -= 0.3 * playersInRange[attackChoice]->strength;
				printf("Due to the high strength of %s, %s hits %s for %.1f damage.\n%s now has %d life points\n", playersInRange[attackChoice]->name, playersInRange[attackChoice]->name, attacker.name, (0.3 * playersInRange[attackChoice]->strength), attacker.name, attacker.life);
			}
		} else {
			puts("That is not a valid choice");
			return -1;
		}
	} else {
		puts("There are no players currently in range");
		return -1;
	}

	return 0;
}

void performAttack(struct slot *upLeft, int numPlayers,struct player *playersArray, struct player attacker) {
	int result = 1;	//Int defined to be 0 upon a succesful attack, and -1 when an attack fails
	int choice;
	while (result != 0) {	//Loop asking the user to choose a type of attack until one succeeds or they cancel
		puts("Enter 0 to perform a near attack, 1 to perform a distant attack, 2 to perform a magic attack or 3 to cancel");
		scanf("%d", &choice);

		switch (choice) {
			case (0):
				result = nearAttack(numPlayers, playersArray, attacker);
				break;
			case (1):
				result = distantAttack(upLeft, numPlayers, playersArray, attacker);
				break;
			case(2):
				result = magicAttack(upLeft, numPlayers, playersArray, attacker);
				break;
			case(3):
				result = 0;
				break;
			default:
				puts("That is not a valid input");
		}
	}
}
