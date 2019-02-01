#include "Callbacks.h"

extern Game game;
void * callbackClient(const char* mapName, unsigned int mapNameSize, int checksum) {
	string name;
	for (unsigned int i = 0; i < mapNameSize; i++) {
		name.push_back(mapName[i]);
	}
	game.screen.selectMap(name.c_str(), checksum);
	return NULL;
}

bool callback(move_s move, int data1, int data2, int data3, int data4, int data5) { 
	bool answer = false;
	if (move == ATTACK) {
		Position pos(data1, data2);
		Position pos2(data3, data4);
		answer = game.player.getMap()->enemyAttack(game.player.getMap()->getUnit(pos), pos2, data5);
		game.screen.showDices(game.myDice, data5);
	}
	else if (move == PURCHASE) {
		string code;
		code.push_back(data1);
		code.push_back(data2);
		units_d unit = game.getUnitFromCode(code);
		Position pos(data3, data4);
		game.player.getMap()->addUnit(unit, pos, game.player.getMap()->getEnemyTeam());
		answer = true;
	}
	else if (move == MOVE) {//VER QUE PASA SI ES CAPTURE
		Position pos(data1, data2);
		Position pos2(data3, data4);
		game.player.getMap()->move(pos2, game.player.getMap()->getUnit(pos));
		game.player.updateInventory();
		answer = true;
	}
	else if (move == PASS) { //CAMBIARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
		answer = true;
	}
	else
		answer = false;
	return answer;
}

int callbackResponseAttack(void) {
	return rand() % 6 + 1;
}