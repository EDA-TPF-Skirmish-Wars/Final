#include "TileClass.h"

Tile::Tile(Position pos, terrains_d type, bool fog)
{
	unitOnTop = nullptr;
	buildingOnTop = nullptr;
	this->fog = fog;
	this->pos = pos;
	this->type = type;
	this->select_status = SELECT_OFF;

}

Tile::~Tile()
{
	if (buildingOnTop != nullptr)
	{
		delete buildingOnTop;
		buildingOnTop = nullptr;
	}

	if (unitOnTop != nullptr)
	{
		delete unitOnTop;
		unitOnTop = nullptr;
	}
}

Position Tile::getPosition()
{
	return pos;
}

terrains_d Tile::getTypeOfTerrain()
{
	if (buildingOnTop = nullptr)
		return type;
	else
		return BUILDING;
}

bool Tile::getFog()
{
	return fog;
}

Unit * Tile::getUnit()
{
	return unitOnTop;
}

Building * Tile::getBuilding()
{
	return buildingOnTop;
}

void Tile::clearFog()
{
		fog = FOG_OFF;
}

bool Tile::IsUnitOnTop()
{
	return unitOnTop != nullptr;
}

bool Tile::IsBuildingOnTop()
{
	return buildingOnTop != nullptr;
}

bool Tile::setUnit(Unit * unitOnTop)
{
	if (this->unitOnTop == nullptr)
	{
		this->unitOnTop = unitOnTop;
	}
	else
		return false;
}
bool Tile::setBuilding(Building * buildingOnTop)
{
	if (this->buildingOnTop == nullptr)
	{
		this->buildingOnTop = buildingOnTop;
	}
	else
		return false;
}
void Tile::setTile(Position pos, terrains_d type, bool fog)
{
	this->pos = pos;
	this->type = type;
	this->fog = fog;
}

void Tile::selectTile()
{
	if (fog == FOG_OFF)
		select_status = SELECT_ON;
}

void Tile::unselectTile()
{
	if (select_status == SELECT_ON)
		select_status = SELECT_OFF;
}


void Tile::removeUnit()
{
	if (unitOnTop != nullptr)
		unitOnTop = nullptr;
}






