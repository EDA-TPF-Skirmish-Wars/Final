#include <iostream>
#include <allegro5/allegro.h>
#include "./Graphics/Graphics.h"
#include "./Network/Connections.h"
#include "./Game/classMap.h"
#include "./Game/classPlayer.h"

void * callbackClient(const char* mapName, unsigned int mapNameSize, int checksum);
static Graphics screen;

int main(void) {
	if (!al_init())
	{
		cout << "Allegro no pudo inicializarse" << endl;
		return 0;
	}
	Connections net;
	net.establishConnection();
	//CARGAR NOMBRE POR GRAPHIC
	//Graphics screen;
	//string name = screen.getName();
	string name = "martina";
	net.setName(name.c_str(), name.size());
	bool temp;
	if (!net.amIServer())
	{
		temp = net.initGame(&callbackClient);
	}
	else
	{
		string mapName = screen.chooseMap();
		temp = net.initGame(NULL, mapName.size(), 0, mapName.c_str());
	}

	

	return  0;
}

void * callbackClient(const char* mapName, unsigned int mapNameSize, int checksum) {
	string name = mapName;
	screen.selectMap(mapName, checksum);
	return NULL;
}