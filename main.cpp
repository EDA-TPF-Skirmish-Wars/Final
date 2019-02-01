#include <iostream>
#include <allegro5/allegro.h>
#include "./Graphics/Graphics.h"
#include "./Network/Connections.h"
#include "./Game/classMap.h"
#include "./Game/classPlayer.h"
#include "./Game.h"
#include "./Callbacks.h"

Game game;

int main(void) {
	//al_init();
	game.initGame();
	game.run();
	return  0;
}

