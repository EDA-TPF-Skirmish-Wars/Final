#include <iostream>
#include <allegro5/allegro.h>
#include "./Graphics/Graphics.h"
#include "./Network/Connections.h"
#include "./Game/classMap.h"
#include "./Game/classPlayer.h"
#include "./Game.h"


int main(void) {
	Game game;
	game.run();
	return  0;
}