#pragma once

#include "TileClass.h"
#include "unitsInfo.h" 

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

typedef struct {
	bool attackUpAvailable;
	bool attackDownAvailable;
	bool attackRightAvailable;
	bool attackLeftAvailable;
	bool buyAvailable;
	bool moveUpAvailable;
	bool moveDownAvailable;
	bool moveLeftAvailable;
	bool moveRightAvailable;
	bool passAvailable;
	bool captureAvailable;
	bool canLoad;
	bool canUnload;
}options_s;


typedef struct {
	teams_d team;
	unsigned int HQCPoints;
	unsigned int numberFactories;
	unsigned int numberCities;
	unsigned int numberUnits;
}p_inv_s;

class Map
{
public:
	Map(); //seteo el arreglo de punteros a tile en null
	~Map();

	Unit getUnit(Position pos);
	Building getBuilding(Position pos);

	Unit * getUnitPtr(Position pos);
	Building * getBuildingPtr(Position pos);

	unit_type getUnitType(Position pos);
	teams_d getUnitTeam(Position pos);
	teams_d getBuildingTeam(Position pos);
	bool getFog(Position pos);
	terrains_d getTerrain(Position pos);

	teams_d getTeam(); //del mapa
	teams_d getEnemyTeam();

	bool IsUnitOnTop(Position pos);
	bool IsBuildingOnTop(Position pos);
	
	void setTeam(teams_d team);
	void setEnemyTeam(teams_d enemyTeam);

	void addTile(Position pos, terrains_d type, bool fog);
	bool addBuilding(buildings_d type, teams_d color, Position pos);
	bool addUnit(units_d unitClass, Position pos, teams_d owner);

	options_s getOptions(Position pos);
	
	//funciones para completar options_s
	bool buyingAvailable(Position pos);
	bool captureAvailable(Position pos);
	bool loadAvailable(Position pos);
	bool unloadAvailable(Position pos, Position WhereTo);
	bool moveUPavailable(Position pos);
	bool moveDOWNavailable(Position pos);
	bool moveLEFTavailable(Position pos);
	bool moveRIGHTavailable(Position pos);


	void removeUnit(Position pos);
	void changeUnitPos(Position pos, Position newPos);
	void clearFog(Position pos); // saca la fog de la tile de arriba abajo derecha e izquierda de la posición que le mando
	void selectTile(Position pos);
	void unselectTile(Position pos);
	void unloadAPC(Position pos, Position newPos);

	bool posInMap(Position pos);


	p_inv_s getPlayerInventory(teams_d color);
	options_s getOptions(Position pos); //cambia si en esa tile hay solo building, solo unit o hay ambas 

private:
	Tile * board[BOARD_HEIGHT][BOARD_WIDTH]; //para agregar tile uso la position
	teams_d team;
	teams_d enemyTeam;
};
