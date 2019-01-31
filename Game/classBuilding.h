#pragma once
#include "Position.h"

#define HQ_CP 8
#define FACTORY_CP 4
#define CITY_CP 4

#define REDUCED_CP 1
#define FULL_STRENTH_CP 2

typedef enum buildings { HQ, FACTORY, CITY, NO_BUILDING} buildings_d;
typedef enum { TEAM_RED, TEAM_BLUE, TEAM_GREEN, TEAM_YELLOW, NEUTRAL, ERROR_TEAM }teams_d;

class Building {
public:
	Building(buildings_d type, teams_d color, Position pos);//setea los CP según el tipo de building

	buildings_d getBuildingType();
	unsigned int getCapturePoints();
	teams_d getBuildingTeam();
	Position getPosition();

	bool BuildingUnderAttack(); 
	void captureBuilding(teams_d color, bool UnitReduced); //resta los CP dependiendo si la unidad está reducida y si la terminó de capturar(CP = 0) setea el team correspondiente
	void setToNeutral();
	void resetCapturePoints();
		

private:
	buildings_d type;
	teams_d color;
	unsigned int capturePoints;
	Position pos;
};
