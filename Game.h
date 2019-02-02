#ifndef GAME_H
#define GAME_H
#include <allegro5/allegro.h>
#include "./Graphics/Graphics.h"
#include "./Network/Connections.h"
#include "./Game/classMap.h"
#include "./Game/classPlayer.h"
#include <string>

class Game {
public:
	//Game();
	void initGame();
	//~Game();
	void run();
	/*void  * callbackClient(const char* mapName, unsigned int mapNameSize, int checksum);
	bool callback(move_s move, int data1, int data2, int data3, int data4, int data5);
	int callbackResponseAttack(void);*/
	Graphics screen;
	Player player;
	Connections net;
	unsigned int myDice;
	string getUnitCode(units_d unitClass);
	units_d getUnitFromCode(string code);
private:
	int dice;


};
#endif