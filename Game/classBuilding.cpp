#include "classBuilding.h"

Building::Building(buildings_d type, teams_d color, Position pos)
{
	this->type = type;
	this->color = color;
	this->pos = pos;

	resetCapturePoints();
}

buildings_d Building::getBuildingType()
{
	return type;
}
unsigned int Building::getCapturePoints()
{
	return capturePoints;
}
teams_d Building::getBuildingTeam()
{
	return color;
}
Position Building::getPosition()
{
	return pos;
}

bool Building::BuildingUnderAttack()
{
	if (type == HQ)
		return (capturePoints != HQ_CP);
	else
		return (capturePoints != CITY_CP);
}

void Building::captureBuilding(teams_d color, bool UnitReduced)
{
	if (color != this->color)
	{
		if (UnitReduced)
		{
			if (capturePoints >= REDUCED_CP)
				capturePoints = capturePoints - REDUCED_CP;
			else
				resetCapturePoints();
				this->color = color;
		}
		else
		{
			if (capturePoints >= FULL_STRENTH_CP)
				capturePoints = capturePoints - FULL_STRENTH_CP;
			else
				resetCapturePoints();
				this->color = color;
		}
	}
}
void Building::setToNeutral()
{
	this->color = NEUTRAL;
}

void Building::resetCapturePoints()
{
	if (type == HQ)
	{
		capturePoints = HQ_CP;
	}
	else
	{
		capturePoints = FACTORY_CP;
	}
}