//#include "classAPC.h"
//
//using namespace std;
//
//classAPC::classAPC(Position pos, teams_d owner) 
//{
//	this->pos = pos;
//	this->owner = owner;
//	UnitsLoaded.clear();
//}
//
//classAPC::~classAPC()
//{
//	while (UnitsLoaded.empty())
//	{
//		delete UnitsLoaded.front();
//		UnitsLoaded.pop_front();
//	}
//}
//
//bool classAPC::isFull()
//{
//
//	return (UnitsLoaded.size() == APC_MAX_LOAD);
//}
//
//bool classAPC::canLoad(teams_d colorToLoad)
//{
//	if (isFull() == false && colorToLoad == this->owner)
//		return true;
//	else
//		return false;
//}
//
//bool classAPC::canUnload(Position pos)
//{
//	if ((abs(pos.row - this->pos.row) + abs(pos.column - this->pos.column)) == 1)
//		return true;
//	else
//		return false;
//}
//
//bool classAPC::loadUnitIfPossible(Unit unitToLoad, teams_d colorToLoad)
//{
//	if (isFull() == false && colorToLoad == owner)
//	{
//		UnitsLoaded.push_back(&unitToLoad);
//		return true;
//	}
//	else
//		return false;
//}
//
//Unit * classAPC::unloadingUnitIfPossible(Position pos)
//{
//	Unit *  unitUnloaded = nullptr;
//	if (!UnitsLoaded.empty())
//	{
//		if ((abs(pos.row - this->pos.row) + abs(pos.column - this->pos.column)) == 1) //se descarga solo a lugaes que esten a 1 de distancia
//		{
//			unitUnloaded = UnitsLoaded.back();
//			unitUnloaded->ChangeUnitPosition(pos);
//			UnitsLoaded.pop_back();
//			
//		}
//	}
//	return unitUnloaded;
//}
//
//void classAPC::ChangeAPCpos(Position pos)
//{
//	this->pos = pos;
//}
//
//void classAPC::healLoadedUnits()
//{
//	list<Unit*>::iterator iter;
//	iter = this->UnitsLoaded.begin();
//	while(iter != this->UnitsLoaded.end())
//	{
//		(*iter)->heal();
//	}
//}
//void classAPC::ChangeUnitsPosition()
//{
//	list<Unit*>::iterator iter;
//	iter = this->UnitsLoaded.begin();
//	while (iter != this->UnitsLoaded.end())
//	{
//		(*iter)->ChangeUnitPosition(this->pos);
//	}
//}
//
//
//
//void classAPC::endtTurnLoadedUnit()
//{
//	list<Unit*>::iterator iter;
//	iter = this->UnitsLoaded.begin();
//	while (iter != this->UnitsLoaded.end())
//	{
//		(*iter)->endTurn();
//	}
//}



