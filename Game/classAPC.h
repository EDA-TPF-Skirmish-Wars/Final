#ifndef CLASSAPC_H
#define CLASSAPC_H

#include "./classUnit.h"
//#include "./classBuilding.h"
//#include "./Position.h"
//#include "./classAPC.h"
//#include "./classPlayer.h"
//#include "./classMap.h"
//#include "./unitsInfo.h"
//#include "./TileClass.h"

#define APC_MAX_LOAD 2

using namespace std;

class classAPC: public Unit
{
public:
	classAPC(Position pos, teams_d owner);
	~classAPC(); //destruir las unidades que queden aadntro

	bool isFull();	// Si esta lleno el APC devuelve un true

	bool loadUnitIfPossible(Unit unitToLoad, teams_d colorToLoad); //ver cuando se puede 

	Unit * unloadingUnitIfPossible(Position pos); 

	void healLoadedUnits();
	void ChangeUnitsPosition();

	bool canLoad(teams_d colorToLoad);
	bool canUnload(Position pos);
	void endtTurnLoadedUnit();



private:
	unsigned int NUnitsInside; //cuantifica la cantidad de unidades que tiene adentro 0 1 2
	list<Unit*> UnitsLoaded;
	teams_d owner;
	Position pos;
};

#endif