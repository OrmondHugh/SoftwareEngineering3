/*
 * crossfireOperations.h
 *
 *  Created on: 29 Mar 2017
 *      Author: liliana
 */

#ifndef CROSSFIREOPERATIONS_H_
#define CROSSFIREOPERATIONS_H_



#endif /* CROSSFIREOPERATIONS_H_ */

/*
 * Definition of boolean types
 * This avoids using <stdbool.h>
 */
typedef int bool;
enum { false, true };

/*
 * The slot template
 */
enum slotType {
	ground,
	hill,
	city
};

enum turnChoice { //for choosing what to do at each turn
	move,
	attack,
	quit
};

enum direction {
	up,
	down,
	left,
	right
};

struct slot {
	int row;
	int column;
	struct slot *left;
	struct slot *right;
	struct slot *up;
	struct slot *down;
	enum slotType type;

};

enum playertype {
	elf,
	human,
	ogre,
	wizard
};

struct player {
	char name[15];
	enum playertype type;
	int smartness;
	int strength;
	int magic_skills;
	int luck;
	int dexterity;
	int life;
	struct slot pos;
	bool inplay;
};

void createBoard(int boardSize,	struct slot	**upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight);
//creates the board and assigns the adjacent slots to each slot

/*
 * This function traverses the board to find a slot at a predefined
 * position (row, column)
 * Parameters:
 * 	row: the row in which the desired slot is located
 * 	column: the column in which the desired slot is located
 * 	initialSlot: the slot from which the slot search should start
 */
struct slot *reachDesiredElement(int row, int column, struct slot * initialSlot);

int getNumPlayers(void);

int distantAttack(struct slot *upLeft, int numPlayers, struct player *playersArray, struct player attacker);

int magicAttack (struct slot *upLeft, int numPlayers, struct player *playersArray, struct player attacker);

int nearAttack (int numPlayers, struct player *playersArray, struct player attacker);

struct player getPlayer(int num, struct player *playerArray);
//gets the name and type of each player

struct player setupPlayer(struct player p, struct slot * upLeft);
//calls functions to fill the players stats and place the player on the board.

struct player statfillOgre(struct player p);
struct player statfillElf(struct player p);
struct player statfillHuman(struct player p);
struct player statfillWizard(struct player p);
//each assigns stats based on the player type

enum turnChoice getTurnChoice(void);
//asks the player to move, attack, or quit.

enum direction getMoveChoice(int row, int col);
//Asks the user for the direction they wish to move

struct player playerMove(struct player p, enum direction dir);
//moves the player

void performAttack(struct slot *upLeft, int numPlayers,struct player *playersArray, struct player attacker);
//Asks the player to chose attack type and performs it

struct player playerLeave(struct player p);
//removes the player from the game

bool checkGameMode(struct player p[6]);
//scrolls through the players to check if at least 2 are still playing

void findSlots(int reqDist, int currDist, struct slot * currSlot, struct slot * foundSlots, int * count, bool explored[7][7]);
