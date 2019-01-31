#ifndef TILECLASS_H
#define TILECLASS_H

#include "./Position.h"
#include "./classUnit.h"
#include "./unitsInfo.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Tile
{
public:
	friend class Map;
	
	Tile(Position pos, terrains_d type, bool fog); //puntero unit y building NULL
	~Tile(); //borrar punteros

	Position getPosition();
	terrains_d getTypeOfTerrain();
	bool getFog();
	Unit * getUnit();
	Building * getBuilding();

	void clearFog();
	
	bool IsUnitOnTop();
	bool IsBuildingOnTop();

	bool setUnit(Unit * unitOnTop); //ver si hay building de capturar o de curar la unit
	bool setBuilding(Building * buildingOnTop);
	void setTile(Position pos, terrains_d type, bool fog);

	void removeUnit(); //ver si hay unit y building dejar de capturar

	void selectTile();
	void unselectTile();
	
private:
	Position pos;
	terrains_d type;
	Unit * unitOnTop;
	Building * buildingOnTop;
	bool fog; 
	bool select_status;
	string imagePath;
};

#endif // !TILECLASS_H