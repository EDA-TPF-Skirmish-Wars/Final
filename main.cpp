#include <iostream>
#include <Windows.h>
#include <allegro5/allegro.h>
#include "./Graphics/Graphics.h"
#include "./Network/Connections.h"
#include "./Game/classMap.h"
#include "./Game/classPlayer.h"
#include "./Game.h"
#include "./Callbacks.h"

int main(void) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	Game game;
	game.initGame();
	game.run();
	return  0;
}

