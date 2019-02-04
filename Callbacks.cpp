#include "Callbacks.h"
#include "./Game.h"

typedef enum {NO_UNLOAD, UNLOADING, FIRST_UNLOAD, SECOND_UNLOAD}unl_s;
static unl_s unload;

void * callbackClient(const char* mapName, unsigned int mapNameSize, char checksum, void * screen1) {
	Graphics * screen = (Graphics *)screen1;
	string name;
	for (unsigned int i = 0; i < mapNameSize; i++) {
		name.push_back(mapName[i]);
	}
	unsigned char checksum1 = (unsigned char)checksum;
	screen->selectMap(name.c_str(), checksum1, false);
	return NULL;
}

bool callback(move_s move, int data1, int data2, int data3, int data4, int data5, void * game1) { 
	Game * game = (Game*)game1;
	bool answer = false;
	if (move == ATTACK) {
		Position pos(data1, data2);
		Position pos2(data3, data4);
		answer = game->player.getMap()->enemyAttack(game->player.getMap()->getUnitPtr(pos), pos2, data5);
		game->screen.showDices(game->myDice, data5);
		unload = NO_UNLOAD;
	}
	else if (move == ENEMY_ATTACK) {
		Position pos(data1, data2);
		Position pos2(data3, data4);
		game->player.getMap()->enemyAttack(game->player.getMap()->getUnitPtr(pos), pos2, data5);
		if (game->player.getMap()->getUnitPtr(pos2) == nullptr) {
			game->myDice = 0;
		}
		else {
			game->myDice = callbackResponseAttack();
		}
		answer = game->player.getMap()->attack(game->player.getMap()->getUnitPtr(pos2), pos, game->myDice);
		game->screen.showDices(game->myDice, data5);
		unload = NO_UNLOAD;
	}
	else if (move == PURCHASE) {
		string code;
		code.push_back(data1);
		code.push_back(data2);
		units_d unit = game->getUnitFromCode(code);
		Position pos(data3, data4);
		game->player.getMap()->addUnit(unit, pos, game->player.getMap()->getEnemyTeam());
		answer = true;
		unload = NO_UNLOAD;
	}
	else if (move == MOVE) {//VER QUE PASA SI ES CAPTURE
		Position pos(data1, data2);
		Position pos2(data3, data4);
		if (unload == NO_UNLOAD && pos != pos2) {
			game->player.getMap()->enemyMove(pos2, game->player.getMap()->getUnitPtr(pos));
			game->player.updateInventory();
		}
		else if ((unload == UNLOADING || unload == FIRST_UNLOAD) && pos != pos2) {
			game->player.getMap()->unloadAPC(pos, pos2);
			game->player.updateInventory();
		}
		else if (unload != SECOND_UNLOAD && pos != pos2) {
			answer = false;
		}
		if (pos == pos2 && unload == NO_UNLOAD)
			unload = UNLOADING;
		else if (unload == UNLOADING)
			unload = FIRST_UNLOAD;
		else if (unload == FIRST_UNLOAD)
			unload = SECOND_UNLOAD;
		else
			unload = NO_UNLOAD;
		answer = true;
	}
	else if (move == PASS) {
		answer = true;
		unload = NO_UNLOAD;
	}
	else
		answer = false;
	return answer;
}

int callbackResponseAttack(void) {
	return rand() % 6 + 1;
}