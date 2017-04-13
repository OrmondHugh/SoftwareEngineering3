/*
 * statFunctions.c
 *
 *  Created on: 4 Apr 2017
 *      Author: eoghancunningham
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "crossfireOperations.h"

struct player statfillOgre(struct player p){
	srand(time(NULL));
	p.smartness = rand() % 21;
	p.strength = 80 + rand() % 21;
	p.dexterity = 80 + rand() % 21;
	p.magic_skills = 0;
	while (((p.luck + p.smartness) > 50) || (p.luck == 0)){
		p.luck = rand() % 45;
	}
	p.life = 100;
	return p;
}
struct player statfillElf(struct player p){
	srand(time(NULL));
	p.smartness = 70 + rand()%31;
	p.strength = 1 + rand()%50;
	p.dexterity = 1 + rand()%100;
	p.magic_skills = 50 + (rand()%31);
	p.luck = 60 + rand() % 41;
	p.life = 100;
	return p;
}
struct player statfillWizard(struct player p){
	srand(time(NULL));
	p.smartness = 90 + rand() % 11;
	p.strength = 1 + rand() % 20;
	p.dexterity = 1 + rand() % 100;
	p.magic_skills = 80 + rand() % 21;
	p.luck = 50 + rand() % 51;
	p.life = 100;
	return p;
}
struct player statfillHuman(struct player p){
	srand(time(NULL));
	int sum = 301;
	while (sum > 300) {
		p.dexterity = 1 + rand() % 100;
		p.luck = 1 + rand() % 100;
		p.magic_skills = 1 + rand() % 100;
		p.smartness = 1 + rand() % 100;
		p.strength = 1 + rand() % 100;
		sum = p.dexterity + p.luck + p.strength + p.magic_skills + p.smartness;
		p.life = 100;
	}
	return p;
}

struct player setupPlayer(struct player p, struct slot * upLeft) { //sets up each player
	int col = 0, row = 0;
	switch(p.type)
		{
			case 0: p = statfillOgre(p); break;
			case 1: p = statfillElf(p); break;   //assigns the stats
			case 2: p = statfillHuman(p); break;
			case 3: p = statfillWizard(p); break;
		}
		row += rand();
		row %= 7;
		col += rand();
		col %= 7;
		p.pos = * reachDesiredElement( row, col, upLeft); //places player on a slot
		p.inplay = true;
		return p;
}

struct player playerLeave(struct player p) {
	printf("\n%s has left the game\n", p.name);
	p.inplay = false;			//removes a player from the game
	return p;
}

bool checkGameMode(struct player p[6]){
	int count = 0;
	bool gameStatus;
	for(int i=0; i<6; i++){ //counts how many players are still player
		if (p[i].inplay == true){
			count++;
		}
		if(count >= 2){
			gameStatus = true; //ensures at least two
		}
	}
	return gameStatus;
}
