#include "Callbacks.h"

extern Game game;
void * callbackClient(const char* mapName, unsigned int mapNameSize, int checksum) {
	string name = mapName;
	game.screen.selectMap(mapName, checksum);
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
	return answer;
}

int callbackResponseAttack(void) {
	return rand() % 6 + 1;
}