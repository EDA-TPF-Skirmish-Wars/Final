#pragma once
#include "unitsInfo.h"
#include "classMap.h"
#include "classAPC.h"
#include <list>

typedef enum { SELECTED, ATTACKING, MOVING, BLOCKED, DEAD, IDLE} unit_state_d; // estados dependiendo en si pueden ser clickeadas o no

class Unit {
public:

	Unit(units_d unitClass, Position pos, teams_d owner);
	~Unit();

	static unsigned int getCost(units_d unitClass);
	unsigned int getCost();
	unsigned int getActualMP();
	unsigned int getMaxMps();
	unsigned int getDefense();
	unsigned int getHP();
	unsigned int getTerrainMC(terrains_d type);
	unsigned int getAttackFP(unit_type TargetUnitType, bool Isreduced);
	Position getPosition();
	teams_d getTeam();
	units_d getUnitClass();
	unit_type getType();
	unsigned int getMaxRange();
	unsigned int getMinRange();

	unsigned int getMoveMPS(Position origin, Position destination, Map map);


	bool isReduced();
	bool isAlive();

	void heal();
	bool move(Position WhereTo, Map map);
	bool attack(Map map, Position whereTo, unsigned int dice);
	bool capture(Map map, Position pos);
	bool loadAPC(Map map, Position pos);
	
	void ChangeUnitPosition(Position where);
	static Unit* boughtUnit(units_d unitClass, Position position, teams_d color);

	bool IsValidMove(Map map, Position WhereTO); //VER mp!!! que devuelva los que necesita
	bool IsValidAttack(Map map, Position WhereTO);

	int attackDamage(int initdamage, unsigned dice, terrains_d enemyTerrain, buildings_d isThereBuilding); //Devuelve el fp con el que ataca la unuidad( le paso el damage con el defence ya restado)
	
	void blockUnit(); //ya no la puedo usar mas durante el turno
	void selectUnit();
	void endTurn(); //reseteo variables necesarias
	void resetMP();
	
	list<Position> getPossibleMoves( Position tempPos, Map map, int currMPs);
	list<Position> getPossibleAttacks(Map map);

	bool isItAPC();
	bool ifAPCisFull();


private:
	units_d unitClass;
	unit_type type;
	teams_d owner;
	Position pos;

	static unsigned int maxMP;
	unsigned int movingPoints;
	unsigned int healthPoints;

	static unsigned int unitCost;

	unsigned int rangeMax;
	unsigned int rangeMin;

	unsigned int defense;

	unsigned int mcGrass;
	unsigned int mcRoad;
	unsigned int mcForest;
	unsigned int mcRiver;
	unsigned int mcHill;

	unsigned int firePowerRatingFOOT;
	unsigned int firePowerRatingTREAD;
	unsigned int firePowerRatingWHEEL;

	unsigned int firePowerReducedFOOT;
	unsigned int firePowerReducedTREAD;
	unsigned int firePowerReducedWHEEL;

	unit_state_d status;

};

