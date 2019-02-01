#ifndef GAME_H
#define GAME_H
#include <allegro5/allegro.h>
#include "./Graphics/Graphics.h"
#include "./Network/Connections.h"
#include "./Game/classMap.h"
#include "./Game/classPlayer.h"

class Game {
public:
	Game();
	~Game();
	void run();
protected:
	Graphics screen;
	Player player;
private:
	void * callbackClient(const char* mapName, unsigned int mapNameSize, int checksum);
	bool callback(move_s move, int data1, int data2, int data3, int data4, int data5);
	unsigned int myDice;
	string getUnitCode(units_d unitClass);

};

#endif