#ifndef CLASSMAP_H
#define CLASSMAP_H

#include "./classUnit.h"
#include "./classBuilding.h"
#include <list>
#include "./classAPC.h"
#include "./TileClass.h"

using namespace std;

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 12

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
	Position destination;
	unsigned int movingPoints;
}moves_s;

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
	//~Map();

	Unit getUnit(Position pos);
	Building getBuilding(Position pos);

	Unit * getUnitPtr(Position pos);
	Building * getBuildingPtr(Position pos);

	unit_type getUnitType(Position pos);
	teams_d getUnitTeam(Position pos);
	teams_d getBuildingTeam(Position pos);
	bool getFog(Position pos);
	terrains_d getTerrain(Position pos);
	Tile * getTilePtr(Position pos);

	teams_d getTeam(); //del mapa
	teams_d getEnemyTeam();

	bool IsUnitOnTop(Position pos);
	bool IsBuildingOnTop(Position pos);
	
	void setTeam(teams_d team);
	void setEnemyTeam(teams_d enemyTeam);

	void addTile(Position pos, terrains_d type, bool fog);
	bool addBuilding(buildings_d type, teams_d color, Position pos);
	bool addUnit(units_d unitClass, Position pos, teams_d owner);
	
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
	void changeUnitPos(Unit * unit, Position newPos);
	void clearFog(Position pos); // saca la fog de la tile de arriba abajo derecha e izquierda de la posición que le mando
	void selectTile(Position pos);
	void unselectTile(Position pos);
	void unloadAPC(Position pos, Position newPos);

	bool posInMap(Position pos);


	p_inv_s getPlayerInventory(teams_d color);
	options_s getOptions(Position pos); //cambia si en esa tile hay solo building, solo unit o hay ambas 

	list<Position> getPossibleAttacks(Unit * unit);
	bool IsValidAttack(Unit * unit, Position WhereTO);
	bool attack(Unit * unit, Position whereTo, unsigned int dice);
	bool move(Position WhereTo, Unit * unit);
	bool capture(Unit * unit, Position pos);
	bool IsValidMove(Unit * unit, Position WhereTO);
	bool loadAPC(Unit * unit, Position pos);

	bool enemyAttack(Unit * unit, Position whereTo, unsigned int dice);
	bool IsValidEnemyAttack(Unit * unit, Position WhereTO);
	list<Position> getPossibleEnemyAttacks(Unit * unit);

	bool enemyMove(Position WhereTo, Unit * unit);
	bool IsValidEnemyMove(Unit * unit, Position WhereTO);


	void updateCP();

	void endTurnUnits();


	void getPossibleMoves(Unit * unit, int currMPs, moves_s temp ,list<moves_s> * moves); //incluye lugares doende se puede capturar a loadear a un apc

	unsigned int getMoveMPS(Unit * unit, Position destination);

private:
	Tile * board[BOARD_HEIGHT][BOARD_WIDTH]; //para agregar tile uso la position
	teams_d team;
	teams_d enemyTeam;
};

#endif