#include "classAPC.h"

classAPC::classAPC(Position pos, teams_d owner) : Unit(APC, pos, owner)
{
	this->pos = pos;
	this->owner = owner;
	NUnitsInside = 0;
	UnitsLoaded.clear();
}

classAPC::~classAPC()
{
	while (UnitsLoaded.empty())
	{
		delete UnitsLoaded.front();
		UnitsLoaded.pop_front();
	}
}

bool classAPC::isFull()
{
	return (NUnitsInside == APC_MAX_LOAD);
}

bool classAPC::canLoad(teams_d colorToLoad)
{
	if (isFull() == false && colorToLoad == this->owner)
		return true;
	else
		return false;
}

bool classAPC::canUnload(Position pos)
{
	if ((abs(pos.row - this->pos.row) + abs(pos.column - this->pos.column)) == 1)
		return true;
	else
		return false;
}

bool classAPC::loadUnitIfPossible(Unit unitToLoad, teams_d colorToLoad)
{
	if (NUnitsInside < APC_MAX_LOAD && colorToLoad == owner)
	{
		UnitsLoaded.push_back(&unitToLoad);
		NUnitsInside++;
		return true;
	}
	else
		return false;
}

Unit * classAPC::unloadingUnitIfPossible(Position pos)
{
	Unit *  unitUnloaded = nullptr;
	if (!UnitsLoaded.empty())
	{
		if ((abs(pos.row - this->pos.row) + abs(pos.column - this->pos.column)) == 1) //se descarga solo a lugaes que esten a 1 de distancia
		{
			unitUnloaded = UnitsLoaded.back();
			unitUnloaded->ChangeUnitPosition(pos);
			UnitsLoaded.pop_back();
			NUnitsInside--;
		}
	}
	return unitUnloaded;
}

void classAPC::healLoadedUnits()
{
	list<Unit*>::iterator iter;
	iter = this->UnitsLoaded.begin();
	while(iter != this->UnitsLoaded.end())
	{
		(*iter)->heal();
	}
}
void classAPC::ChangeUnitsPosition()
{
	list<Unit*>::iterator iter;
	iter = this->UnitsLoaded.begin();
	while (iter != this->UnitsLoaded.end())
	{
		(*iter)->ChangeUnitPosition(this->pos);
	}
}